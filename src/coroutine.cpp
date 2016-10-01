#include "coroutine.h"
#include "schedule.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>

namespace coroutine {

Coroutine::Coroutine(const CoroutineFunc& func, int id_arg)
    : func_(func),
      id_(id_arg),
      state_(kReady),
      stack_(NULL),
      capacity_(0),
      size_(0) {
}

Coroutine::~Coroutine() {
    assert(state_ == kDead);
}

void Coroutine::start(Schedule* schedule) {
    if (func_) {
        setState(kRunning);
        func_(schedule);
    }
}

void Coroutine::saveStack(char* top) {
    char dummy = 0;
    assert(top - &dummy <= static_cast<ptrdiff_t>(Schedule::kStackSize));
    if (capacity_ < top - &dummy) {
        ::free(stack_);
        capacity_ = top - &dummy;
        stack_ = static_cast<char*>(::malloc(static_cast<size_t>(capacity_)));
    }
    size_ = top - &dummy;
    memcpy(stack_, &dummy, static_cast<size_t>(size_));
}

}  // namespace coroutine

