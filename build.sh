cifuzz run ntpd_fuzzer_internal \
    --build-only \
    --build-command 'export CFLAGS="${CFLAGS} -fprofile-instr-generate -fcoverage-mapping"; env; ./autogen.sh; ./configure --disable-dependency-tracking AM_DEFAULT_VERBOSITY=1; make V=1 all fuzzer_internal' \
    --verbose