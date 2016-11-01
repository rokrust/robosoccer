//============================================================================
// Name        : robot.h
// Author      :
// Version     :
// Copyright   : (C)
// Description : Header of the Robot class
//============================================================================

#ifndef ROBOT_H
#define ROBOT_H

#include "kogmo_rtdb.hxx"
#include "robo_control.h"

class Robot : public RoboControl
{
private:
    int device_nr;

public:
    Robot(RTDBConn DBC_in, int device_nr_in);
    ~Robot();


    void drive_to_pos(Position pos_in);


    double calc_dist(Position pos_a, Position pos_b);


    int spot_turn(Angle phi_in);
};

#endif // ROBOT_H
