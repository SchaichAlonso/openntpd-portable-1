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

  double freq = 0.0;
  imsg_compose(&cifuzz_pipe_to_ntpd, /* IMSG_ADJFREQ */ 2, 0, 0, -1, &freq, sizeof(freq));
  while (msgbuf_write(&cifuzz_pipe_to_ntpd.w) <= 0) {
    assert(errno == EAGAIN);
    printf("too fast\n");
    sleep(1);
  }
}
