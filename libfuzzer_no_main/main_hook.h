#pragma once

#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

pthread_t cifuzz_ntpd_get_thread_id();
void      cifuzz_ntpd_set_thread_id(pthread_t tid);
int       cifuzz_ntpd_is_running();
void      cifuzz_ntpd_set_is_running(int is_running);
void      cifuzz_ntpd_send_sighup();

#ifdef __cplusplus
}
#endif