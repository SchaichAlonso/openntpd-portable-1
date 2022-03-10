set -e
./autogen.sh
./configure
make -j$(nproc)
# Note: The build script will be executed from the project's root directory.