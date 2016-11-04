#ifndef TIMER_H
#define TIMER_H

#include <sys/time.h>
#include <time.h>

/**
 * @brief The Timer class
 */

class Timer{
private:
    timespec start_time, current_time, temp;
    int utimestamp;

public:
    Timer(){;}
    Timer(int time):utimestamp(time*1000){}

    void enable(){clock_gettime( CLOCK_MONOTONIC, &start_time); }

    void set_timestamp(int timestamp){ utimestamp = timestamp*1000; }

    bool timeout();
};
#endif // TIMER_H
