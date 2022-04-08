FROM cifuzz/builder-c:1.1
ENV TZ=Europe/Berlin
RUN cat /etc/*release && apt-get update && apt-get -y install automake autoconf git libtool bison tzdata nscd

COPY ./ntpd.conf /usr/local/etc/ntpd.conf
RUN groupadd _ntp
RUN useradd -g _ntp -s /sbin/nologin -d /var/empty -c 'OpenNTP daemon' _ntp
RUN mkdir -p /var/empty
RUN chown 0 /var/empty
RUN chgrp 0 /var/empty
RUN chmod 0755 /var/empty

RUN mkdir mkdir -p /usr/local/var/run