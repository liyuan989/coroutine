#include "Schedule.h"

#include <boost/bind.hpp>

#include <stdio.h>

using namespace coroutine;

void func(Schedule* schedule, int base)
{
    for (int i = 0; i < 5; ++i)
    {
        printf("running id = %d, num = %d\n", schedule->getRunningCoroutineId(), base + i);
        schedule->suspendCurrentCoroutine();
    }
}

int main(int argc, char* argv[])
{
    Schedule schedule;
    int coroutine1 = schedule.newCoroutine(boost::bind(func, &schedule, 1));
    int coroutine2 = schedule.newCoroutine(boost::bind(func, &schedule, 1000));
    for (int i = 0; i < 5; ++i)
    {
        schedule.runCoroutineById(coroutine1);
        schedule.runCoroutineById(coroutine2);
    }
    return 0;
}
