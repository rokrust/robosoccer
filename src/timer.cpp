#include "timer.h"

bool Timer::timeout(){
    clock_gettime(CLOCK_MONOTONIC, &current_time);

    return current_time.tv_sec - start_time.tv_sec >= timestamp;
}
