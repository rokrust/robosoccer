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

    void (*timeout_action)();

public:
    Timer(){;}

    //translate from micro to nano
    Timer(int time, void (*action)() = NULL): utimeout_time(time*1000), timeout_action(action){}


    void enable(){ clock_gettime( CLOCK_MONOTONIC, &start_time); }

    int get_timeout_time(){ return utimeout_time; }

    void set_timeout_time(int timeout_time){ utimeout_time = timeout_time*1000; }

    bool timeout();
};
#endif // TIMER_H
