#!/usr/bin/env bash

# enable for debugging LSAN issues
# LSAN_OPTIONS=verbosity=1:log_threads=1 

# There is an LSAN Error thrown when fuzzer is started. It is only thrown when started with the fuzzer 
export ASAN_OPTIONS=detect_leaks=0

/openntpd-portable/ntpd_fuzzer

# for debugging
# LC_CTYPE=C.UTF-8 gdb --args /openntpd-portable/ntpd_fuzzer -d -f /etc/ntpd.conf
