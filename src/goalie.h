#ifndef GOALIE_H
#define GOALIE_H

#include "robot.h"

class Goalie : public Robot
{
private:



public:
    Goalie(RTDBConn DBC_in, int device_nr_in, int robot_array_index, Position pos);

    void do_a_penalty_save();

    void do_the_goalkeepers_kick();

};

#endif // GOALIE_H
