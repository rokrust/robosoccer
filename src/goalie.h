#ifndef GOALIE_H
#define GOALIE_H

#include "robot.h"

class Goalie : public Robot
{
private:



public:
    Goalie(RTDBConn DBC_in, int device_nr_in, RawBall *datBall_in);

    int go_to_penalty_save_position(bool is_left_side);

};

#endif // GOALIE_H
