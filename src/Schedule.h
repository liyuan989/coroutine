// This is a public header file.

#ifndef __COROUTINE_SCHEDULE_H__
#define __COROUTINE_SCHEDULE_H__

#include "Noncopyable.h"
#include "Coroutine.h"

#include <ucontext.h>

#include <map>
#include <vector>
#include <stdint.h>

namespace coroutine
{

class Schedule : Noncopyable
{
public:
    static const int kStackSize = 1024 * 1024;

    typedef boost::function<void ()> CoroutineFunc;

    Schedule(int size = 16);
    ~Schedule();

    // Return new coroutine's id.
    // If the schedule is full, return -1.
    int newCoroutine(const CoroutineFunc& func);
    void runCoroutineById(int id);
    void suspendCurrentCoroutine();
    Coroutine::State getCoroutineStateById(int id) const;

    // If there is no coroutine running, return -1.
    int getRunningCoroutineId() const
    {
        return running_id_;
    }

private:
    friend void coroutineFunc(uint32_t low32, uint32_t high32);

    CoroutinePtr getCoroutineById(int id) const;
    void deleteCoroutineById(int id);

    void setRunningCoroutineId(int id)
    {
        running_id_ = id;
    }

    typedef std::map<int, CoroutinePtr> CoroutineMap;

    const int         kCapacity_;
    int               running_id_;
    ucontext_t        main_context_;
    char              stack_[kStackSize];
    CoroutineMap      coroutines_;
    std::vector<int>  flags_;
};

}  // namespace coroutine

#endif
