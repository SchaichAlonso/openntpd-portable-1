#include <assert.h>

#include <cifuzz/cifuzz.h>
#include <fuzzer/FuzzedDataProvider.h>

#include <iostream>
#include <string>

#include <arpa/inet.h> // htons, inet_addr
#include <netinet/in.h> // sockaddr_in
#include <sys/types.h> // uint16_t
#include <sys/socket.h> // socket, sendto
#include <pthread.h>
#include <signal.h>
#include <unistd.h> // close

#include <thread>

#include "libfuzzer_no_main/main_hook.h"

#include <sys/queue.h> // used by, but not included by, <imsg.h>
extern "C" {
#include "include/imsg.h" // needs to be in extern C block
#include "src/ntpd.h" // for IMSG_ADJFREQ and friends

extern struct imsgbuf *ibuf; // from ntpd.c
}

int sock;
sockaddr_in destination;

int cifuzz_pipe_chld[2];
struct imsgbuf cifuzz_pipe_to_ntpd;

void cifuzz_pipe_inject()
{
  assert(cifuzz_ntpd_is_running());

  int retval = socketpair(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, PF_UNSPEC, &cifuzz_pipe_chld[0]);
  assert(retval == 0);

  memset(&cifuzz_pipe_to_ntpd, 0, sizeof(cifuzz_pipe_to_ntpd));

  imsg_init(ibuf, cifuzz_pipe_chld[0]);
  imsg_init(&cifuzz_pipe_to_ntpd, cifuzz_pipe_chld[1]);

  /*
   * Wakeup `poll`-ing ntpd
   */
  cifuzz_ntpd_send_sighup();
}

FUZZ_TEST_SETUP() {
  // Perform any one-time setup required by the FUZZ_TEST function.
  std::this_thread::sleep_for(std::chrono::milliseconds(10000));
  
  std::string hostname{"172.17.0.2"};
  uint16_t port = 123;

  sock = ::socket(AF_INET, SOCK_DGRAM, 0);

  destination.sin_family = AF_INET;
  destination.sin_port = htons(port);
  destination.sin_addr.s_addr = inet_addr(hostname.c_str());

  cifuzz_pipe_inject();
}



FUZZ_TEST(const uint8_t *data, size_t size) {
  
  FuzzedDataProvider fuzzed_data(data, size);
  std::string msg = fuzzed_data.ConsumeRandomLengthString();


  int n_bytes = ::sendto(sock, msg.c_str(), msg.length(), 0, reinterpret_cast<sockaddr*>(&destination), sizeof(destination));
  //std::cout << n_bytes << " bytes sent" << std::endl;
  //::close(sock);

  assert(cifuzz_ntpd_is_running());

  int imsg_type = fuzzed_data.ConsumeIntegral<int>();
  int imsg_id = 0; /* fuzz? */
  pid_t imsg_pid = 0; /* don't touch */
  int imsg_fd = -1; /* don't touch */
  std::vector<uint8_t> imsg_data = fuzzed_data.ConsumeRemainingBytes<uint8_t>();

  if (imsg_data.size() == 0) {
    // this is unit test material.
    return;
  }

swtich (imsg_type) {
  case IMSG_ADJTIME:
  case IMSG_ADJFREQ:
  case IMSG_SETTIME:
    if (imsg_data.size() < sizeof(double)) {
      return;
    }
    imsg_data.resize(sizeof(double));
    break;
  default:
    break;
}

#if 0
  double freq = 0.0;
  imsg_compose(&cifuzz_pipe_to_ntpd, /* IMSG_ADJFREQ */ 2, 0, 0, -1, &freq, sizeof(freq));
#else
  imsg_compose(&cifuzz_pipe_to_ntpd, imsg_type, imsg_id, imsg_pid, imsg_fd, &imsg_data[0], imsg_data.size());
#endif
  while (msgbuf_write(&cifuzz_pipe_to_ntpd.w) <= 0) {
    assert(errno == EAGAIN);
    printf("too fast\n");
    sleep(1);
  }
}
