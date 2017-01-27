
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


// turn constants
#define BASE_TURN_SPEED 80
#define TURN_RAMP_UP 0
#define DDEG_MULTIPLYER 2.29
#define MAX_WHEELSPEED 200

// goalie constants
#define GOALIE_SPEED 120
#define GOALIE_CONST_SPEED_120_FORWARD 2200
#define GOALIE_CONST_SPEED_120_BACKWARD 2150

// controller driving constants
#define ACCEPTABLE_DISTANCE_THRESHOLD 0.04 // 0.08
#define ACCEPTABLE_HEADING_THRESHOLD 0.05
#define VIA_POS_THRESHOLD 0.1 // in m; if distance to via pos is lower than that, the real target pos is aimed for
#define VIA_POS_OFFSET 0.2 // in m; small: approach via pos with small speed in the end, big: big speeds

#define EXTRAPOL_LIMIT 1.0

//Field position constants
#define GOAL_MAX_YPOS 0.13
#define GOAL_MIN_YPOS -0.13
#define GOAL_LEFT_XPOS -1.48
#define GOAL_RIGHT_XPOS 1.48


enum Parameters
     {K_ph = 20, // working thursday: 20
      K_dh = 5, // working thursday: 5
      K_pt = 170, // working thursday: 170
      K_it = 15 }; // working well: 15

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


    Controller_data controller_data;
    Path_finder path_finder;

    Position target_pos; //Might put this in another class
    
    void reset_integrators_if_necessary(Angle ref_heading, Angle cur_heading);
    double error_buffer_mean();

public:

    Robot(RTDBConn DBC_in, int device_nr_in, int robot_array_index);
    ~Robot();

    int spot_turn(Angle phi_in, bool verbose=true);

    int drive_parallel(float diff_to_drive);

    // controller functions
    int update_speed_controller(Angle ref_heading, Angle cur_heading);
    int update_heading_controller(Angle ref_heading, Angle cur_heading);
    int set_wheelspeed(Position* robot_positions);

    void set_sampling_time(double sampling_time);
    double get_sampling_time();
    void set_target_pos(Position target_pos_to_set);
    Position get_target_pos();

    // misc
    int ddeg(Angle goal_phi);
};

#endif // ROBOT_H
