#ifndef STRIKER_H
#define STRIKER_H

#include "robot.h"
#include "game.h"

#include <time.h>
#include <stdlib.h>

class Striker : public Robot
{
private:


public:
    Striker(RTDBConn DBC_in, int device_nr_in, RawBall *datBall_in);

    void shoot_ball_at_goal(bool is_left_side);
    int shoot_penalty();
};

#endif // STRIKER_H
