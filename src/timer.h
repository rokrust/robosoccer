#ifndef TIMER_H
#define TIMER_H

#include <sys/time.h>
#include <time.h>
#include <iostream>


class Timer{
private:
    unsigned int start_time, timer_duration_ms;


public:

    ~Timer() {;}

    Timer(int timer_duration_ms = 0): timer_duration_ms(timer_duration_ms){}

    void enable(){ start_time = clock(); }

    bool timeout(){

        clock_t difference = clock() - start_time;
        unsigned int msec = difference * 1000 / CLOCKS_PER_SEC;


        return msec > this->timer_duration_ms;
    }


    void set_timer_duration_ms(int timer_duration_ms){ this->timer_duration_ms = timer_duration_ms; }
    int get_timer_duration_ms(){ return timer_duration_ms; }

};
#endif // TIMER_H
