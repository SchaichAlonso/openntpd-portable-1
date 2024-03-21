#include <sys/resource.h>
#include <sys/time.h>
#include <sys/timex.h>

#include <stdio.h>

static void setup() __attribute__ ((constructor));
static void cleanup() __attribute__ ((destructor));

void setup()
{
    printf("Injecting stubs.\n");
}

void cleanup()
{
    printf("Done.\n");
}

int adjtimex (struct timex *__ntx)
{
  (void)__ntx;

  return 0;
}

int adjtime(const struct timeval *delta, struct timeval *olddelta)
{
    (void)delta;
    (void)olddelta;

    return 0;
}

int setpriority(__priority_which_t which, id_t who, int prio)
{
  (void)which;
  (void)who;
  (void)prio;

  return 0;
}