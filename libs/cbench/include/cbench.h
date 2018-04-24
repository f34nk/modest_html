#ifndef CBENCH_H
#define CBENCH_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "hires-timer.h"

#ifdef _WIN32

#define CBENCH_SIGNATURE static void
#define NO_OPTIMIZE

#else

#define CBENCH_SIGNATURE static inline void
// #define NO_OPTIMIZE asm("")
#define NO_OPTIMIZE

#endif

typedef struct benchmark_results {
    hires_interval* rts;

    size_t nsamples;
    size_t nruns;

    char* name;
} benchmark_results;

typedef void* CBENCH_ARGS_T;
typedef void (*CBENCH_F_T)(size_t,
                           size_t,
                           benchmark_results*,
                           CBENCH_ARGS_T);

/*
 * Usage:
 *
 * CBENCH_DEF(BenchName) {
 *     { setup code... }
 *     CBENCH_INIT(BenchmarkLabel) {
 *         { per-benchmark setup... }
 *         CBENCH_MEASURE_START
 *             { measured code }
 *         CBENCH_MEASURE_STOP
 *         { per-benchmark cleaup... }
 *     }
 *     { teardown code... }
 * }
 */

#define CBENCH_CALL(bname) CBENCH_##bname
#define CBENCH_DEF(bname) CBENCH_SIGNATURE CBENCH_CALL(bname)   \
    (size_t CBENCH_nsamples,                                    \
     size_t CBENCH_nruns,                                       \
     benchmark_results* CBENCH_results,                         \
     CBENCH_ARGS_T CBENCH_args)

#define CBENCH_INIT(CBENCH_name_fmt, ...)                       \
    char* CBENCH_BRNAME;                                        \
    HIRES_TIME CBENCH_start, CBENCH_end;                       \
    size_t CBENCH_sample_count = 0;                             \
    int CBENCH_run_count = 0;                                   \
    size_t CBENCH_BRNAME_LEN =                                  \
        snprintf(NULL,                                          \
                 0,                                             \
                 CBENCH_name_fmt, ##__VA_ARGS__);               \
    INIT_TIMER();                                               \
    CBENCH_BRNAME = (char*)malloc(CBENCH_BRNAME_LEN + 1);       \
    snprintf(CBENCH_BRNAME,                                     \
             CBENCH_BRNAME_LEN + 1,                             \
             CBENCH_name_fmt, ##__VA_ARGS__);                   \
    new_benchmark_results(CBENCH_results,                       \
                          CBENCH_nsamples,                      \
                          CBENCH_nruns,                         \
                          CBENCH_BRNAME);                       \
    free(CBENCH_BRNAME);                                        \
    for (CBENCH_sample_count = 0;                               \
         CBENCH_sample_count < CBENCH_nsamples;                 \
         CBENCH_sample_count++)                                 \

#define CBENCH_MEASURE_START GET_HIRES_TIME(&CBENCH_start);     \
    for (CBENCH_run_count = 0;                                  \
         CBENCH_run_count < CBENCH_nruns;                       \
         CBENCH_run_count++) {                                  \
        NO_OPTIMIZE; // Prevent code optimization

#define CBENCH_MEASURE_STOP  }                                  \
    GET_HIRES_TIME(&CBENCH_end);                                \
    hires_time_diff(&CBENCH_end,                                \
                    &CBENCH_start,                              \
                    &CBENCH_results->rts[CBENCH_sample_count]);

void new_benchmark_results(benchmark_results *bres,
                           size_t nsamples,
                           size_t nruns,
                           char* CBENCH_BRNAME);
void free_benchmark_results(benchmark_results* bres);

void average_hires_interval(hires_interval* rts,
                            size_t nrts,
                            hires_interval* avg);
void stddev_hires_interval(hires_interval* rts,
                           size_t nrts,
                           hires_interval* stddev);

char* hires_interval_to_str(hires_interval* rt);

benchmark_results do_bench_results(CBENCH_F_T cbench_f,
                                   size_t nsamples,
                                   size_t nruns,
                                   CBENCH_ARGS_T args);

void summarize_benchmark_results(benchmark_results* bres);

void do_bench_print_summary(CBENCH_F_T cbench_f,
                            size_t nsamples,
                            size_t nruns,
                            CBENCH_ARGS_T args);

#endif /* CBENCH_H */
