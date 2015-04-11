#include "Coroutine.h"
#include "Schedule.h"

#include <assert.h>

namespace coroutine
{

Coroutine::Coroutine(const Callback& callback, int id_arg)
    : callback_(callback),
      id_(id_arg),
      state_(kReady),
      stack_(NULL),
      capacity_(0),
      size_(0)
{
}


Coroutine::~Coroutine()
{
    assert(state_ == kDead);
}

void Coroutine::start()
{
    if (callback_)
    {
        setState(kRunning);
        callback_();
    }
}

void Coroutine::saveStack(char* top)
{
    char dummy = 0;
    assert(top - &dummy <= static_cast<ptrdiff_t>(Schedule::kStackSize));
    if (capacity_ < top - &dummy)
    {
        ::free(stack_);
        capacity_ = top - &dummy;
        stack_ = static_cast<char*>(::malloc(capacity_));
    }
    size_ = top - &dummy;
    memcpy(stack_, &dummy, size_);
}

}  // namespace coroutine
