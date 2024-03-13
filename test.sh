FUZZ_TARGET=ntpd_fuzzer_coverage
FUZZ_TARGET_PATH=/openntpd-portable/ntpd_fuzzer_coverage

find /tmp -name 'ntpd.*.pid' -exec rm -v {} \;
find /tmp -name "${FUZZ_TARGET}.*.profraw" -exec rm -v {} \;
find /tmp -name "${FUZZ_TARGET}.profdata" -exec rm -v {} \;

export ASAN_OPTIONS=detect_leaks=0
export NO_CIFUZZ=1
export LLVM_PROFILE_FILE=/tmp/${FUZZ_TARGET}.%p.profraw

${FUZZ_TARGET_PATH} -runs=10000000
llvm-profdata merge -sparse /tmp/${FUZZ_TARGET}.*.profraw -o /tmp/${FUZZ_TARGET}.profdata
llvm-cov report ${FUZZ_TARGET} -instr-profile=/tmp/${FUZZ_TARGET}.profdata
