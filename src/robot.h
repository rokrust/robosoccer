
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


enum Parameters
     {K_ph = 20, // working thursday: 20
      K_dh = 5, // working thursday: 5
      K_pt = 170, // working thursday: 170
      K_it = 100 }; // working thursday: 100

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
    int device_nr;
    int robot_array_index;

    // turn constants
    const int BASE_TURN_SPEED = 80;
    const int TURN_RAMP_UP = 0;
    const double DDEG_MULTIPLYER = 2.29;
    const int MAX_WHEELSPEED = 200;

    // goalie constants
    const int GOALIE_SPEED = 120;
    const int GOALIE_CONST_SPEED_120_FORWARD = 2200;
    const int GOALIE_CONST_SPEED_120_BACKWARD = 2150;

    // controller driving constants
    const double ACCEPTABLE_DISTANCE_THRESHOLD = 0.08;
    const double ACCEPTABLE_HEADING_THRESHOLD = 0.05;

    const double EXTRAPOL_LIMIT = 1.0;

    Controller_data controller_data;
    Path_finder path_finder;

    Position target_pos;

    void reset_integrators_if_necessary(Angle ref_heading, Angle cur_heading);
    double error_buffer_mean();

public:

    Robot(RTDBConn DBC_in, int device_nr_in, int robot_array_index, Position* robot_positions);
    ~Robot();

    int spot_turn(Angle phi_in, bool verbose=true);
    int drive_parallel(float diff_to_drive);

    // controller functions
    int update_speed_controller(Angle ref_heading, Angle cur_heading);
    int update_heading_controller(Angle ref_heading, Angle cur_heading);
    void set_wheelspeed(int timer_duration, Position* robot_positions=NULL);

    void set_sampling_time(int sampling_time);

    // target pos
    void set_target_pos(Position pos, bool extrapol=false);
    void set_robot_target_pos(Position pos, bool extrapol=false, bool shift_linear=false);
    Position get_target_pos();
    Position get_robot_target_pos();

    // misc
    int ddeg(Angle goal_phi);

protected:
    const double GOAL_MAX_YPOS = 0.13;
    const double GOAL_MIN_YPOS = -0.13;
    const double GOAL_LEFT_XPOS = -1.48;
    const double GOAL_RIGHT_XPOS =  1.48;
};

#endif // ROBOT_H
