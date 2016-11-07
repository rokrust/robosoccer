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
    RawBall* datBall;

    Robot(RTDBConn DBC_in, int device_nr_in, RawBall* datBall_in);
    ~Robot();

    void drive_precise(Position pos_in, bool verbose);

    /**
     * @brief Custom driving function
     * @param pos_in Position to drive to
     */
    void drive_to_pos(Position pos_in, bool verbose);

    /**
     * @brief drive_parallel
     * @param pos_in
     * @param verbose
     */
    void drive_parallel(float diff_to_drive, bool verbose);

    /**
     * @brief calc_ddeg
     * @param goal_phi
     * @return
     */
    int calc_ddeg(Angle goal_phi);

    /**
     * @brief Calculate Euclidean distance between two positions
     * @param pos_a First position
     * @param pos_b Second position
     * @return Distance between the two positions in meters
     */
    double calc_dist(Position pos_a, Position pos_b);

    /**
     * @brief Calculate the difference between two position (e.g. the direction of a movement)
     * @param pos_a First position
     * @param pos_b Second position
     * @return Difference between the two positions in meters
     */
    Position calc_pos_diff(Position pos_a, Position pos_b);

    /**
     * @brief Turn on the spot
     * @param phi_in Angle to turn to
     * @return Estimated time required to turn in micro seconds
     */
    int spot_turn(Angle phi_in);

    int spot_turn_time_speed(int turn_time, int wheel_speed, bool left_negativ);
};

#endif // ROBOT_H
