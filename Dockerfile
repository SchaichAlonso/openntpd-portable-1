FROM ubuntu

ENV TZ=Europe/Berlin

# required for non-interactive installation tzdata 
ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update -o Acquire::Check-Valid-Until=false -o Acquire::Check-Date=false && apt-get -y install git automake autoconf libtool bison gdb gdbserver nano clang llvm lcov curl make tzdata nscd strace

RUN mkdir /openntpd-portable
COPY . /openntpd-portable 
COPY ./ntpd.conf /usr/local/etc/ntpd.conf
COPY ./ntpd.conf /etc/ntpd.conf

# set up some ntpd run env requirements
RUN groupadd _ntp
RUN useradd -g _ntp -s /sbin/nologin -d /var/empty -c 'OpenNTP daemon' _ntp
RUN mkdir -p /var/empty
RUN chown 0 /var/empty
RUN chgrp 0 /var/empty
RUN chmod 0755 /var/empty
RUN mkdir -p /usr/local/var/run

# install pwndbg for better debugging experience
# RUN apt-get update  -o Acquire::Check-Valid-Until=false -o Acquire::Check-Date=false && apt-get install sudo gdb-multiarch gcc g++ wget cmake pkg-config binutils -y
# RUN mkdir /pwndbg
# RUN git clone https://github.com/pwndbg/pwndbg /pwndbg
# RUN cd /pwndbg && ./setup.sh


# install cifuzz
RUN sh -c "$(curl -fsSL https://raw.githubusercontent.com/CodeIntelligenceTesting/cifuzz/main/install.sh)"
RUN cifuzz --version

# nscd required to be running by ntpd
RUN /etc/init.d/nscd start

COPY ./run.sh /openntpd-portable/run.sh

WORKDIR /openntpd-portable

# build ntpd and fuzzer linked against libfuzzer_no_main
RUN cifuzz run ntpd_fuzzer_external --build-only --build-command "./autogen.sh; ./configure --disable-dependency-tracking AM_DEFAULT_VERBOSITY=1; make; make main_hook; make ntpd_coverage; make fuzzer_external; make fuzzer_internal" -v --use-sandbox=false

# on host 
# docker build . -t ntpd-env
# docker run --cap-add SYS_TIME --cap-add SYS_NICE -v /dev/log:/dev/log -it ntpd-env /openntpd-portable/run.sh
# 
# in container
# ./run.sh
# 
# if you want to debug inside container
# docker run --cap-add SYS_TIME --cap-add SYS_NICE --cap-add=SYS_PTRACE --security-opt seccomp=unconfined -v /dev/log:/dev/log -it ntpd-env bash
