
#include <flibc.h>
#include <unistd.h>
#include <stdio.h>

#include "ntp.h"
 
extern "C" uint8_t processing_input;
 
FUZZ_UDP_SERVER(original_main, 123, int client_fd, const uint8_t* data, size_t size) {
 
 // TODO: ensure to sent max NTP_MSGSIZE bytes
    if (size < NTP_MSGSIZE) {
        processing_input = 1;
        int written = write(client_fd, data, size);
        while (processing_input) {
        usleep(10);
        }
    }
}