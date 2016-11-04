#include "timer.h"

bool Timer::timeout(){
    clock_gettime(CLOCK_MONOTONIC, &current_time);

    return current_time.tv_nsec - start_time.tv_nsec >= timestamp;
}
