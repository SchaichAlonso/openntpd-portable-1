#include <stdint.h>
#include <stddef.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <file_input.h> // ci fuzz file input helper

extern "C" { /*
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h> */
#include "ntpd.h"


int parse_config(const char *filename, struct ntpd_conf *xconf);
}
// extern "C" int FUZZ_INIT_WITH_ARGS(int *argc, char ***argv) {
extern "C" int FUZZ_INIT() {

  // Add global setup code here - called once before fuzzing starts

  return 0;
}

extern "C" int FUZZ(const uint8_t *Data, size_t Size) {
  if (Size < sizeof(ntpd_conf)) return 0;
  // process and split fuzzer input (*Data) into ntpd_conf struct contents and file contents
  FuzzedDataProvider dp(Data, Size);
  std::vector<uint8_t> confVector = dp.ConsumeBytes<u_int8_t>(sizeof(ntpd_conf));
  std::vector<uint8_t> remaining = dp.ConsumeRemainingBytes<uint8_t>();

  // and setup local objects necessary to call the function under test

  ntpd_conf * conf = (ntpd_conf *) confVector.data(); 
  auto ci_file_input = ci::input_file{remaining.data(), remaining.size()};
  parse_config(ci_file_input.name(),conf);

  // reset state and free all locally allocated resources

  return 0;
}
