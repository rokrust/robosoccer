#ifndef OPPONENT_H
#define OPPONENT_H

#include "robot.h"

class Opponent : public Robot
{
private:



public:
    Opponent(RTDBConn DBC_in, int device_nr_in, int robot_array_index, Position target_pos = Position(0,0));
};

#endif // OPPONENT_H
