FROM ubuntu

RUN apt-get update && apt-get -y install git automake autoconf libtool bison gdb gdbserver nano clang llvm lcov curl make

ENV TZ=Europe/Berlin
ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt -y install tzdata nscd strace

RUN mkdir /openntpd-portable
COPY . /openntpd-portable 
COPY ./ntpd.conf /usr/local/etc/ntpd.conf

RUN groupadd _ntp
RUN useradd -g _ntp -s /sbin/nologin -d /var/empty -c 'OpenNTP daemon' _ntp

RUN mkdir -p /var/empty
RUN chown 0 /var/empty
RUN chgrp 0 /var/empty
RUN chmod 0755 /var/empty

RUN mkdir -p /usr/local/var/run




RUN sh -c "$(curl -fsSL https://raw.githubusercontent.com/CodeIntelligenceTesting/cifuzz/main/install.sh)"
RUN cifuzz --version

RUN /etc/init.d/nscd start

WORKDIR /openntpd-portable

# RUN make

# on ubuntu host check if /run/systemd/journal/dev-log is a socket
# $ ls -l /run/systemd/journal/dev-log
# srw-rw-rw- 1 root root 0 Dec 16 09:17 /run/systemd/journal/dev-log
#
# sudo ln -s /run/systemd/journal/dev-log /dev/log
#
#  docker run --cap-add SYS_TIME --cap-add SYS_NICE -v /dev/log:/dev/log -it testimage bash
#
#  docker run --cap-add SYS_TIME --cap-add SYS_NICE -v /dev/log:/dev/log -it testimage /openntpd-portable/src/ntpd -d