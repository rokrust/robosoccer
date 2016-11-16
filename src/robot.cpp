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

#define BASE_TURN_SPEED 80
#define TURN_RAMP_UP 0
#define DDEG_MULTIPLYER 2.29

// drive constants
#define PI 3.14159265
#define ANGLE_TURN_THRESHOLD 35
#define DSPEED_GAIN 2.0
#define V_GROUND_MAX 100
#define V_GROUND_MIN 0
#define MAX_DSPEED_REL 0.1
#define DRIVE_DURATION 200
#define WHEEL_SPEED_RAMPUP 10
#define DRIVE_RAMP_UP_START 100
#define DIST_THRESHOLD_LINEAR 0.40
#define DIST_THRESHOLD_STOP 0.08

// goalie constants
#define GOALIE_SPEED 120
#define GOALIE_CONST_SPEED_120_FORWARD 2200
#define GOALIE_CONST_SPEED_120_BACKWARD 2150


Robot::Robot(RTDBConn DBC_in, int device_nr_in, RawBall *datBall_in) : RoboControl(DBC_in, device_nr_in)
{
    device_nr = device_nr_in;
    datBall = datBall_in;
}

Robot::~Robot()
{
}

void Robot::drive_to_pos(Position pos_in, bool verbose=false)
{    
    // initialize left and right wheel speed, ground speed and speed difference
    int v_left, v_right, v_ground, dspeed;

    // calculate the distance between the robot and the goal
    Position goal_pos = pos_in;
    Position cur_pos = this->GetPos();
    double dist = cur_pos.DistanceTo(pos_in);
    if (verbose) {
        cout << "Distance to goal: " << dist << "\n" << endl;
    }

    // calculate the difference in the orientation
    Angle cur_phi = this->GetPhi();
    Angle goal_phi = cur_pos.AngleOfLineToPos(goal_pos);
    int ddeg = calc_ddeg(goal_phi);

    if (dist > DIST_THRESHOLD_STOP) {
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
            ddeg = calc_ddeg(goal_phi);

            // update the distance
            cur_pos = this->GetPos();
            dist = cur_pos.DistanceTo(pos_in);

            // calculate the ground speed depending on the distance
            if (dist > DIST_THRESHOLD_LINEAR) {
                v_ground = V_GROUND_MAX;
            } else {
                v_ground = int(dist*((V_GROUND_MAX - V_GROUND_MIN) / DIST_THRESHOLD_LINEAR) + V_GROUND_MIN);
            }

            // calculate the speed difference that will be applied on the wheels,
            // depending on the ground speed and the difference in orientation
            dspeed = int((float(fabs(sin(ddeg*PI/180))) * float(DSPEED_GAIN)) * float(MAX_DSPEED_REL) * float(v_ground));
            if (ddeg > 0) {
                // robot orientation too far left of goal in driving direction
                v_left = v_ground - dspeed;
                v_right = v_ground + dspeed;
            } else {
                // robot orientation too far right of goal in driving direction
                v_left = v_ground + dspeed;
                v_right = v_ground - dspeed;
            }

            int cur_left = this->GetSpeedLeft();
            int cur_right = this->GetSpeedRight();
            int ramp_up;


            if ((cur_left > WHEEL_SPEED_RAMPUP) || (cur_right > WHEEL_SPEED_RAMPUP)) {
                ramp_up = 0;
            } else {
                ramp_up = DRIVE_RAMP_UP_START;
            }

            ramp_up = 50;

            if (verbose) {
                cout << "Disance to goal is: " << dist << endl;
                cout << "Ground speed is: " << v_ground << endl;
                cout << "ddeg is: " << ddeg << endl;
                cout << "Left speed: " << v_left << "   Right speed: " << v_right << endl;
                cout << "RampUp is: " << ramp_up << endl;
                cout << "---------------------" << endl;
            }

            // set the wheel speeds for the run time
            this->MoveMs(v_left, v_right, DRIVE_DURATION, ramp_up);
            usleep((DRIVE_DURATION + 30)*1000);
        }
    }
}

int Robot::drive_parallel(float diff_to_drive, bool verbose=false)
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

int Robot::calc_ddeg(Angle goal_phi) {
    Angle cur_phi = this->GetPhi();
    int ddeg = goal_phi.Deg() - cur_phi.Deg();
    if (ddeg > 180) {
        ddeg = ddeg - 360;
    }
    if (ddeg < -180) {
        ddeg = ddeg + 360;
    }
    return ddeg;
}

Position Robot::calc_pos_diff(Position pos_a, Position pos_b)
{
    // calculate the difference between two position (e.g. the direction of a movement)
    Position pos_diff;
    pos_diff.SetX(pos_a.GetX() - pos_b.GetX());
    pos_diff.SetY(pos_a.GetY() - pos_b.GetY());
    return pos_diff;
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

void Robot::test_loop_drive_parallel()
{
    // set the wheel speed for the turn time
    Position r1 = this->GetPos();
    Position r0 = r1;

    // From three measurements:
    // Go forward at 3076.92 ms per m
    // Go backward at 3076.92 ms per m
    // Formula to apply: run_time = 3076.92(ms/m) * diff_to_drive(m)

    cin.get();
    cin.get();

    cout << "Sign" << "\t" << "diff_to_drive" << "\t" << "Runtime" << "\t" << "diff_driven\trelError" << endl;


    float diff_to_drive_step = 0.05;

    //int i;
    int run_time;
    int v_left = 100;
    int v_right = 100;

    cout.precision(4);

    //for (i = 50; i < 900; i = i + 50)
    for (float diff_to_drive = 0.05; diff_to_drive < 0.31; diff_to_drive = diff_to_drive + diff_to_drive_step)
    {
        // Test forward
        run_time = 2500 * diff_to_drive; // forward: 3200 for speed 80 | 2200 for speed 120
        //run_time = i;
        this->MoveMs(v_left, v_right, run_time, TURN_RAMP_UP);
        usleep((run_time+500) * 1000 + 1000000);

        r0 = r1;
        r1 = this->GetPos();

        cout << "'+" << "\t'" << diff_to_drive << "\t'" << run_time << "\t'"
             << r1.DistanceTo(r0) << "\t'" << r1.DistanceTo(r0)/diff_to_drive << endl;

        // Test backward
        run_time = 2600 * diff_to_drive; // backward: 3400 for speed 80 | 2150 for speed 120
        //run_time = i;
        this->MoveMs(-v_left, -v_right, run_time, TURN_RAMP_UP);
        usleep((run_time+500) * 1000 + 1000000);

        r0 = r1;
        r1 = this->GetPos();

        cout << "'-" << "\t'" << diff_to_drive << "\t'" << run_time << "\t'"
             << r1.DistanceTo(r0) << "\t'" << r1.DistanceTo(r0)/diff_to_drive << endl;
    }
}
