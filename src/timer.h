#ifndef TIMER_H
#define TIMER_H

#include <sys/time.h>
#include <time.h>
#include <iostream>

using namespace std;

/**
 * @brief The Timer class
 */

class Timer{
private:
    struct timeval current_time;
    long long start_time_ms, current_time_ms, end_time_ms;
    int timeout_duration_ms;

    // Converts the given timeval to the ms passed since some date back in 1970
    long long time_in_ms(timeval time_to_conv) {
        return (long long) time_to_conv.tv_sec * 1000L + time_to_conv.tv_usec / 1000;
    }

    void print_time_ms_readable(long long ms_to_print)
    {
        long long ms2p = ms_to_print; // is shorter

        int ms = ms2p%1000;
        int s = (ms2p/1000)%60;
        int m = (ms2p/(1000*60))%60;
        int h = (ms2p/(1000*60*60))%24;

        cout << " ";
        if(h<10)
            cout << "0" << h << ":";
        else
            cout << h << ":";

        if(m<10)
            cout << "0" << m << ":";
        else
            cout << m << ":";

        if (s<10)
            cout << "0" << s << ",";
        else
            cout << s << ",";

        if (ms < 10)
            cout << "00" << ms << " ";
        else if (ms < 100 && ms >= 10)
            cout << "0" << ms << " ";
        else
            cout << ms << " ";
    }

    // timespec start_time, current_time; // , temp;
    // void (*timeout_action)();

public:
    Timer(){;}

    ~Timer(){;}

    Timer(int timer_duration)
    {
        timeout_duration_ms = timer_duration;
        enable();

        if (0) // set 1 if you want to print,
        {
            cout << "Timer initialised: ";
            cout << "st and ct is "; print_time_ms_readable(start_time_ms); //cout << endl;
            cout << "; et is "; print_time_ms_readable(end_time_ms); cout << endl;
        }
    }

    void enable() {
        gettimeofday(&current_time, NULL);
        current_time_ms = time_in_ms(current_time);

        start_time_ms = current_time_ms;
        end_time_ms = current_time_ms + timeout_duration_ms;

        if (1) // set 1 if you want to print,
        {
            cout << "Timer enabled: ";
            cout << "st and ct is "; print_time_ms_readable(start_time_ms); // cout << endl;
            cout << "; et is "; print_time_ms_readable(end_time_ms); cout << endl;
        }
    }

    bool timeout(){
        gettimeofday(&current_time, NULL);
        current_time_ms = time_in_ms(current_time);

        if (current_time_ms >= end_time_ms) {
            enable();
            return true;
        } else {
            return false;
        }

    }



    //translate from micro to nano
    // Timer(int time, void (*action)() = NULL): utimeout_time(time*1000), timeout_action(action){}

    /*    //The case where the current_time is on another second than start_time
        if (current_time.tv_nsec - start_time_ms.tv_nsec < 0) {
            return true;
        }

        else {
            return current_time.tv_nsec - start_time_ms.tv_nsec >= utimeout_time;
        }
    }

    int get_timeout_time()
    {
        return utimeout_time;
    }

    void set_timeout_time(int timeout_time) {
        utimeout_time = timeout_time*1000;
    } */


};
#endif // TIMER_H
