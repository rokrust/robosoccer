#include "goalie.h"

Goalie::Goalie(RTDBConn DBC_in, int device_nr_in) : Robot(DBC_in, device_nr_in)
{

}


/*
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
}*/
