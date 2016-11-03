//============================================================================
// Name        : soccer_test.cpp
// Author      :
// Version     :
// Copyright   : (C)
// Description : Implementation of the Soccer Test class
//============================================================================

#include "soccer_tests.h"

Soccer_Tests::Soccer_Tests(Robot* blue1_in, Robot* blue2_in, Robot* blue3_in,
                           Robot* red1_in, Robot* red2_in, Robot* red3_in,
                           RawBall* datBall_in)
{
    blue1 = blue1_in;
    blue2 = blue2_in;
    blue3 = blue3_in;
    red1 = red1_in;
    red2 = red2_in;
    red3 = red3_in;
    datBall = datBall_in;
}

void Soccer_Tests::move_in_out()
{
    // Define outer positions for all six robots
    Position posb0_out(-0.5, -0.6);
    Position posb1_out(0.0, -0.6);
    Position posb2_out(0.5, -0.6);
    Position posr0_out(-0.5, 0.6);
    Position posr1_out(0.0, 0.6);
    Position posr2_out(0.5, 0.6);

    // Define inner positions for all six robots
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
    Position posgoal(0.35, 0.3);

    blue1->drive_to_pos(posgoal);
}

void Soccer_Tests::test_turns()
{
    Angle to_pc(0);
    Angle to_wall(180);

    int wait_time;

    wait_time = blue1->spot_turn(to_pc);
    //blue1->TurnAbs(0);
    usleep(wait_time);

    usleep(3000000);

    // blue1->TurnAbs(180);
    wait_time = blue1->spot_turn(to_wall);
    usleep(wait_time);
}

int Soccer_Tests::test_goalie() {
    bool goal_at_pc_side; // = false;
    cout << "Which side? (1 for goal at PC side, 0 for goal at other side)" << endl;
    cin >> goal_at_pc_side;

    // Initialize Goalie Position
    Position goalie_pos_init(0.0, 0.25);
    Angle goalie_angle_init(-90);
    if (goal_at_pc_side) {
        goalie_pos_init.SetX(1.3); // (1.3, 0.25)
    } else {
        goalie_pos_init.SetX(-1.3); // (-1.3, 0.25) - Angle to zero is fine
    }
    blue1->GotoXY(goalie_pos_init.GetX(), goalie_pos_init.GetY(), 160, true);
    usleep(5000 * 1000);

    blue1->spot_turn(goalie_angle_init);
    usleep(1.5 * pow(10,6));

    // Wait for a ball to appear close to the goal
    Position r1_ball = datBall->GetPos();
    Position r0_ball = r1_ball;
    Position dr_ball = blue1->calc_pos_diff(r1_ball, r0_ball);
    Position dr_ball_mean = dr_ball;

    Position r_goalie = blue1->GetPos();
    float x_goalline = r_goalie.GetX();

    bool wait4ball = true;
    while (wait4ball) {
        usleep(100 * 1000); // x (ms) * 1000

        r0_ball = r1_ball;
        r1_ball = datBall->GetPos();
        dr_ball = blue1->calc_pos_diff(r1_ball, r0_ball);

        // Calculate the mean over the last 5 values
        dr_ball_mean.SetX(1*dr_ball.GetX() + 4*dr_ball_mean.GetX()/5);
        dr_ball_mean.SetY(1*dr_ball.GetY() + 4*dr_ball_mean.GetY()/5);

        float dx_ball2goalline = abs(x_goalline - r1_ball.GetX()); // Distance from Ball to the Goalline, only x relevant
        cout << "dx_ball = " << dx_ball2goalline << " - dr_ball = " << dr_ball_mean.GetPos() << endl;


        if (dx_ball2goalline <= 0.9) {
            // y = m*x + y_0
            double m = dr_ball_mean.GetY() / dr_ball_mean.GetX();
            double x_G = x_goalline;
            double x_0 = r1_ball.GetX();
            double y_0 = r1_ball.GetY();
            double y_G = m * (x_G - x_0) + y_0;

            cout << "y_G = " << y_G << endl;

            if (y_G < 0.25 && y_G >= -0.25) {
                cout << "Goalie reaction" << endl;
                blue1->GotoXY(x_G, y_G, 120, true);
            }

            usleep(4000 *1000);

            wait4ball = false;
        }
    }

    cout << "Ball is close! " << endl;


    return 0;
}
