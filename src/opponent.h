#ifndef OPPONENT_H
#define OPPONENT_H

#include "robot.h"

class Opponent : public Robot
{
private:



public:
    Opponent(RTDBConn DBC_in, int device_nr_in, int robot_array_index, Position* robot_positions);
};

#endif // OPPONENT_H
