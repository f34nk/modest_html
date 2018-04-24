#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <math.h>

#include <time.h>

#include <string.h>

#include "hires-timer.h"
#include "cbench.h"

bool hires_interval_lt(hires_interval *rts1, hires_interval* rts2) {
    if (rts1->secs < rts2->secs) {
        return true;
    } else if (rts1->secs == rts2->secs) {
        return (rts1->nanosecs < rts2->nanosecs);
    } else {
        return false;
    }
}

void div_hires_interval(hires_interval* rts, size_t d) {
    rts->nanosecs = ceil((((double) BILLION) / ((double) d))
                         * (double)(rts->secs % d))
        + rts->nanosecs / d;
    rts->secs /= d;
}

void average_hires_interval(hires_interval* rts, size_t nrts, hires_interval* avg) {
    double secs = 0;
    double nanosecs = 0;

    double secs_intpart;

    uint_fast32_t intavgnsecs;

    if (nrts < 1) {
        avg->secs = 0;
        avg->nanosecs = 0;
        return;
    }

    for (size_t i = 0; i < nrts; i++) {
        secs = (i * secs) / (i + 1.) + ((double)rts[i].secs) / (i + 1.);
        nanosecs = (i * nanosecs) / (i + 1.)
            + ((double)rts[i].nanosecs) / (i + 1.);
    }

    intavgnsecs = floor(modf(secs, &secs_intpart) * BILLION);

    avg->secs = floor(secs_intpart);
    avg->nanosecs = ceil(nanosecs) + intavgnsecs;

    if (avg->nanosecs >= BILLION) {
        avg->secs += 1;
        avg->nanosecs -= BILLION;
    }
}

void stddev_hires_interval(hires_interval* rts, size_t nrts, hires_interval* stddev) {
    if (nrts == 1) {
        stddev->secs = 0;
        stddev->nanosecs = 0;
        return;
    }

    hires_interval avg;
    average_hires_interval(rts, nrts, &avg);

    double stddev_nsecs = 0;

    for (size_t i = 0; i < nrts; i++) {
        double dsecs, dnsecs;
        dsecs = (double)(rts[i].secs) - (double)avg.secs;
        dsecs *= BILLION;

        dnsecs = (double)(rts[i].nanosecs) - (double)avg.nanosecs;

        stddev_nsecs += (dsecs + dnsecs) * (dsecs + dnsecs);
    }

    stddev_nsecs /= (nrts - 1);
    stddev_nsecs = sqrt(stddev_nsecs);

    stddev->secs = (size_t)floor(stddev_nsecs / BILLION);
    stddev->nanosecs = (size_t)ceil(stddev_nsecs) - stddev->secs * BILLION;
}

char* cbench_asprintf(const char *fmt, ...) {
    va_list args;
    char* buf = NULL;

    va_start(args, fmt);
    int count = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    if (count > 0) {
        buf = (char*)malloc(sizeof(buf) * (count + 1));

        va_start(args, fmt);
        vsnprintf(buf, count + 1, fmt, args);
        va_end(args);
    }
    return buf;
}

char* hires_interval_to_str(hires_interval* rt) {
    size_t s = rt->secs;
    size_t ns = rt->nanosecs;
    size_t ms = ns / 1000000;
    ns %= 1000000;
    size_t us = ns / 1000;
    ns %= 1000;

    char* buffer;

    if (s != 0) {
        buffer = cbench_asprintf("%llu.%03llu s", s, ms);
    } else if (ms != 0) {
        buffer = cbench_asprintf("%llu.%03llu ms", ms, us);
    } else if (us != 0) {
        buffer = cbench_asprintf("%llu.%03llu us", us, ns);
    } else {
        buffer = cbench_asprintf("%llu ns", ns);
    }
    return buffer;
}

void new_benchmark_results(benchmark_results *bres,
                           size_t nsamples,
                           size_t nruns,
                           char* CBENCH_BRNAME) {
    bres->rts = (hires_interval*) malloc(sizeof(*bres->rts)
                                   * nsamples);
    bres->nsamples = nsamples;
    bres->nruns = nruns;

    bres->name = strdup(CBENCH_BRNAME);
}

void free_benchmark_results(benchmark_results* bres) {
    free(bres->rts);
    free(bres->name);
}

void summarize_benchmark_results(benchmark_results* bres) {
    hires_interval avg, stddev;
    hires_interval *best, tmp;

    average_hires_interval(bres->rts, bres->nsamples, &avg);
    char* sample_mean = hires_interval_to_str(&avg);

    stddev_hires_interval(bres->rts, bres->nsamples, &stddev);
    char *sample_stddev = hires_interval_to_str(&stddev);

    div_hires_interval(&avg, bres->nruns);
    char *run_mean = hires_interval_to_str(&avg);

    div_hires_interval(&stddev, bres->nruns);
    char *run_stddev = hires_interval_to_str(&stddev);

    best = &bres->rts[0];
    for (size_t i = 1; i < bres->nsamples; i++)
        if (hires_interval_lt(&bres->rts[i], best))
            best = &bres->rts[i];

    char *best_sample = hires_interval_to_str(best);

    tmp = *best;
    div_hires_interval(&tmp, bres->nruns);
    char *best_run = hires_interval_to_str(&tmp);

    int wc0 = 14;

    int wc1 = 6;
    if (strlen(best_sample) > wc1)
        wc1 = strlen(best_sample);
    if (strlen(sample_stddev) > wc1)
        wc1 = strlen(sample_stddev);
    if (strlen(sample_mean) > wc1)
        wc1 = strlen(sample_mean);

    int wc2 = 3;
    if (strlen(best_run) > wc2)
        wc2 = strlen(best_run);
    if (strlen(run_stddev) > wc2)
        wc2 = strlen(run_stddev);
    if (strlen(run_mean) > wc2)
        wc2 = strlen(run_mean);

    // Title
    int estrnlen = snprintf(NULL, 0,
                            ">>> Estimated runtime: %s (+ %s)",
                            best_run,
                            run_stddev);
    int titlelen = strlen(bres->name) + 6;
    if (estrnlen > titlelen)
        titlelen = estrnlen;
    for (int i = 0; i < titlelen; i++)
        printf("=");
    printf("\n>> %s <<\n", bres->name);
    for (int i = 0; i < titlelen; i++)
        printf("-");
    printf("\n>>> Estimated runtime: %s (+ %s)\n",
           best_run,
           run_stddev);
    for (int i = 0; i < titlelen; i++)
        printf("-");
    printf("\n>>> Details\n\n%llu samples of %llu runs\n",
           bres->nsamples,
           bres->nruns);

    int twidth = 15 + (3 + wc1) + (3 + wc2);
    for (int i = 0; i < twidth - 1; i++)
        printf("_");
    printf("\n              | sample");
    for (int i = 0; i < wc1 - 6; i++)
        printf(" ");
    printf(" | run");
    for (int i = 0; i < wc2 - 3; i++)
        printf(" ");
    printf(" |\n");

    char *divider = malloc(twidth + 2);
    for (int i = 0; i < twidth; i++)
        divider[i] = '-';
    divider[wc0] = '+';
    divider[wc0 + wc1 + 3] = '+';
    divider[twidth - 1] = '|';
    divider[twidth] = '\n';
    divider[twidth + 1] = '\0';

    printf(divider);

    printf("Fastest mean  | ");
    for (int i = 0; i < wc1 - strlen(best_sample); i++)
        printf(" ");
    printf("%s | ", best_sample);
    for (int i = 0; i < wc2 - strlen(best_run); i++)
        printf(" ");
    printf("%s |\n", best_run);

    printf(divider);

    printf("Sample mean   | ");
    for (int i = 0; i < wc1 - strlen(sample_mean); i++)
        printf(" ");
    printf("%s | ", sample_mean);
    for (int i = 0; i < wc2 - strlen(run_mean); i++)
        printf(" ");
    printf("%s |\n", run_mean);

    printf(divider);

    printf("Sample stddev | ");
    for (int i = 0; i < wc1 - strlen(sample_stddev); i++)
        printf(" ");
    printf("%s | ", sample_stddev);
    for (int i = 0; i < wc2 - strlen(run_stddev); i++)
        printf(" ");
    printf("%s |\n", run_stddev);

    printf("\n");

    free(sample_mean);
    free(sample_stddev);
    free(run_mean);
    free(run_stddev);
    free(best_sample);
    free(best_run);
    free(divider);
}

benchmark_results do_bench_results(CBENCH_F_T cbench_f,
                                   size_t nsamples,
                                   size_t nruns,
                                   CBENCH_ARGS_T args) {
    benchmark_results bres;
    (*cbench_f)(nsamples, nruns, &bres, args);

    return bres;
}

void do_bench_print_summary(CBENCH_F_T cbench_f,
                            size_t nsamples,
                            size_t nruns,
                            CBENCH_ARGS_T args) {
    benchmark_results bres = do_bench_results(cbench_f,
                                              nsamples,
                                              nruns,
                                              args);
    summarize_benchmark_results(&bres);
    free_benchmark_results(&bres);
}
