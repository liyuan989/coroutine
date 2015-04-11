// This is a public header file.

#ifndef __COROUTINE_SCHEDULE_H__
#define __COROUTINE_SCHEDULE_H__

#include "Noncopyable.h"
#include "Coroutine.h"

#include <ucontext.h>

#include <boost/shared_ptr.hpp>

#include <map>
#include <vector>
#include <stdint.h>

namespace coroutine
{

class Schedule : Noncopyable
{
public:
    static const int kStackSize = 1024 * 1024;

    typedef Coroutine::Callback Callback;

    Schedule(int size = 16);
    ~Schedule();

    // return new coroutine's id.
    int newCoroutine(const Callback& cb);
    void runCoroutineById(int id);
    void suspendCurrentCoroutine();
    Coroutine::State getCoroutineStateById(int id) const;

    int getRunningCoroutineId() const
    {
        return running_id_;
    }

private:
    friend void func(uint32_t low32, uint32_t high32);

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
