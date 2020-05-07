#include <pthread.h>
#include "time_isel.h"
#include <sys/time.h>
#include <string.h>

#define MAXTASKS 20

void
delay_until (struct timeval* next_activation)
{
  struct timeval now, timeout;
  gettimeofday (&now, NULL);
  timeval_sub (&timeout, next_activation, &now);
  select (0, NULL, NULL, NULL, &timeout);
}

void
timeval_sub (struct timeval *res, struct timeval *a, struct timeval *b)
{
  res->tv_sec = a->tv_sec - b->tv_sec;
  res->tv_usec = a->tv_usec - b->tv_usec;
  if (res->tv_usec < 0) {
    --res->tv_sec;
    res->tv_usec += 1000000;
  }
}

void
timeval_add (struct timeval *res, struct timeval *a, struct timeval *b)
{
  res->tv_sec = a->tv_sec + b->tv_sec
    + a->tv_usec / 1000000 + b->tv_usec / 1000000;
  res->tv_usec = a->tv_usec % 1000000 + b->tv_usec % 1000000;
}

int
timeval_less (struct timeval *a, struct timeval *b)
{
  return (a->tv_sec < b->tv_sec) ||
    ((a->tv_sec == b->tv_sec) && (a->tv_usec < b->tv_usec));
}

int
timeval_get_ms (struct timeval *a)
{
  return a->tv_sec * 1000 + a->tv_usec / 1000;
}
