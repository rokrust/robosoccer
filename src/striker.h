#ifndef STRIKER_H
#define STRIKER_H

#include "robot.h"

#include <time.h>
#include <stdlib.h>


class Striker : public Robot
{
private:


public:
    Striker(RTDBConn DBC_in, int device_nr_in, int robot_array_index, Position pos);

    int shoot_penalty();

    void shoot_ball_at_goal(bool is_left_side);
};

#endif // STRIKER_H
