//============================================================================
// Name        : soccer_test.cpp
// Author      :
// Version     :
// Copyright   : (C)
// Description : Implementation of the Soccer Test class
//============================================================================

#include "soccer_tests.h"

Soccer_Tests::Soccer_Tests(Robot* blue1_in, Robot* blue2_in, Robot* blue3_in,
                           Robot* red1_in, Robot* red2_in, Robot* red3_in)
{
    blue1 = blue1_in;
    blue2 = blue2_in;
    blue3 = blue3_in;
    red1 = red1_in;
    red2 = red2_in;
    red3 = red3_in;
}

void Soccer_Tests::move_in_out()
{
    /** Define outer positions for all six robots */
    Position posb0_out(-0.5, -0.6);
    Position posb1_out(0.0, -0.6);
    Position posb2_out(0.5, -0.6);
    Position posr0_out(-0.5, 0.6);
    Position posr1_out(0.0, 0.6);
    Position posr2_out(0.5, 0.6);

    /** Define inner positions for all six robots */
    Position posb0_in(-0.2, -0.1);
    Position posb1_in(0.0, -0.1);
    Position posb2_in(0.2, -0.1);
    Position posr0_in(-0.2, 0.1);
    Position posr1_in(0.0, 0.1);
    Position posr2_in(0.2, 0.1);

    int loop = 1;

    while (loop)
    {
        cout << "Moving all six robots to the outer positions" << endl;

        blue1->GotoXY(posb0_out.GetX(), posb0_out.GetY(), 40, true);
        blue2->GotoXY(posb1_out.GetX(), posb1_out.GetY(), 40, true);
        blue3->GotoXY(posb2_out.GetX(), posb2_out.GetY(), 40, true);
        red1->GotoXY(posb0_out.GetX(), posr0_out.GetY(), 40, true);
        red2->GotoXY(posb1_out.GetX(), posr1_out.GetY(), 40, true);
        red3->GotoXY(posb2_out.GetX(), posr2_out.GetY(), 40, true);

        usleep(6000000);
        cout << "Moving all six robots to the inner positions" << endl;

        blue1->GotoXY(posb0_out.GetX(), posb0_in.GetY(), 40, true);
        blue2->GotoXY(posb1_out.GetX(), posb1_in.GetY(), 40, true);
        blue3->GotoXY(posb2_out.GetX(), posb2_in.GetY(), 40, true);
        red1->GotoXY(posb0_out.GetX(), posr0_in.GetY(), 40, true);
        red2->GotoXY(posb1_out.GetX(), posr1_in.GetY(), 40, true);
        red3->GotoXY(posb2_out.GetX(), posr2_in.GetY(), 40, true);

        //loop = 0;
        usleep(6000000);
    }
}

void Soccer_Tests::const_wheel_speed()
{
    Position posstart(-0.7, 0.0);

    Angle cur_phi;
    Angle to_pc(0);


    int v_left = 60;
    int v_right = 60;
    int run_ms = 4000;
    int ramp_up = 200;

    int loop = 1;

    while (loop)
    {
        blue1->GotoXY(posstart.GetX(), posstart.GetY(), 40, true);
        usleep(6000000);

        cur_phi = blue1->GetPhi();
        while (abs(cur_phi.Deg() - to_pc.Deg()) > 10)
        {
            blue1->TurnAbs(to_pc);
            cur_phi = blue1->GetPhi();
            usleep(500000);
        }

        blue1->MoveMs(v_left, v_right, run_ms, ramp_up);

        // loop = 0;
        usleep(6000000);
    }
}

void Soccer_Tests::easy_p_ctrl()
{
    Position posstart(-0.7, 0.0);
    Position posgoal(0.7, 0.0);

    Angle cur_phi;
    Angle to_pc(0);


    int base_velo = 40;
    int ddeg, v_left, v_right;
    int goaldeg = 0;
    int run_ms = 200;
    int ramp_up = 0;

    int loop = 1;

    while (loop)
    {
        /** Go to starting position and adjust orientation */
        blue1->GotoXY(posstart.GetX(), posstart.GetY(), 40, true);
        usleep(6000000);

        cur_phi = blue1->GetPhi();
        while (abs(cur_phi.Deg() - to_pc.Deg()) > 10)
        {
            blue1->TurnAbs(to_pc);
            cur_phi = blue1->GetPhi();
            usleep(500000);
        }

        /** Drive while the distance is above a threshold */
        while (abs(blue1->GetX() - posgoal.GetX()) > 0.1)
        {
            cur_phi = blue1->GetPhi();
            ddeg = goaldeg - cur_phi.Deg();
            v_left = base_velo - (ddeg / 2);
            v_right = base_velo + (ddeg / 2);

            blue1->MoveMs(v_left, v_right, run_ms, ramp_up);
        }

        // loop = 0;
    }
}

void Soccer_Tests::drive_to_pos()
{
    //Position posstart(-0.7, 0.0);
    Position posgoal(0.7, 0.0);

    blue1->drive_to_pos(posgoal);
}

void Soccer_Tests::test_turns()
{
    Angle to_pc(0);
    Angle to_wall(180);

    int wait_time;

    wait_time = blue1->spot_turn(to_pc);
    usleep(wait_time);

    usleep(3000000);

    wait_time = blue1->spot_turn(to_wall);
    usleep(wait_time);
}
