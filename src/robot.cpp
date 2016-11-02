//============================================================================
// Name        : robot.cpp
// Author      :
// Version     :
// Copyright   : (C)
// Description : Implementation of the Robot class
//============================================================================

#include "robot.h"

// debug switch
#define DEBUG 1

// turn constants - change with caution!
#define ANGLE_TURN_THRESHOLD 35
#define BASE_TURN_SPEED 80
#define TURN_RAMP_UP 0
#define DDEG_MULTIPLYER 2.29

// drive constants
#define V_GROUND_MAX 200
#define V_GROUND_MIN 10
#define MAX_DSPEED_REL 0.25
#define DRIVE_DURATION 100
#define WHEEL_SPEED_RAMPUP 20
#define DRIVE_RAMP_UP_START 100
#define DIST_THRESHOLD_LINEAR 0.40
#define DIST_THRESHOLD_STOP 0.10


Robot::Robot(RTDBConn DBC_in, int device_nr_in) : RoboControl(DBC_in, device_nr_in)
{
    device_nr = device_nr_in;
}

Robot::~Robot()
{
}

void Robot::drive_to_pos(Position pos_in)
{
    // initialize left and right wheel speed, ground speed and speed difference
    int v_left, v_right, v_ground, dspeed;

    // calculate the distance between the robot and the goal
    Position goal_pos = pos_in;
    Position cur_pos = this->GetPos();
    double dist = this->calc_dist(cur_pos, pos_in);
    if (DEBUG) {
        cout << "Distance to goal: " << dist << "\n" << endl;
    }

    // calculate the difference in the orientation
    Angle cur_phi = this->GetPhi();
    Angle goal_phi = cur_pos.AngleOfLineToPos(goal_pos);
    int ddeg = goal_phi.Deg() - cur_phi.Deg();
    if (DEBUG) {
        cout << "Current orientation is: " << cur_phi.Deg() << endl;
        cout << "Goal line orientation is: " << goal_phi.Deg() << endl;
        cout << "Angle difference is: " << ddeg << endl;
    }

    // if the orientation difference is above a threshold, turn on the spot before driving
    if (abs(ddeg) > ANGLE_TURN_THRESHOLD) {
        int wait_time = this->spot_turn(goal_phi);
        usleep(wait_time);
    }

    // drive the robot while the distance is above the threshold
    while (dist > DIST_THRESHOLD_STOP) {
        // update the difference in orientation
        cur_phi = this->GetPhi();
        goal_phi = cur_pos.AngleOfLineToPos(goal_pos);
        ddeg = goal_phi.Deg() - cur_phi.Deg();

        // calculate the ground speed depending on the distance
        // it is always at least MIN_SPEED
        if (dist > DIST_THRESHOLD_LINEAR) {
            v_ground = V_GROUND_MAX;
        } else {
            v_ground = dist*((V_GROUND_MAX - V_GROUND_MIN) / DIST_THRESHOLD_LINEAR) + V_GROUND_MIN;
        }

        if (DEBUG) {
            cout << "Ground speed is: " << v_ground << endl;
        }

        // calculate the speed difference that will be applied on the wheels,
        // depending on the ground speed and the difference in orientation
        dspeed = int((abs(ddeg) / ANGLE_TURN_THRESHOLD) * MAX_DSPEED_REL * v_ground);
        if (ddeg > 0) {
            // robot orientation too far left of goal in driving direction
            v_left = v_ground + dspeed;
            v_right = v_ground;
        } else {
            // robot orientation too far right of goal in driving direction
            v_left = v_ground;
            v_right = v_ground + dspeed;
        }

        if (DEBUG) {
            cout << "Left speed: " << v_left << "   Right speed: " << v_right << endl;
        }

        int cur_left = this->GetSpeedLeft();
        int cur_right = this->GetSpeedRight();
        int ramp_up;


        if ((cur_left > WHEEL_SPEED_RAMPUP) || (cur_right > WHEEL_SPEED_RAMPUP)) {
            ramp_up = 0;
        } else {
            ramp_up = DRIVE_RAMP_UP;
        }

        // set the wheel speeds for the run time
        this->MoveMs(v_left, v_right, DRIVE_DURATION, ramp_up);
        usleep((RUN_ONCE + 30)*1000);

        // update the distance
        cur_pos = this->GetPos();
        dist = this->calc_dist(cur_pos, pos_in);
    }
}

double Robot::calc_dist(Position pos_a, Position pos_b)
{
    // calculate the Euclidean distance between two positions
    double x_dist = pos_a.GetX() - pos_b.GetX();
    double y_dist = pos_a.GetY() - pos_b.GetY();
    double dist = sqrt(pow(x_dist, 2) + pow(y_dist, 2));
    return dist;
}

int Robot::spot_turn(Angle phi_in)
{
    // define left and right wheel speed variables
    int v_left, v_right;

    // calculate the difference in the current and the desired orientation
    Angle cur_phi = this->GetPhi();
    Angle goal_phi = phi_in;
    int ddeg = goal_phi.Deg() - cur_phi.Deg();
    if (DEBUG) {
        cout << "Angle difference before turn: " << ddeg << endl;
    }

    if (ddeg > 0) {
        // clockwise
        v_left = -BASE_TURN_SPEED;
        v_right = BASE_TURN_SPEED;
    } else {
        // anti-clockwise
        v_left = BASE_TURN_SPEED;
        v_right = -BASE_TURN_SPEED;
    }

    // calculate the turn time depending on the degree to turn and the turning speed
    int turn_time = int(DDEG_MULTIPLYER * abs(ddeg));
    if (DEBUG) {
        cout << "Turn time: " << turn_time << endl;
    }

    // set the wheel speed for the turn time
    this->MoveMs(v_left, v_right, turn_time, TURN_RAMP_UP);

    // calculate the time that the turn will take in micro seconds
    // there is 30ms delay due to the bluetooth system
    int wait_time = (turn_time + 30) * 1000;
    if (DEBUG) {
        cout << "Wait time in ms: " << wait_time << endl;
    }


    if (DEBUG) {
        // wait for the turn to finish and check the orientation after the turn to tweak the parameters
        usleep(wait_time);
        cur_phi = this->GetPhi();
        goal_phi = phi_in;
        ddeg = goal_phi.Deg() - cur_phi.Deg();
        cout << "After turn angle difference: " << ddeg << endl;
        return 0;
    }

    // return the time that the turn will take for higher level functions to wait
    return wait_time;
}

int Robot::spot_turn_time_speed(int turn_time, int wheel_speed, bool left_negativ)
{
    // define left and right wheel speed variables
    int v_left, v_right;

    // calculate the difference in the current and the desired orientation
    Angle cur_phi = this->GetPhi();
    cout << "Current angle before turn: " << cur_phi.Deg() << endl;

    if (left_negativ) {
        // miraculously clockwise
        v_left = -wheel_speed;
        v_right = wheel_speed;
    } else {
        // miraculously anti-clockwise
        v_left = wheel_speed;
        v_right = -wheel_speed;
    }

    // set the wheel speed for the turn time
    this->MoveMs(v_left, v_right, turn_time, TURN_RAMP_UP);

    usleep(2*pow(10, 6));

    Angle new_phi = this->GetPhi();
    cout << "Current angle after turn: " << new_phi.Deg() << endl;

    int diff = new_phi.Deg() - cur_phi.Deg();
    if (diff > 360) {
        diff = diff - 360;
    } else if (diff < 0) {
        diff = diff + 360;
    }
    cout << "Angle difference-----> " << diff << endl;

    return 0;
}
