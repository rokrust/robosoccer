//============================================================================
// Name        : soccer_test.cpp
// Author      :
// Version     :
// Copyright   : (C)
// Description : Implementation of the Soccer Test class
//============================================================================

#include "soccer_tests.h"

Soccer_Tests::Soccer_Tests(Goalie* goalie_in, Striker* striker1_in, Striker* striker2_in,
                           Opponent* opponent1_in, Opponent* opponent2_in, Opponent* opponent3_in,
                           RawBall* datBall_in)
{
    goalie = goalie_in;
    striker1 = striker1_in;
    striker2 = striker2_in;
    opponent1 = opponent1_in;
    opponent2 = opponent2_in;
    opponent3 = opponent3_in;
    datBall = datBall_in;

    cout << "Soccer Tests initialized" << endl;
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

        goalie->GotoXY(posb0_out.GetX(), posb0_out.GetY(), 40, true);
        striker1->GotoXY(posb1_out.GetX(), posb1_out.GetY(), 40, true);
        striker2->GotoXY(posb2_out.GetX(), posb2_out.GetY(), 40, true);
        opponent1->GotoXY(posb0_out.GetX(), posr0_out.GetY(), 40, true);
        opponent2->GotoXY(posb1_out.GetX(), posr1_out.GetY(), 40, true);
        opponent3->GotoXY(posb2_out.GetX(), posr2_out.GetY(), 40, true);

        usleep(6000000);
        cout << "Moving all six robots to the inner positions" << endl;

        goalie->GotoXY(posb0_out.GetX(), posb0_in.GetY(), 40, true);
        striker1->GotoXY(posb1_out.GetX(), posb1_in.GetY(), 40, true);
        striker2->GotoXY(posb2_out.GetX(), posb2_in.GetY(), 40, true);
        opponent1->GotoXY(posb0_out.GetX(), posr0_in.GetY(), 40, true);
        opponent2->GotoXY(posb1_out.GetX(), posr1_in.GetY(), 40, true);
        opponent3->GotoXY(posb2_out.GetX(), posr2_in.GetY(), 40, true);

        //loop = 0;
        usleep(6000000);
    }
}

void Soccer_Tests::drive_to_pos()
{
    Position posgoal(0.7, 0.0);
    Position posgoal2(-0.7, 0.0);
    while(true) {
        goalie->drive_to_pos(posgoal, true);
        cin.get();
        goalie->drive_to_pos(posgoal2, true);
        cin.get();
    }
}

void Soccer_Tests::test_command_driving()
{
    double x_in, y_in;

    while(true) {
        cout << "X: " << endl;
        cin >> x_in;
        cout << "Y: " << endl;
        cin >> y_in;
        Position goal_pos(x_in, y_in);
        cout << "Going to position x:" << x_in << " y:" << y_in << endl;
        goalie->drive_to_pos(goal_pos, true);
        cin.get();
    }
}

void Soccer_Tests::test_turns()
{
    Angle to_pc(90);
    Angle to_wall(-90);

    int wait_time;

    while(true) {

        wait_time = striker2->spot_turn(to_pc);
        usleep(wait_time);
        cin.get();

        wait_time = striker2->spot_turn(to_wall);
        usleep(wait_time);
        cin.get();

    }
}

void Soccer_Tests::turn_experiments()
{
    int r_value;

    int i, j;
    for (j = 0; j < 5; j++) {
        for (i = 1; i <= 10; i++) {
            cout << "Turn time " << 100*i << ", speed 60" << endl;
            cin.get();
            r_value = goalie->spot_turn_time_speed(100*i, 60, true);
        }
    }

    cout << r_value << endl;
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
    goalie->GotoXY(goalie_pos_init.GetX(), goalie_pos_init.GetY(), 160, true);
    usleep(5000 * 1000);

    goalie->spot_turn(goalie_angle_init);
    usleep(1.5 * pow(10,6));

    // Wait for a ball to appear close to the goal
    Position r1_ball = datBall->GetPos();
    Position r0_ball = r1_ball;
    Position dr_ball = goalie->calc_pos_diff(r1_ball, r0_ball);
    Position dr_ball_mean = dr_ball;

    Position r_goalie = goalie->GetPos();
    float x_goalline = r_goalie.GetX();

    bool wait4ball = true;
    while (wait4ball) {
        usleep(100 * 1000); // x (ms) * 1000

        r0_ball = r1_ball;
        r1_ball = datBall->GetPos();
        dr_ball = goalie->calc_pos_diff(r1_ball, r0_ball);

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
                goalie->GotoXY(x_G, y_G, 120, true);
            }

            usleep(4000 *1000);

            wait4ball = false;
        }
    }

    cout << "Ball is close! " << endl;


    return 0;
}
