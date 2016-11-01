#include "robot.h"

// debug switch
#define DEBUG 1

// turn constants - change with caution!
#define ANGLE_TURN_THRESHOLD 35
#define BASE_TURN_SPEED 60
#define TURN_RAMP_UP 50
#define DDEG_MULTIPLYER 182

// drive constants
#define MIN_SPEED 20
#define MAX_DSPEED_REL 0.15
#define V_GROUND_FACTOR 100
#define RUN_ONCE 80
#define DRIVE_RAMP_UP 50
#define DIST_THRESHOLD 0.10


Robot::Robot(RTDBConn DBC_in, int device_nr_in) : RoboControl(DBC_in, device_nr_in)
{
    device_nr = device_nr_in;
}

Robot::~Robot()
{

}

void Robot::drive_to_pos(Position pos_in)
{
    int v_left, v_right, v_ground, dspeed;

    Position goal_pos = pos_in;
    Position cur_pos = this->GetPos();
    double dist = this->calc_dist(cur_pos, pos_in);

    if (DEBUG) {
        cout << "Distance to goal: " << dist << "\n" << endl;
    }

    Angle cur_phi = this->GetPhi();
    Angle goal_phi = cur_pos.AngleOfLineToPos(goal_pos);

    int ddeg = goal_phi.Deg() - cur_phi.Deg();
    if (DEBUG) {
        cout << "Current orientation is: " << cur_phi.Deg() << endl;
        cout << "Goal line orientation is: " << goal_phi.Deg() << endl;
        cout << "Angle difference is: " << ddeg << endl;
    }

    if (abs(ddeg) > ANGLE_TURN_THRESHOLD) {
        int wait_time = this->spot_turn(goal_phi);
        usleep(wait_time);
    }

    while (dist > DIST_THRESHOLD) {
        // drive the robot
        cur_phi = this->GetPhi();
        goal_phi = cur_pos.AngleOfLineToPos(goal_pos);
        ddeg = goal_phi.Deg() - cur_phi.Deg();
        v_ground = std::max(int(dist * V_GROUND_FACTOR), MIN_SPEED);

        if (DEBUG) {
            cout << "Ground speed is: " << v_ground << endl;
        }

        dspeed = int((ddeg/ANGLE_TURN_THRESHOLD)*MAX_DSPEED_REL*v_ground);
        v_left = v_ground - dspeed;
        v_right = v_ground + dspeed;

        if (DEBUG) {
            cout << "Left speed: " << v_left << "   Right speed: " << v_right << endl;
        }

        this->MoveMs(v_left, v_right, RUN_ONCE, DRIVE_RAMP_UP);
        usleep((RUN_ONCE + 30)*1000);

        // update distance
        cur_pos = this->GetPos();
        dist = this->calc_dist(cur_pos, pos_in);
    }
}

double Robot::calc_dist(Position pos_a, Position pos_b)
{
    double x_dist = pos_a.GetX() - pos_b.GetX();
    double y_dist = pos_a.GetY() - pos_b.GetY();
    double dist = sqrt(pow(x_dist, 2) + pow(y_dist, 2));
    return dist;
}

int Robot::spot_turn(Angle phi_in)
{
    int v_left, v_right;

    Angle cur_phi = this->GetPhi();
    Angle goal_phi = phi_in;
    int ddeg = goal_phi.Deg() - cur_phi.Deg();

    if (DEBUG) {
        cout << "Angle difference before turn: " << ddeg << endl;
    }

    if (ddeg > 0) {
        v_left = -BASE_TURN_SPEED;
        v_right = BASE_TURN_SPEED;
    } else {
        v_left = BASE_TURN_SPEED;
        v_right = -BASE_TURN_SPEED;
    }

    int turn_time = (abs(ddeg) * DDEG_MULTIPLYER) / BASE_TURN_SPEED;
    if (DEBUG) {
        cout << "Turn time: " << turn_time << endl;
    }

    this->MoveMs(v_left, v_right, turn_time, TURN_RAMP_UP);

    int wait_time = (turn_time + 30) * 1000;
    if (DEBUG) {
        cout << "Wait time in ms: " << wait_time << endl;
    }


    if (DEBUG) {
        usleep(wait_time);
        cur_phi = this->GetPhi();
        goal_phi = phi_in;
        ddeg = goal_phi.Deg() - cur_phi.Deg();
        cout << "After turn angle difference: " << ddeg << endl;
        return 0;
    }

    return wait_time;
}
