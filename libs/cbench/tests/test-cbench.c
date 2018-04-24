#include <stdbool.h>

#include "hires-timer.h"
#include "cbench.h"

bool hires_interval_gteq(hires_interval* a, hires_interval* b) {
    if (a->secs > b->secs) {
        return true;
    } else if (a->secs == b->secs) {
        return a->nanosecs >= b->nanosecs;
    } else {
        return false;
    }
}

void slow_function(hires_interval* ivl) {
    HIRES_TIME t_start, t_cur;
    hires_interval cur_ivl;
    GET_HIRES_TIME(&t_start);
    while (1) {
        GET_HIRES_TIME(&t_cur);
        hires_time_diff(&t_cur, &t_start, &cur_ivl);
        if (hires_interval_gteq(&cur_ivl, ivl)) {
            break;
        }
    }
}

CBENCH_DEF(bench_sleep) {
    int i = *(int*)CBENCH_args;

    hires_interval ivl;
    ivl.secs = 0;
    ivl.nanosecs = i;

    CBENCH_INIT("Running for %i ns", i) {
        CBENCH_MEASURE_START
        slow_function(&ivl);
        CBENCH_MEASURE_STOP
    }
}

int main(int argc, char *argv[])
{
    int cnt = 7000000;
    INIT_TIMER();
    do_bench_print_summary(CBENCH_bench_sleep, 3, 100, &cnt);

    return 0;
}
