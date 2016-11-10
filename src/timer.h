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
    int utimeout_time;

public:
    Timer(){;}

    //translate from micro to nano
    Timer(int time):utimeout_time(time*1000){}


    void enable(){clock_gettime( CLOCK_MONOTONIC, &start_time); }

    void set_timestamp(int timestamp){ utimeout_time = timestamp*1000; }

    bool timeout();
};
#endif // TIMER_H
