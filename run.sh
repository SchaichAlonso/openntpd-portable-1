#!/usr/bin/env bash

# enable for debugging LSAN issues
# LSAN_OPTIONS=verbosity=1:log_threads=1 

# There is an LSAN Error thrown when fuzzer is started. It is only thrown when started with the fuzzer 
export ASAN_OPTIONS=detect_leaks=0

# create seed corpus dir in case it not created yet
mkdir -p /tmp/corpus

#/openntpd-portable/ntpd_fuzzer
cifuzz run ntpd_fuzzer --build-command "id" --use-sandbox=false -v --seed-corpus /tmp/corpus

# copy corpus over to mounted /tmp on host
if cp .cifuzz-corpus/ntpd_fuzzer/* /tmp/corpus; then
  echo "Cifuzz corpus ($(ls -la /openntpd-portable/.cifuzz-corpus/ntpd_fuzzer/* |wc -l) entries) was copied to /tmp/corpus"
fi

# for debugging
# LC_CTYPE=C.UTF-8 gdb --args /openntpd-portable/ntpd_fuzzer -d -f /etc/ntpd.conf
