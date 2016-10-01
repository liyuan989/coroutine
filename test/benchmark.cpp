#include "schedule.h"

#include <sys/time.h>

#include <stdint.h>
#include <stdio.h>

using namespace coroutine;

void func(Schedule* schedule) {
    schedule->suspendCurrentCoroutine();
}

int main(int argc, char* argv[]) {
    Schedule schedule;
    int coroutine1 = schedule.createCoroutine(func);
    int coroutine2 = schedule.createCoroutine(func);
    const int  kMicrosecondsPerSecond = 1000 * 1000;
    struct timeval tv_start;
    gettimeofday(&tv_start, NULL);
    int64_t start = static_cast<int64_t>(tv_start.tv_sec) * kMicrosecondsPerSecond + tv_start.tv_usec;
    for (int i = 0; i < 2000 * 10000; ++i) {
        schedule.runCoroutineById(coroutine1);
        schedule.runCoroutineById(coroutine2);
    }
    struct timeval tv_end;
    gettimeofday(&tv_end, NULL);
    int64_t end = static_cast<int64_t>(tv_end.tv_sec) * kMicrosecondsPerSecond + tv_end.tv_usec;
    printf("Total time of switching 40M times: %fs\n",
           static_cast<double>(end - start) / kMicrosecondsPerSecond);
    return 0;
}

