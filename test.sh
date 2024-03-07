find /tmp -name 'ntpd.*.pid' -exec rm -v {} \;
find /tmp -name '*.profraw' -exec rm -v {} \;
find /tmp -name '*.profdata' -exec rm -v {} \;

export ASAN_OPTIONS=detect_leaks=0
export NO_CIFUZZ=1
export LLVM_PROFILE_FILE='/tmp/ntpd_fuzzer_coverage.%p.profraw'

/openntpd-portable/ntpd_fuzzer_coverage -runs=10000
llvm-profdata merge -sparse /tmp/*.profraw -o /tmp/ntp_fuzzer_coverage.profdata
llvm-cov report ./ntpd_fuzzer_coverage -instr-profile=/tmp/ntp_fuzzer_coverage.profdata
