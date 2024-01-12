#!/usr/bin/env bash

# enable for debugging LSAN issues
# LSAN_OPTIONS=verbosity=1:log_threads=1 

# There is an LSAN Error thrown when fuzzer is started. It is only thrown when started with the fuzzer 
export ASAN_OPTIONS=detect_leaks=0

# create seed corpus dir in case it not created yet
mkdir -p /tmp/corpus

if [ "$arg" == "external" ]; then
    #/openntpd-portable/ntpd_fuzzer_external
    cifuzz run ntpd_fuzzer_external --build-command "id" --use-sandbox=false -v --seed-corpus /tmp/corpus
    # for debugging
    # LC_CTYPE=C.UTF-8 gdb --args /openntpd-portable/ntpd_fuzzer_external -d -f /etc/ntpd.conf
    
    # copy corpus over to mounted /tmp on host
    if cp .cifuzz-corpus/ntpd_fuzzer_external/* /tmp/corpus; then
        echo "Cifuzz corpus ($(ls -la /openntpd-portable/.cifuzz-corpus/ntpd_fuzzer_external/* |wc -l) entries) was copied to /tmp/corpus"
    fi
elif [ "$arg" == "internal" ] || [ -z "$arg" ]; then
    #/openntpd-portable/ntpd_fuzzer_internal
    cifuzz run ntpd_fuzzer_internal --build-command "id" --use-sandbox=false -v --seed-corpus /tmp/corpus
    # for debugging
    # LC_CTYPE=C.UTF-8 gdb --args /openntpd-portable/ntpd_fuzzer_internal -d -f /etc/ntpd.conf
    
    # copy corpus over to mounted /tmp on host
    if cp .cifuzz-corpus/ntpd_fuzzer_internal/* /tmp/corpus; then
        echo "Cifuzz corpus ($(ls -la /openntpd-portable/.cifuzz-corpus/ntpd_fuzzer_internal/* |wc -l) entries) was copied to /tmp/corpus"
    fi
else
    echo "Usage: $0 [internal|external]"
fi
