#!/usr/bin/env bash

export ASAN_OPTIONS=detect_leaks=0

#echo "$(ls -la /tmp/corpus| wc -l) corpus entries were found in /tmp/corpus"
#cifuzz coverage ntpd_fuzzer_coverage --build-command "id" -v --use-sandbox=false --format lcov --add-corpus /tmp/corpus

cifuzz coverage ntpd_fuzzer_coverage --build-command "id" -v --use-sandbox=false --format lcov 

# copy lcov report to /tmp on host
if cp ntpd_fuzzer_coverage.coverage.lcov /tmp/lcov.info; then
  echo "Coverge Report copied to /tmp/lcov.info"
fi