// This is an internal header file, you should not include this.

#ifndef __COROUTINE_COROUTINE_H__
#define __COROUTINE_COROUTINE_H__

#include "noncopyable.h"

#include <stddef.h>
#include <functional>
#include <memory>

#include <ucontext.h>


namespace coroutine {

class Schedule;

class Coroutine : Noncopyable {
public:
    enum State {
        kInvalid,
        kDead,
        kReady,
        kRunning,
        kSuspend,
    };

    typedef std::function<void (Schedule*)> CoroutineFunc;

    Coroutine(const CoroutineFunc& func, int id);
    ~Coroutine();

    void start(Schedule* schedule);
    void saveStack(char* top);

    int id() const { return id_; }

    State state() const { return state_; }
    void setState(State state_arg) { state_ = state_arg; }

    ucontext_t* getContextMutable() { return &context_; }
    const ucontext_t& getContext() const { return context_; }

    char* stack() const { return stack_; }

    ptrdiff_t size() const { return size_; }

    ptrdiff_t capacity() const { return capacity_; }

private:
    CoroutineFunc func_;
    int id_;
    State state_;
    ucontext_t context_;
    char* stack_;
    ptrdiff_t capacity_;
    ptrdiff_t size_;
};

typedef std::shared_ptr<Coroutine> CoroutinePtr;

}  // namespace coroutine

#endif

