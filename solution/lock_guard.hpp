#pragma once

#include "../mutex.h"

class lock_guard
{
    mutex* m = nullptr;

public:
    explicit lock_guard(mutex& m) : m{&m}
    {
        m.lock();
    }

    ~lock_guard()
    {
        if (m) // Ensure we do not delete an empty lock guard
        {
            m->unlock();
        }
    }

    lock_guard(const lock_guard&) = delete;

    lock_guard& operator=(const lock_guard&) = delete;

    lock_guard(lock_guard&& other) noexcept : m{other.m}
    {
        // Make sure to leave the other lock guard in a valid but empty state
        other.m = nullptr;
    }

    lock_guard& operator=(lock_guard&& other) noexcept
    {
        if (this != &other)
        {
            // Steal the pointer from the other lock guard
            m = other.m;
            // Make sure to leave the other lock guard in a valid but empty state
            other.m = nullptr;
        }
        return *this;
    }
};
