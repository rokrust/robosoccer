#include "goalie.h"

Goalie::Goalie(RTDBConn DBC_in, int device_nr_in, RawBall* datBall_in) : Robot(DBC_in, device_nr_in, datBall_in)
{

}

int Goalie::go_to_penalty_save_position(bool is_left_side)
{
    Position goaliePos;
    Angle goalieAngle(-90);

    /* if (is_left_side) {
        goaliePos.SetX(-1.3);
        goaliePos.SetY(0.0);
    }
    else {
        goaliePos.SetX(1.3);
        goaliePos.SetY(0.0);
    }

    GotoPos(goaliePos);
    usleep(4000 * 1000);
    spot_turn(goalieAngle); */



    // Wait for a ball to appear close to the goal
    Position r1_ball = datBall->GetPos();
    Position r0_ball = r1_ball;
    Position dr_ball = calc_pos_diff(r1_ball, r0_ball);
    Position dr_ball_mean = dr_ball;

    Position r_goalie = GetPos();
    float x_goalline = r_goalie.GetX();

    bool wait4ball = true;
    int i = 0;
    while (wait4ball) {
        usleep(100 * 1000); // x (ms) * 1000

        r_goalie = this->GetPos();
        i = i + 1;

        r0_ball = r1_ball;
        r1_ball = datBall->GetPos();
        dr_ball = calc_pos_diff(r1_ball, r0_ball);

        // Calculate the mean over the last 5 values
        dr_ball_mean.SetX(1*dr_ball.GetX() + 4*dr_ball_mean.GetX()/5);
        dr_ball_mean.SetY(1*dr_ball.GetY() + 4*dr_ball_mean.GetY()/5);

        // y = m*x + y_0
        // y_G = m*x_G + y_0
        double m = dr_ball_mean.GetY() / dr_ball_mean.GetX();
        double x_G = x_goalline;
        double x_0 = r1_ball.GetX();
        double y_0 = r1_ball.GetY();
        double y_G = m * (x_G - x_0) + y_0;

        float dx_ball2goalline = abs(x_goalline - r1_ball.GetX()); // Distance from Ball to the Goalline, only x relevant

        cout << i << "  dx_ball = " << dx_ball2goalline << " - dr_ball = " << dr_ball_mean.GetPos();
        cout << i << "  y_G = " << y_G << endl;

        if (i % 5 == 0 && abs(dr_ball_mean.GetX()) > 0.005)
        {
            if (y_G < 0.25 && y_G >= -0.25) {
                float deltaY = y_G-r_goalie.GetY();
                cout << "Goalie moving deltaY = " << deltaY << endl; // to (" << x_G << " , " << y_G << ")" << endl;
                this->drive_parallel(deltaY, false);
            }
        }


        // if (dx_ball2goalline <= 0.1) {
        // }




          //  wait4ball = false;

    }

    cout << "Saved the ball" << endl;

    return 0;
}
