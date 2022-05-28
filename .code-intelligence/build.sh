set -e
BUILDPROJECT=1
if test $BUILDPROJECT = 1
then
    ./autogen.sh
    ./configure
    cp .code-intelligence/server.c src/server.c
    cp .code-intelligence/util.c src/util.c
    cp .code-intelligence/ntpd.c src/ntpd.c
    cp .code-intelligence/ntp.c src/ntp.c
    cp .code-intelligence/ntp_dns.c src/ntp_dns.c
    make clean
    make -j$(nproc)
fi
# Note: The build script will be executed from the project's root directory.
