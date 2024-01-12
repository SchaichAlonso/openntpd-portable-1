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
#include <pthread.h>

#include <cerrno>
#include <fcntl.h>

#define PARENT_SOCK_FILENO		3
#define NTP_MSGSIZE           68 // see ntp.h  

extern "C" pthread_mutex_t processing_input_mutex;
extern "C" uint8_t processing_input;

int sock;
sockaddr_in destination;

FUZZ_TEST_SETUP() {
  // Perform any one-time setup required by the FUZZ_TEST function.
  std::cerr << "Internal Fuzzer: Wait for the application to start..." << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(5000));
  
  // set socket non-blocking flags
  int flags;
  flags = fcntl(PARENT_SOCK_FILENO, F_GETFL, 0);
  if (flags == -1) {
    perror("fcntl");
  }
  flags |= O_NONBLOCK;
  if ((flags = fcntl(PARENT_SOCK_FILENO, F_SETFL, flags)) == -1) {
    perror("fcntl");
    std::cerr << "set NONBLOCK flag for socket did not work" << std::endl;
  }

  // some intercepted valid imsg
  // ssize_t bytes_written = write(PARENT_SOCK_FILENO, "\x01\x00\x00\x00\x24\x00\x00\x00\x00\x00\x00\x00\x20\x00\x00\x00\x00\x80\xe2\x84\xdc\xfd\x11\x00", 24);
}


FUZZ_TEST(const uint8_t *data, size_t size) {
  
  FuzzedDataProvider fuzzed_data(data, size);
  std::string msg = fuzzed_data.ConsumeRandomLengthString();

  if (size < NTP_MSGSIZE && size > 0 ) {
    pthread_mutex_lock(&processing_input_mutex);
    processing_input = 1;

    ssize_t bytes_written = write(PARENT_SOCK_FILENO, msg.c_str(), msg.length());
    //std::cerr << bytes_written << std::endl;

    if (bytes_written == -1) {
      // default linux pipe buffer reached
      perror("write");
      std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    }

    pthread_mutex_unlock(&processing_input_mutex);
    while (processing_input) {
      if (bytes_written == 0) {
        break;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    
  }
 
}
