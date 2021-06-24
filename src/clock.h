#ifndef __CLOCK_H_
#define __CLOCK_H_

#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define CSTART                                           \
    struct timeval tval_before, tval_after, tval_result; \
    gettimeofday(&tval_before, NULL);

#define CSTOP                                          \
    gettimeofday(&tval_after, NULL);                   \
    timersub(&tval_after, &tval_before, &tval_result); \
    printf("Time elapsed: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);

#endif
