
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
#include "timer.h"
#include "pathfinder.h"

#include <math.h>
#include <boost/circular_buffer.hpp>

#define MAX_WHEELSPEED 200

enum Parameters
     {K_ph = 20,
      K_ih = 25,
      K_dh = 5,
      K_pt = 170, // old: 170
      K_it = 100 }; // old: 100

struct Controller_data{
    //General
    double sampling_time;

    //Speed controller
    double speed_integrator;

    //Heading controller
    double heading_integrator;
    int buffer_size;
    int current_sample;
    double* error_buffer;
};


class Robot : public RoboControl
{
private:
    /**
     * @brief Device number of the robot
     */
    int device_nr;

    //These can probably be removed
    int left_wheel_speed;
    int right_wheel_speed;

    Controller_data controller_data;
    Path_finder path_finder;

    void reset_integrators_if_necessary(Angle ref_heading, Angle cur_heading);
    double error_buffer_mean();

public:
    void set_target_pos(Position pos){path_finder.set_target_pos(pos);}
    Robot(RTDBConn DBC_in, int device_nr_in, int robot_array_index, Position pos);
    ~Robot();

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
    int drive_parallel(float diff_to_drive, bool verbose);

    /**
     * @brief calc_ddeg
     * @param goal_phi
     * @return
     */
    int calc_ddeg(Angle goal_phi);

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

    void test_loop_drive_parallel();

    int update_speed_controller(Angle ref_heading, Angle cur_heading);

    int update_heading_controller(Angle ref_heading, Angle cur_heading);

    void set_wheelspeed(int timer_duration);

    Path_finder get_path_finder(){return path_finder;}

};

#endif // ROBOT_H
