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
    // WITH PATHFINDER
    //Acceptably close to target_pos
    /* if(GetPos().DistanceTo(path_finder.get_target_pos()) < ACCEPTABLE_DISTANCE_THRESHOLD){
        controller_data.speed_integrator = 0;
    } */

    // WITHOUT PATHFINDER
    // Acceptably close to target_pos
    if (GetPos().DistanceTo(temporary_target_pos) < ACCEPTABLE_DISTANCE_THRESHOLD) {
        controller_data.speed_integrator = 0;
    }

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
    robot_array_index = robot_array_index;

    go_via_pos = false;

    // initialize controller data
    // can also consider putting this into a private function
    controller_data.sampling_time = 0.01;
    controller_data.speed_integrator = 0.0;
    controller_data.heading_integrator = 0.0;
    controller_data.buffer_size = 6;
    controller_data.current_sample = 0;
    controller_data.error_buffer = new double[6];

    // path_finder = Path_finder(robot_array_index, robot_positions);
}

Robot::~Robot()
{
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

int Robot::spot_turn_on_target_if_necessary()
{
    // Threshold to do a spotturn
    Angle angle_threshold(60);
    Angle cur_heading = GetPhi();
    Angle ref_heading = GetPos().AngleOfLineToPos(temporary_target_pos);
    Angle diff_heading = ref_heading - cur_heading;

    // cout << "Diff Angles = " << diff_heading << endl;

    int wait_time = -1;
    if (diff_heading >= angle_threshold || diff_heading <= -angle_threshold) {
        cout << "SPOT TURNING THE SHIT OUT OF DAT ROBOT - " << endl;
        wait_time = spot_turn(ref_heading);
    }

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

//Set u_speed according to distance_to_pos (should be called every controller tick
//P controller might be good enough
int Robot::update_speed_controller(Angle ref_heading, Angle cur_heading) {
    // WITH PATHFINDER
    // double distance_to_pos = GetPos().DistanceTo(path_finder.get_target_pos());

    // WITHOUT PATHFINDER
    // double distance_to_pos = GetPos().DistanceTo(target_pos);
    double distance_to_pos = GetPos().DistanceTo(temporary_target_pos);

    controller_data.speed_integrator += distance_to_pos * controller_data.sampling_time;

    int u_speed = K_pt*distance_to_pos + K_it*controller_data.speed_integrator;

    u_speed *= cos((ref_heading - cur_heading).Get());

    if (u_speed > MAX_WHEELSPEED || u_speed < -MAX_WHEELSPEED){
        controller_data.speed_integrator = 0;
    }

    cout << "Speed: " << u_speed << endl;
    return u_speed;

}

//Set u_omega according to ref_heading and cur_heading (should be called every controller tick)
int Robot::update_heading_controller(Angle ref_heading, Angle cur_heading){
    double current_error = (ref_heading - cur_heading).Get();
    current_error = sin(current_error);

    controller_data.error_buffer[controller_data.current_sample] = current_error;
    //controller_data.heading_integrator += current_error*controller_data.sampling_time;

    //double int_error = controller_data.heading_integrator;
    double diff_error = (current_error - error_buffer_mean())/controller_data.buffer_size;
    controller_data.current_sample = (controller_data.current_sample + 1) % controller_data.buffer_size;

    int u_omega = K_ph*current_error + K_dh*diff_error; // + K_ih*int_error;

    return u_omega;
}

//Set wheel speed according to u_speed and u_omega (should be called every controller tick)
int Robot::set_wheelspeed(int timer_duration, Position* robot_positions) {

    // WITH VECTOR FIELDS
    // Angle ref_heading = path_finder.calculate_reference_angle(robot_array_index, robot_positions);
    // Angle cur_heading = GetPhi();

    // WITHOUT VECTOR FIELDS
    Angle ref_heading = GetPos().AngleOfLineToPos(temporary_target_pos);
    Angle cur_heading = GetPhi();

    reset_integrators_if_necessary(ref_heading, cur_heading);

    int u_omega = update_heading_controller(ref_heading, cur_heading);
    int u_speed = update_speed_controller(ref_heading, cur_heading);

    //hard coding should be removed
    
    if (u_speed > saturation_speed) {
        u_speed = saturation_speed;
    }
    if (u_speed < -saturation_speed) {
        u_speed = -saturation_speed;
    }

    int right_wheel_speed = u_speed + u_omega;
    int left_wheel_speed = u_speed - u_omega;

    //hard coding should be removed
    MoveMs(left_wheel_speed, right_wheel_speed, timer_duration+10, 100);


    return 0;

    /* int wait_time = spot_turn_on_target_if_necessary();
    return wait_time; */
}

void Robot::set_sampling_time_s(double sampling_time_s)
{
    controller_data.sampling_time = sampling_time_s;
}

double Robot::get_sampling_time_s()
{
    return controller_data.sampling_time;
}

void Robot::set_robot_target_pos(Position target_pos_to_set)
{
    target_pos = target_pos_to_set;
    temporary_target_pos = target_pos;
    go_via_pos = false;
}

bool Robot::update_temporary_target_pos(bool extrapol)
{
    bool changed_to_final_target_position = false;

    if (abs(GetPos().DistanceTo(via_pos)) <= VIA_POS_THRESHOLD && go_via_pos)
    {
        cout << "Distance to Via Pos is " << GetPos().DistanceTo(via_pos) << endl;
        go_via_pos = false;
        changed_to_final_target_position = true; // changes temporary target position from via_pos to target_pos, so this is true
    }

    // Linearly shift the via pos a bit further so the robot drives faster over it
    Position pos;
    if (go_via_pos) {
        Position direction_to_via_pos(via_pos.GetX()-GetPos().GetX(), via_pos.GetY()-GetPos().GetY());
        double distance_to_via_pos = abs(via_pos.DistanceTo(GetPos()));
        // Scale
        direction_to_via_pos.SetX(direction_to_via_pos.GetX()/distance_to_via_pos);
        direction_to_via_pos.SetY(direction_to_via_pos.GetY()/distance_to_via_pos);
        pos.SetX(via_pos.GetX() + (VIA_POS_THRESHOLD+VIA_POS_OFFSET)*direction_to_via_pos.GetX());
        pos.SetY(via_pos.GetY() + (VIA_POS_THRESHOLD+VIA_POS_OFFSET)*direction_to_via_pos.GetY());
    } else {
        pos = target_pos;
    }

    // Extrapolation Shit
    if (!extrapol) {
        temporary_target_pos = pos; // Not extrapolating (because it should not extrapolated as given by variable extrapol)
    }
    else if (extrapol) {
        Position current_pos = GetPos();
        double dist_to_targ = current_pos.DistanceTo(pos);
        if (dist_to_targ > EXTRAPOL_LIMIT) {
            Position direction_to_targ((pos.GetX() - current_pos.GetX()) / dist_to_targ,
                                       (pos.GetY() - current_pos.GetY()) / dist_to_targ);
            Position extrapol_target_pos((current_pos.GetX() + EXTRAPOL_LIMIT * direction_to_targ.GetX()),
                                         (current_pos.GetY() + EXTRAPOL_LIMIT * direction_to_targ.GetY()));
            temporary_target_pos = extrapol_target_pos; // Really extrapolating (Distance to targ > EXTRAPOL_LIMIT)
        } else {
            temporary_target_pos = pos; // Not extrapolating (Distance to targ < EXTRAPOL_LIMIT)
        }
    }

    return changed_to_final_target_position;
}

void Robot::set_robot_via_path(Position via_pos_to_set, Position target_pos_to_set)
{
    target_pos = target_pos_to_set;
    via_pos = via_pos_to_set;
    temporary_target_pos = via_pos;
    go_via_pos = true;
}

Position Robot::get_robot_target_pos()
{
    return target_pos;
}

Position Robot::get_temporary_target_pos()
{
    return temporary_target_pos;
}

Position Robot::get_robot_via_pos()
{
    return via_pos;
}


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


// PATHFINDER SETTERS AND GETTERS
void Robot::set_pathfinder_target_pos(Position pos, bool extrapol)
{
    if (extrapol) {
        Position current_pos = GetPos();
        double dist_to_targ = current_pos.DistanceTo(pos);
        if (dist_to_targ > EXTRAPOL_LIMIT) {
            Position direction_to_targ((pos.GetX() - current_pos.GetX()) / dist_to_targ,
                                       (pos.GetY() - current_pos.GetY()) / dist_to_targ);
            Position extrapol_target_pos((current_pos.GetX() + EXTRAPOL_LIMIT * direction_to_targ.GetX()),
                                         (current_pos.GetY() + EXTRAPOL_LIMIT * direction_to_targ.GetY()));
            path_finder.set_target_pos(extrapol_target_pos); // Really extrapolating (Distance to targ > EXTRAPOL_LIMIT)
        } else {
            path_finder.set_target_pos(pos); // Not extrapolating (Distance to targ < EXTRAPOL_LIMIT)
        }
    } else {
        path_finder.set_target_pos(pos); // Not extrapolating (because it should not extrapolated as given by variable extrapol)
    }
}

Position Robot::get_pathfinder_target_pos()
{
    return path_finder.get_target_pos();
}
