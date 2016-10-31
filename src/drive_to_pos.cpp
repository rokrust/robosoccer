//============================================================================
// Name        : drive_to_pos.cpp
// Author      : Simon B. Gasse
// Version     :
// Copyright   : (C)
// Description : Basic driving function
//============================================================================


#include <time.h>
#include <iostream>
#include "kogmo_rtdb.hxx"
#include "robo_control.h"
#include <cmath>



/* Fallback

void drive_to_pos(RoboControl &robo, Position &pos, int velo) {
    robo.GotoXY(pos.GetX(), pos.GetY(), velo, true);
}

*/

// dist: 0.58
// velo: 40
// this is a test

int drive_to_pos(RoboControl &robo, Position &goal_pos, int velo, int move_time) {
    robo.GotoXY(goal_pos.GetX(), goal_pos.GetY(), velo, true);

    // get current position, current orientation and goal orientation
    Position cur_pos = robo.GetPos();
    Angle cur_phi = robo.GetPhi();
    Angle goal_phi = cur_pos.AngleOfLineToPos(goal_pos);

    int ddeg, v_left, v_right;
    int base_velo = velo;
    int run_ms = 200;
    int ramp_up = 0;

    // baseline solution: turn and move then
    robo.TurnAbs(goal_phi);

    /** Drive while the distance is above a threshold */
    while ((abs(robo.GetX() - goal_pos.GetX()) > 0.1) || (abs(robo.GetY() - goal_pos.GetY()) > 0.1)) {
        cur_pos = robo.GetPos();
        cur_phi = robo.GetPhi();
        goal_phi = cur_pos.AngleOfLineToPos(goal_pos);

        ddeg = goal_phi.Deg() - cur_phi.Deg();
        v_left = base_velo - (ddeg/2);
        v_right = base_velo + (ddeg/2);

        robo.MoveMs(v_left, v_right, run_ms, ramp_up);

    }

    // Return estimated movement time
    if (move_time == 0) {
        move_time = 10000000;
    }
    return 0;



}


