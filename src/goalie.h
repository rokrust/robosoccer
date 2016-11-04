#ifndef GOALIE_H
#define GOALIE_H

#include "robot.h"

class Goalie : public Robot
{
private:



public:
    Goalie(RTDBConn DBC_in, int device_nr_in);

};

#endif // GOALIE_H
