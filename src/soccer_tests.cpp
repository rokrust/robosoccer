//============================================================================
// Name        : soccer_test.cpp
// Author      :
// Version     :
// Copyright   : (C)
// Description : Implementation of the Soccer Test class
//============================================================================

#include "soccer_tests.h"
#include "game.h"

Soccer_Tests::Soccer_Tests(Goalie* goalie_in, Striker* striker1_in, Striker* striker2_in,
                           Opponent* opponent1_in, Opponent* opponent2_in, Opponent* opponent3_in)
{
    goalie = goalie_in;
    striker1 = striker1_in;
    striker2 = striker2_in;
    opponent1 = opponent1_in;
    opponent2 = opponent2_in;
    opponent3 = opponent3_in;

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
    int robot_choice_in;

    while(true) {
        Robot* my_robot;
        cout << "Select robot to control:" << endl;
        cout << "1: Goalie" << endl;
        cout << "2: Striker1" << endl;
        cout << "3: Striker2" << endl;
        cout << "4: Opponent1" << endl;
        cout << "5: Opponent2" << endl;
        cout << "6: Opponent3" << endl;
        cin >> robot_choice_in;
        switch (robot_choice_in) {
        case 1: my_robot = Game::goalie; break;
        case 2: my_robot = Game::striker1; break;
        case 3: my_robot = Game::striker2; break;
        case 4: my_robot = Game::opponent1; break;
        case 5: my_robot = Game::opponent2; break;
        case 6: my_robot = Game::opponent3; break;
        default: cout << "Wrong input. Setting default to goalie" << endl;
            my_robot = Game::goalie; break;
        }

        cout << "X: " << endl;
        cin >> x_in;
        cout << "Y: " << endl;
        cin >> y_in;
        Position goal_pos(x_in, y_in);
        cout << "Going to position x:" << x_in << " y:" << y_in << endl;
        my_robot->drive_to_pos(goal_pos, true);
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
