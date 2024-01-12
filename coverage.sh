#!/usr/bin/env bash

arg=$1

export ASAN_OPTIONS=detect_leaks=0

if [ "$arg" == "external" ]; then
    #echo "$(ls -la /tmp/corpus| wc -l) corpus entries were found in /tmp/corpus"
    #cifuzz coverage ntpd_fuzzer_coverage_external --build-command "id" -v --use-sandbox=false --format lcov --add-corpus /tmp/corpus
    cifuzz coverage ntpd_fuzzer_coverage_external --build-command "id" -v --use-sandbox=false --format lcov 

    # copy lcov report to /tmp on host
    if cp ntpd_fuzzer_coverage_external.coverage.lcov /tmp/lcov.info; then
    echo "Coverge Report copied to /tmp/lcov.info"
    fi
elif [ "$arg" == "internal" ] || [ -z "$arg" ]; then
    #echo "$(ls -la /tmp/corpus| wc -l) corpus entries were found in /tmp/corpus"
    #cifuzz coverage ntpd_fuzzer_coverage_internal --build-command "id" -v --use-sandbox=false --format lcov --add-corpus /tmp/corpus
    cifuzz coverage ntpd_fuzzer_coverage_internal --build-command "id" -v --use-sandbox=false --format lcov 

    # copy lcov report to /tmp on host
    if cp ntpd_fuzzer_coverage_internal.coverage.lcov /tmp/lcov.info; then
    echo "Coverge Report copied to /tmp/lcov.info"
    fi

else
    echo "Usage: $0 [internal|external]"
fi