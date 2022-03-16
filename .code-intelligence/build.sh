set -e
BUILDPROJECT=0
if test $BUILDPROJECT = 1
then
    ./autogen.sh
    ./configure
    make clean
    make -j$(nproc)
fi
# Note: The build script will be executed from the project's root directory.
