
//============================================================================
// Name        : robot.cpp
// Author      :
// Version     :
// Copyright   : (C)
// Description : Implementation of the Robot class
//============================================================================

#include "robot.h"
#include "game.h"

double Robot::error_buffer_mean(){
    double average_error = 0;

    for(int i = 0; i < controller_data.buffer_size; i++){
        average_error += controller_data.error_buffer[i];
    }

    return average_error/= controller_data.buffer_size;
}

void Robot::reset_integrators_if_necessary(Angle ref_heading, Angle cur_heading){

    //Acceptably close to target_pos
    if((ref_heading - cur_heading).Abs() < ACCEPTABLE_HEADING_THRESHOLD){
        controller_data.heading_integrator = 0;
    }
}

Robot::Robot(RTDBConn DBC_in, int device_nr_in, int robot_array_index) :
			 RoboControl(DBC_in, device_nr_in)
{
    cout << "Robot Object initialized" << endl;
    device_nr = device_nr_in;
    this->robot_array_index = robot_array_index;

    // initialize controller data
    // can also consider putting this into a private function
    controller_data.sampling_time = 0.01;
    controller_data.speed_integrator = 0.0;
    controller_data.heading_integrator = 0.0;
    controller_data.buffer_size = 6;
    controller_data.current_sample = 0;
    controller_data.error_buffer = new double[6];

    path_finder = Path_finder(robot_array_index);
}

Robot::~Robot()
{
}

//Set u_speed according to distance_to_pos (should be called every controller tick
//P controller might be good enough
int Robot::update_speed_controller(Angle ref_heading, Angle cur_heading) {
    double distance_to_pos = GetPos().DistanceTo(target_pos);

    if(distance_to_pos < ACCEPTABLE_DISTANCE_THRESHOLD){
        return 0;
    }

    controller_data.speed_integrator += distance_to_pos * controller_data.sampling_time;

    int u_speed = K_pt*distance_to_pos + K_it*controller_data.speed_integrator;
    u_speed *= cos((ref_heading - cur_heading).Get());



    return u_speed;

}

//Set u_omega according to ref_heading and cur_heading (should be called every controller tick)
int Robot::update_heading_controller(Angle ref_heading, Angle cur_heading){
    double current_error = (ref_heading - cur_heading).Get();

    //current_error = sin(current_error); //WTF?? From shit period

    controller_data.error_buffer[controller_data.current_sample] = current_error;
    //controller_data.heading_integrator += current_error*controller_data.sampling_time;

    //double int_error = controller_data.heading_integrator;
    double diff_error = (current_error - error_buffer_mean())/controller_data.buffer_size;
    controller_data.current_sample = (controller_data.current_sample + 1) % controller_data.buffer_size;

    int u_omega = K_ph*current_error + K_dh*diff_error; // + K_ih*int_error;



    return u_omega;
}

//Set wheel speed according to u_speed and u_omega (should be called every controller tick)
int Robot::set_wheelspeed(Position* robot_positions) {
    Angle ref_heading = path_finder.calculate_reference_angle(robot_array_index, robot_positions);
    Angle cur_heading = GetPhi();

    int u_omega = update_heading_controller(ref_heading, cur_heading);
    int u_speed = update_speed_controller(ref_heading, cur_heading);

    if(u_speed == 0){
        controller_data.heading_integrator = 0;
        controller_data.speed_integrator = 0;
        MoveMs(0, 0, 1000, 100);
        return 1;
    }

    //Speed limit
    if (u_speed > MAX_WHEELSPEED) {
        u_speed = MAX_WHEELSPEED;
    }
    if (u_speed < -MAX_WHEELSPEED) {
        u_speed = -MAX_WHEELSPEED;
    }

    int right_wheel_speed = u_speed + u_omega;
    int left_wheel_speed = u_speed - u_omega;

    //right_wheel_speed = 0;
    //left_wheel_speed = 0;

    //hard coding should be removed
    MoveMs(left_wheel_speed, right_wheel_speed, 1000, 100);


    return 0;

}

void Robot::set_sampling_time(double sampling_time)
{
    controller_data.sampling_time = sampling_time;
}

double Robot::get_sampling_time()
{
    return controller_data.sampling_time;
}

void Robot::set_target_pos(Position target_pos)
{
    this->target_pos = target_pos;
    path_finder.set_target_pos(target_pos);
}

Position Robot::get_target_pos()
{
    return target_pos;
}

void Robot::set_avoidance_degree(int field_index, double avoidance_degree){
    path_finder.set_robot_vector_field_weight(field_index, avoidance_degree);

}


/*******************************Shit*******************************/

int Robot::ddeg(Angle goal_phi)
{
    Angle cur_phi = GetPhi();
    int ddeg = goal_phi.Deg() - cur_phi.Deg();
    if (ddeg > 180) {
        ddeg = ddeg - 360;
    }
    if (ddeg < -180) {
        ddeg = ddeg + 360;
    }
    return ddeg;
}

int Robot::spot_turn(Angle phi_in, bool verbose)
{
    // define left and right wheel speed variables
    int v_left, v_right;

    // calculate the difference in the current and the desired orientation
    // Angle cur_phi = this->GetPhi();
    Angle goal_phi = phi_in;
    int cur_ddeg = ddeg(goal_phi);  // goal_phi.Deg() - cur_phi.Deg();
    if (verbose) {
        cout << "Angle difference before turn: " << cur_ddeg << endl;
    }

    if (cur_ddeg > 0) {
        // clockwise
        v_left = -BASE_TURN_SPEED;
        v_right = BASE_TURN_SPEED;
    } else {
        // anti-clockwise
        v_left = BASE_TURN_SPEED;
        v_right = -BASE_TURN_SPEED;
    }

    // calculate the turn time depending on the degree to turn and the turning speed
    int turn_time = int(DDEG_MULTIPLYER * abs(cur_ddeg));
    if (verbose) {
        cout << "Turn time: " << turn_time << endl;
    }

    // set the wheel speed for the turn time
    MoveMs(v_left, v_right, turn_time, TURN_RAMP_UP);

    // calculate the time that the turn will take in micro seconds
    // there is 30ms delay due to the bluetooth system
    int wait_time = (turn_time + 30);
    if (verbose) {
        cout << "Wait time in ms: " << wait_time << endl;
    }

    // return the time that the turn will take for higher level functions to wait
    return wait_time;
}

int Robot::drive_parallel(float diff_to_drive)
{

    // Optimized for Distances between +/- 0.25m
    int v_left, v_right;
    int run_speed = GOALIE_SPEED;

    // calculate the difference in the current and the desired orientation
    int run_time;
    if (diff_to_drive > 0) {
        // forward
        run_time = GOALIE_CONST_SPEED_120_FORWARD * abs(diff_to_drive);
        v_left = run_speed;
        v_right = run_speed;
    } else {
        // backward
        run_time = GOALIE_CONST_SPEED_120_BACKWARD * diff_to_drive;
        v_left = -run_speed;
        v_right = -run_speed;
    }

    cout << "% Runtime: " << run_time << endl;
    this->MoveMs(v_left, v_right, abs(run_time), TURN_RAMP_UP);

    int wait_time = (int(run_time) + 200) * 1000;

    return wait_time;
}

