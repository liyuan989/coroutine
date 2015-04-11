#include "Schedule.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

namespace coroutine
{

namespace detail
{

void func(uint32_t low32, uint32_t high32)
{
    uintptr_t ptr = static_cast<uintptr_t>(low32) | (static_cast<uintptr_t>(high32) << 32);
    Schedule* schedule = reinterpret_cast<Schedule*>(ptr);
    int id = schedule->getRunningCoroutineId();
    const CoroutinePtr& coroutine = schedule->getCoroutineById(id);
    if (coroutine)
    {
        coroutine->start();
    }
    coroutine->setState(coroutine::Coroutine::kDead);
    schedule->deleteCoroutineById(id);
    schedule->setRunningCoroutineId(id);
}

}  // namespace detail

const int Schedule::kStackSize;

Schedule::Schedule(int size)
    : kCapacity_(size),
      running_id_(-1),
      flags_(size, 0)
{
}

Schedule::~Schedule()
{
    assert(running_id_ == -1);
}

int Schedule::newCoroutine(const Callback& cb)
{
    if (coroutines_.size() < static_cast<size_t>(kCapacity_))
    {
        for (int i = 0; i < kCapacity_; ++i)
        {
            int id = (static_cast<int>(coroutines_.size()) + i) % kCapacity_;
            if (flags_[id] == 0)
            {
                CoroutinePtr new_coroutine(new Coroutine(cb, id));
                flags_[id] = 1;
                coroutines_[id] = new_coroutine;
                return id;
            }
        }
    }
    else
    {
        return -1;
    }
    assert(0);
    return -1;
}

void Schedule::runCoroutineById(int id)
{
    assert(running_id_ == -1);
    assert(0 <= id && id < kCapacity_);
    CoroutineMap::const_iterator it = coroutines_.find(id);
    if (it != coroutines_.end())
    {
        const CoroutinePtr& coroutine = it->second;
        if (coroutine)
        {
            Coroutine::State state = coroutine->state();
            if (state == Coroutine::kReady)
            {
                getcontext(coroutine->getContextMutable());
                coroutine->getContextMutable()->uc_stack.ss_sp = stack_;
                coroutine->getContextMutable()->uc_stack.ss_size = kStackSize;
                coroutine->getContextMutable()->uc_link = &main_context_;
                running_id_ = id;
                coroutine->setState(Coroutine::kRunning);
                uintptr_t ptr = reinterpret_cast<uintptr_t>(this);
                makecontext(coroutine->getContextMutable(),
                            reinterpret_cast<void (*)()>(detail::func),
                            2,
                            static_cast<uint32_t>(ptr),
                            static_cast<uint32_t>(ptr >> 32));
                swapcontext(&main_context_, coroutine->getContextMutable());
            }
            else if (state == Coroutine::kSuspend)
            {
                memcpy(stack_ + kStackSize - coroutine->size(), coroutine->stack(), coroutine->size());
                running_id_ = id;
                coroutine->setState(Coroutine::kRunning);
                swapcontext(&main_context_, coroutine->getContextMutable());
            }
            else
            {
                assert(0);
            }
        }
        else
        {
            assert(0);
        }
    }
}

void Schedule::suspendCurrentCoroutine()
{
    assert(running_id_ >= 0);
    CoroutineMap::const_iterator it = coroutines_.find(running_id_);
    if (it != coroutines_.end())
    {
        const CoroutinePtr& coroutine = it->second;
        if (coroutine)
        {
            Coroutine* p = coroutine.get();
            assert(reinterpret_cast<char*>(&p) > stack_);
            (void)p;
            coroutine->saveStack(stack_ + kStackSize);
            coroutine->setState(Coroutine::kSuspend);
            running_id_ = -1;
            swapcontext(coroutine->getContextMutable(), &main_context_);
        }
        else
        {
            assert(0);
        }
    }
    else
    {
        assert(0);
    }
}

Coroutine::State Schedule::getCoroutineStateById(int id) const
{
    CoroutineMap::const_iterator it = coroutines_.find(id);
    if (it != coroutines_.end())
    {
        return it->second->state();
    }
    else
    {
        return Coroutine::kInvalid;
    }
}

CoroutinePtr Schedule::getCoroutineById(int id) const
{
    CoroutineMap::const_iterator it = coroutines_.find(id);
    if (it != coroutines_.end())
    {
        return it->second;
    }
    else
    {
        return CoroutinePtr();
    }
}

void Schedule::deleteCoroutineById(int id)
{
    CoroutineMap::iterator it = coroutines_.find(id);
    if (it != coroutines_.end())
    {
        coroutines_.erase(it);
        flags_[id] = 0;
    }
}

}  // namespace coroutine
