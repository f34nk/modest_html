#ifndef HIRES_TIMER_H
#define HIRES_TIMER_H

#include <stddef.h>
#include <stdint.h>
#include <math.h>

#define BILLION 1000000000UL

typedef struct hires_interval {
    size_t secs;
    uint_fast32_t nanosecs;
} hires_interval;

#ifdef _WIN32

#include <windows.h>

LARGE_INTEGER WIN_PFREQ; //  Ticks per second

#define HIRES_TIME LARGE_INTEGER
#define INIT_TIMER() QueryPerformanceFrequency(&WIN_PFREQ)

#define GET_HIRES_TIME(o) QueryPerformanceCounter(o)

static void hires_time_diff(HIRES_TIME* a,
                            HIRES_TIME* b,
                            hires_interval* iv) {
    LARGE_INTEGER a_secs, b_secs, a_nsecs, b_nsecs;
    a_secs.QuadPart = a->QuadPart / WIN_PFREQ.QuadPart;
    b_secs.QuadPart = b->QuadPart / WIN_PFREQ.QuadPart;

    a_nsecs.QuadPart = ceil((double)(a->QuadPart % WIN_PFREQ.QuadPart)
                            * (((double) BILLION)
                               / ((double)WIN_PFREQ.QuadPart)));

    b_nsecs.QuadPart = ceil((double)(b->QuadPart % WIN_PFREQ.QuadPart)
                            * (((double) BILLION)
                               / ((double)WIN_PFREQ.QuadPart)));

    iv->secs = a_secs.QuadPart - b_secs.QuadPart;
    if (a_nsecs.QuadPart < b_nsecs.QuadPart) {
        iv->secs -= 1;
        iv->nanosecs = (BILLION - b_nsecs.QuadPart) + a_nsecs.QuadPart;
    } else {
        iv->nanosecs = a_nsecs.QuadPart - b_nsecs.QuadPart;
    }
}

#else

#include <time.h>

#define HIRES_TIME struct timespec
#define INIT_TIMER()

#define GET_HIRES_TIME(o) clock_gettime(CLOCK_PROCESS_CPUTIME_ID, (o))

static void hires_time_diff(HIRES_TIME* a,
                            HIRES_TIME* b,
                            hires_interval* iv) {
    iv->secs = a->tv_sec - b->tv_sec;
    if (a->tv_nsec < b->tv_nsec) {
        iv->secs -= 1;
        iv->nanosecs = (BILLION - b->tv_nsec) + a->tv_nsec;
    } else {
        iv->nanosecs = a->tv_nsec - b->tv_nsec;
    }
}

#endif

#endif /* HIRES-TIMER_H */
