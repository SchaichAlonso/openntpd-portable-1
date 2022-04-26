set -e
BUILDPROJECT=1
if test $BUILDPROJECT = 1
then
    ./autogen.sh
    ./configure
    cp .code-intelligence/server.c src/server.c
    cp .code-intelligence/util.c src/util.c
    make clean
    make -j$(nproc)
fi
# Note: The build script will be executed from the project's root directory.
