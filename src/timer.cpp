#include "timer.h"
#include <iostream>

bool Timer::timeout(){
    clock_gettime(CLOCK_MONOTONIC, &current_time);

    //The case where the current_time is on another second than start_time
    if(current_time.tv_nsec - start_time.tv_nsec < 0){
        return true;
    }

    else{
        return current_time.tv_nsec - start_time.tv_nsec >= utimestamp;
    }
}
