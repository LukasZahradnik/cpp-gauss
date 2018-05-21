#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer
{
private:
    const char* header;
    std::chrono::time_point<std::chrono::high_resolution_clock> _beg;
    typedef std::chrono::high_resolution_clock _clock;

public:
    Timer(const char* header);
    ~Timer();
};

#endif // TIMER_H
