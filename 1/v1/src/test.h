// Author : huziang
// This is a file include time function to test time

#ifndef TEST_H_
#define TEST_H_

#include <sys/time.h>

struct timeval begin;

void initTime() {
    gettimeofday( &begin, NULL );
}

long getDiffTime() {
    struct timeval now;
    gettimeofday( &now, NULL );
    return 1000000 * ( now.tv_sec - begin.tv_sec ) + now.tv_usec - begin.tv_usec;
}

#endif  // TEST_H_