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
    int ID;

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
        cout << h << ":";

        if(m<10)
            cout << "0";
        cout << m << ":";

        if (s<10)
            cout << "0";
        cout << s << ",";

        if (ms < 100)
            cout << "0";
        if (ms < 10)
            cout << "0";
        cout << ms << " ";
    }

    // timespec start_time, current_time; // , temp;
    // void (*timeout_action)();

    // TODO: Set endtime

public:
    Timer(){;}

    ~Timer(){;}

    Timer(int timer_duration, int ID_in=0)
    {
        ID = ID_in;
        timeout_duration_ms = timer_duration;
        enable_periodically();

        if (0) // set 1 if you want to print,
        {
            cout << "Initialised timer " << ID << " ";
            cout << "st and ct is "; print_time_ms_readable(start_time_ms); //cout << endl;
            cout << "; et is "; print_time_ms_readable(end_time_ms); cout << endl;
        }
    }

    void enable_periodically() {
        gettimeofday(&current_time, NULL);
        current_time_ms = time_in_ms(current_time);

        start_time_ms = current_time_ms;
        end_time_ms = current_time_ms + timeout_duration_ms;

        if (0) // set 1 if you want to print,
        {
            cout << "Enabled timer " << ID << " ";
            cout << "st and ct is "; print_time_ms_readable(start_time_ms); // cout << endl;
            cout << "; et is "; print_time_ms_readable(end_time_ms); cout << endl;
        }
    }

    void enable_manually(const int manual_duration_ms) {
        gettimeofday(&current_time, NULL);
        current_time_ms = time_in_ms(current_time);

        start_time_ms = current_time_ms;
        end_time_ms = current_time_ms + manual_duration_ms;

        if (0) // set 1 if you want to print,
        {
            cout << "Manual enabled timer " << ID << " ";
            cout << "st and ct is "; print_time_ms_readable(start_time_ms); // cout << endl;
            cout << "; et is "; print_time_ms_readable(end_time_ms); cout << endl;
        }
    }

    bool timeout(){
        gettimeofday(&current_time, NULL);
        current_time_ms = time_in_ms(current_time);

        if (current_time_ms >= end_time_ms) {
            enable_periodically();
            return true;
        } else {
            return false;
        }

    }
};
#endif // TIMER_H
