#ifndef STRIKER_H
#define STRIKER_H

#include "robot.h"

#include <time.h>
#include <stdlib.h>


class Striker : public Robot
{
private:


public:
    Striker(RTDBConn DBC_in, int device_nr_in, int robot_array_index);

    void do_a_shot_at_goal(RawBall *datBall, bool is_left_side);
};

#endif // STRIKER_H
