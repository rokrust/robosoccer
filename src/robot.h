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
    /**
     * @brief Device number of the robot
     */
    int device_nr;

public:
    Robot(RTDBConn DBC_in, int device_nr_in);
    ~Robot();

    /**
     * @brief Custom driving function
     * @param pos_in Position to drive to
     */
    void drive_to_pos(Position pos_in);

    /**
     * @brief Calculate Euclidean distance between two positions
     * @param pos_a First position
     * @param pos_b Second position
     * @return Distance between the two positions in meters
     */
    double calc_dist(Position pos_a, Position pos_b);

    /**
     * @brief Turn on the spot
     * @param phi_in Angle to turn to
     * @return Estimated time required to turn in micro seconds
     */
    int spot_turn(Angle phi_in);
};

#endif // ROBOT_H
