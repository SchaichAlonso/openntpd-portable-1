cifuzz run ntpd_fuzzer_coverage \
    --build-only \
    --build-command 'export CFLAGS="${CFLAGS} -fprofile-instr-generate -fcoverage-mapping"; env; ./autogen.sh; ./configure --disable-dependency-tracking AM_DEFAULT_VERBOSITY=1; make V=1 all ntpd_fuzzer_coverage ntpd_fuzzer' \
    --verbose