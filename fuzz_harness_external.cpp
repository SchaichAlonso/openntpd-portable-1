#include <assert.h>

#include <cifuzz/cifuzz.h>
#include <fuzzer/FuzzedDataProvider.h>

#include <iostream>
#include <string>

#include <arpa/inet.h> // htons, inet_addr
#include <netinet/in.h> // sockaddr_in
#include <sys/types.h> // uint16_t
#include <sys/socket.h> // socket, sendto
#include <unistd.h> // close

#include <thread>

int sock;
sockaddr_in destination;

FUZZ_TEST_SETUP() {
  // Perform any one-time setup required by the FUZZ_TEST function.
  std::cerr << "External Fuzzer: Wait for the application to start..." << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(10000));
  
  std::string hostname{"172.17.0.2"};
  uint16_t port = 123;

  sock = ::socket(AF_INET, SOCK_DGRAM, 0);

  destination.sin_family = AF_INET;
  destination.sin_port = htons(port);
  destination.sin_addr.s_addr = inet_addr(hostname.c_str());
}



FUZZ_TEST(const uint8_t *data, size_t size) {
  
  FuzzedDataProvider fuzzed_data(data, size);
  std::string msg = fuzzed_data.ConsumeRandomLengthString();


  int n_bytes = ::sendto(sock, msg.c_str(), msg.length(), 0, reinterpret_cast<sockaddr*>(&destination), sizeof(destination));
  //std::cout << n_bytes << " bytes sent" << std::endl;
  //::close(sock);

}
