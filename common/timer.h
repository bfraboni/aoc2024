#pragma once

#include <chrono>

struct Timer
{
    typedef std::chrono::high_resolution_clock clock;
    typedef std::chrono::duration<int, std::nano> nano_t;
    typedef std::chrono::duration<int, std::micro> micro_t;
    typedef std::chrono::duration<int, std::milli> milli_t;
    typedef std::chrono::duration<int, std::ratio<1>> sec_t;
    typedef std::chrono::duration<int, std::ratio<60>> min_t;

    clock::time_point start;

    explicit Timer(bool run = true) {if (run) reset();}
    void reset() { start = clock::now(); }

    nano_t     nano()  const { return std::chrono::duration_cast<nano_t >(clock::now() - start); }
    micro_t    micro() const { return std::chrono::duration_cast<micro_t>(clock::now() - start); }
    milli_t    milli() const { return std::chrono::duration_cast<milli_t>(clock::now() - start); }
    sec_t      sec()   const { return std::chrono::duration_cast<sec_t  >(clock::now() - start); }
    min_t      min()   const { return std::chrono::duration_cast<min_t  >(clock::now() - start); }
};