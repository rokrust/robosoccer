#ifndef SOCCER_TESTS_H
#define SOCCER_TESTS_H

#include "robot.h"
//#include "kogmo_rtdb.hxx"
//#include "robo_control.h"

class Soccer_Tests
{
private:
    Robot* blue1;
    Robot* blue2;
    Robot* blue3;
    Robot* red1;
    Robot* red2;
    Robot* red3;

public:
    Soccer_Tests(Robot* blue1_in, Robot* blue2_in, Robot* blue3_in,
                 Robot* red1_in, Robot* red2_in, Robot* red3_in);

    void move_in_out();

    void const_wheel_speed();

    void easy_p_ctrl();

};

#endif // SOCCER_TESTS_H
