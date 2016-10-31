#ifndef SOCCER_TESTS_H
#define SOCCER_TESTS_H

#include "kogmo_rtdb.hxx"
#include "robo_control.h"

class Soccer_Tests
{
private:
    RoboControl* blue1;
    RoboControl* blue2;
    RoboControl* blue3;
    RoboControl* red1;
    RoboControl* red2;
    RoboControl* red3;

public:
    Soccer_Tests(RoboControl* blue1_in, RoboControl* blue2_in, RoboControl* blue3_in,
                 RoboControl* red1_in, RoboControl* red2_in, RoboControl* red3_in);

    void move_in_out();
};

#endif // SOCCER_TESTS_H
