#ifndef __COROUTINE_NONCOPYABLE_H__
#define __COROUTINE_NONCOPYABLE_H__

namespace coroutine
{

// Empty base class emphasis the objects of derived class are non-copyable.
class Noncopyable
{
protected:
    Noncopyable()
    {
    }

    ~Noncopyable()
    {
    }

private:
    Noncopyable(const Noncopyable&);
    Noncopyable& operator=(const Noncopyable&);
};

}  // namespace coroutine

#endif
