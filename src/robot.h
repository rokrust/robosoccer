
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
     {K_ph = 20, // working thursday: 20
      // K_ih = 25, // working thursday: 25
      K_dh = -5, // working thursday: 5
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

    // turn constants
    const int BASE_TURN_SPEED = 80;
    const int TURN_RAMP_UP = 0;
    const double DDEG_MULTIPLYER = 2.29;

    // goalie constants
    const int GOALIE_SPEED = 120;
    const int GOALIE_CONST_SPEED_120_FORWARD = 2200;
    const int GOALIE_CONST_SPEED_120_BACKWARD = 2150;

    // controller driving constants
    const double ACCEPTABLE_DISTANCE_THRESHOLD = 0.08;
    const double ACCEPTABLE_HEADING_THRESHOLD = 0.05;

    int left_wheel_speed;
    int right_wheel_speed;

    Controller_data controller_data;
    Path_finder path_finder;

    void reset_integrators_if_necessary(Angle ref_heading, Angle cur_heading);
    double error_buffer_mean();

public:
    // public constants for inheritance
    const double GOAL_MAX_YPOS = 0.13;
    const double GOAL_MIN_YPOS = -0.13;
    const double GOAL_LEFT_XPOS = -1.48;
    const double GOAL_RIGHT_XPOS =  1.48;

    Robot(RTDBConn DBC_in, int device_nr_in, int robot_array_index, Position pos);
    ~Robot();

    int spot_turn(Angle phi_in, int extra_wait_time_ms=500, bool verbose=true);
    int drive_parallel(float diff_to_drive);

    // controller functions
    int update_speed_controller(Angle ref_heading, Angle cur_heading);
    int update_heading_controller(Angle ref_heading, Angle cur_heading);
    void set_wheelspeed(int timer_duration);

    Path_finder get_path_finder();
    void set_sampling_time(int sampling_time);

    // target pos
    void set_target_pos(Position pos);
    Position get_target_pos();

    // misc
    int ddeg(Angle cur_phi, Angle goal_phi);

    // try out alternative controller
    // TODO: Bias in heading
    const int prec = 3; // output precision
    double err_heading_sum = 0;
    double err_heading_before = 0;
    double u_heading(double bias, double KP_h, double KI_h, double KD_h, bool debug=true);
    double u_dist(double KP_d, double KI_d, double KD_d, bool debug=true);
    void update_movement(Timer& timer, double KP_h, double KI_h, double KD_h, double KP_d, double KI_d, double KD_d, bool debug=true);
    double clip(double input, const double limit, string saturation_print);

};

#endif // ROBOT_H
