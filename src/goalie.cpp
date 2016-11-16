#include "goalie.h"
#include "game.h"

#define GOALIE_SLEEP_TIME 0.15
#define CALCULUS_RATE 1
#define MAX_LOOP_COUNTDOWN -250
#define V_X_THRESHOLD 0.01

Goalie::Goalie(RTDBConn DBC_in, int device_nr_in) : Robot(DBC_in, device_nr_in)
{

}

int Goalie::go_to_penalty_save_position()
{
    // Goalie working principle
    // * Estimate the ball movement
    // * Get the goalie line
    // * Calculate where the ball is going to cross the goalie line

    // initialize the four last ball positions
    Position ball_t0;
    Position ball_t1;
    Position ball_t2;
    Position ball_init = Game::datBall->GetPos();
    double threshold_for_ball_from_init = 0.02;

    // initialize a ball velocity vector storing the differential position data and the components
    Position ball_velocity_vector(0, 0);
    double v_x, v_y;

    // initialize intersection calculus variables
    double m, x_G, y_G, x_0, y_0, delta_y_G;

    // get the goalie position and set the x_line coordinate
    Position goalie_pos = GetPos();
    double goalline_x = goalie_pos.GetX();

    int loop_countdown = 3;
    int calculus_countup = 0;

    bool loop = true;
    while (loop) {
        // pass the new ball estimate through the line
        ball_t0 = ball_t1;
        ball_t1 = ball_t2;
        ball_t2 = Game::datBall->GetPos();

        // reduce the loop countdown to know when all ball position values are initialized
        loop_countdown--;

        // update the goalie position
        goalie_pos = GetPos();

        cout << "---------------" << endl;
        cout << "Ball position info for loop_countdown =  " << loop_countdown << endl;
        cout << "t0  x: " << ball_t0.GetX() << "  y: " << ball_t0.GetY() << endl;
        cout << "t1  x: " << ball_t1.GetX() << "  y: " << ball_t1.GetY() << endl;
        cout << "t2  x: " << ball_t2.GetX() << "  y: " << ball_t2.GetY() << endl;

        // enter calculations and movement only if the estimates are set well
        if (loop_countdown <= 0 &&
                (ball_t1.DistanceTo(ball_init) >= threshold_for_ball_from_init &&
                 ball_t2.DistanceTo(ball_init) >= threshold_for_ball_from_init)) {
            // estimate the ball movement
            v_x = ((ball_t2.GetX() - ball_t1.GetX()) / GOALIE_SLEEP_TIME
                   +(ball_t1.GetX() - ball_t0.GetX()) / GOALIE_SLEEP_TIME) / 2;
            v_y = ((ball_t2.GetY() - ball_t1.GetY()) / GOALIE_SLEEP_TIME
                   +(ball_t1.GetY() - ball_t0.GetY()) / GOALIE_SLEEP_TIME) / 2;
            ball_velocity_vector.SetX(v_x);
            ball_velocity_vector.SetY(v_y);

            // calculate the intersection of the ball movement with the goal line
            m = v_y / v_x;
            x_G = goalline_x;
            // x_0 = ball_t2.GetX();
            y_0 = ball_t2.GetY() - m * ball_t2.GetX(); // ball_t2.GetY();
            y_G = m * x_G + y_0; // m * (x_G - x_0) + y_0;


            calculus_countup++;

            cout << "\n\n\nIntersection calculus information" << endl;
            cout << "used for calc y_G" << endl;
            cout << "ball_init = [" << ball_init.GetX() << ", " << ball_init.GetY() << "]; ";
            cout << "ball_t0 = [" << ball_t0.GetX() << ", " << ball_t0.GetY() << "]; " << endl;
            cout << "ball_t1 = [" << ball_t1.GetX() << ", " << ball_t1.GetY() << "]; ";
            cout << "ball_t2 = [" << ball_t2.GetX() << ", " << ball_t2.GetY() << "]; " << endl;
            cout << "m = " << m << "; x_G = " << x_G << ";" << endl;
            cout << "x_0 = " << x_0 << "; y_0 = " << y_0 << ";" << endl;
            cout << "y_G = " << y_G << "; v_x = " << v_x << "; " << endl;
            cout << "v_y = " << v_y << ";" << endl;
            cout << "r_goalie = [" << goalie_pos.GetX() << ", " << goalie_pos.GetY() << "];" << endl;

            // enter possible movement not with every calculation update
            if ((calculus_countup % CALCULUS_RATE) == 0) {
                if ((y_G < 0.25) && (y_G > -0.25)) { //  && (abs(v_x) > V_X_THRESHOLD)) {
                    loop = false;
                    delta_y_G = y_G - GetPos().GetY();
                    cout << "delta_y_G = " << delta_y_G << "; " << endl;
                    cout << "% Moving the goalie!-------------------" << endl;
                    this->drive_parallel(delta_y_G, false);
                } else {
                    cout << "y_G is not within the goal, therefore the goalie does not move." << endl;
                }
            }
        }

        if ((ball_t2.GetX() > 1.20)) {
            loop = false;
            cout << "Exiting loop because of ball position x: " << ball_t2.GetX() << endl;
        }
        if ((ball_t2.GetX() < -1.20)) {
            loop = false;
            cout << "Exiting loop because of ball position x: " << ball_t2.GetX() << endl;
        }
        if ((loop_countdown < MAX_LOOP_COUNTDOWN)) {
            loop = false;
            cout << "Exiting loop because of loop_countdown: " << loop_countdown << endl;
        }
        usleep(GOALIE_SLEEP_TIME * 1000000);

        // To keep on running
        /* if (loop == false) {
            cout << "Keep running? (0: no, 1: yes)";
            cin >> loop;
            if (loop) {
                cout << "Loop is running again" << endl;
                ball_init = datBall->GetPos();
                loop_countdown = 3;
                calculus_countup = 0;

                GotoPos(goalie_pos);
            }
        } */
    }

    cout << "ball_after = [";
    for (int i=0; i<50; i++) {
        ball_t0 = Game::datBall->GetPos();
        cout << ball_t0.GetX() << ", " << ball_t0.GetY() << "; ";
        usleep(100 * 1000);
    }
    cout << "]; " << endl;

    return 0;
}

/*
// COPY FOR LATER: constant goal keeping, to be added: drive to goalie init position after some driving for goal keeping
int Goalie::go_to_penalty_save_position()
{
    // Goalie working principle
    // * Estimate the ball movement
    // * Get the goalie line
    // * Calculate where the ball is going to cross the goalie line

    // initialize the four last ball positions
    Position ball_t0;
    Position ball_t1;
    Position ball_t2;
    Position ball_t3;

    // initialize a ball velocity vector storing the differential position data and the components
    Position ball_velocity_vector(0, 0);
    double v_x, v_y;

    // initialize intersection calculus variables
    double m, x_G, y_G, x_0, y_0, delta_y_G;

    // get the goalie position and set the x_line coordinate
    Position goalie_pos = GetPos();
    double goalline_x = goalie_pos.GetX();

    int loop_countdown = 4;
    int calculus_countup = 0;

    bool loop = true;
    while (loop) {
        // pass the new ball estimate through the line
        ball_t0 = ball_t1;
        ball_t1 = ball_t2;
        ball_t2 = ball_t3;
        ball_t3 = Game::datBall->GetPos();

        // reduce the loop countdown to know when all ball position values are initialized
        loop_countdown--;

        // update the goalie position
        goalie_pos = GetPos();

        cout << "---------------" << endl;
        cout << "Ball position info for loop_countdown =  " << loop_countdown << endl;
        cout << "t0  x: " << ball_t0.GetX() << "  y: " << ball_t0.GetY() << endl;
        cout << "t1  x: " << ball_t1.GetX() << "  y: " << ball_t1.GetY() << endl;
        cout << "t2  x: " << ball_t2.GetX() << "  y: " << ball_t2.GetY() << endl;
        cout << "t3  x: " << ball_t3.GetX() << "  y: " << ball_t3.GetY() << endl;

        // enter calculations and movement only if the estimates are set well
        if (loop_countdown <= 0) {
            // estimate the ball movement
            v_x = ((ball_t3.GetX() - ball_t2.GetX()) / GOALIE_SLEEP_TIME
                    +(ball_t2.GetX() - ball_t1.GetX()) / GOALIE_SLEEP_TIME
                    +(ball_t1.GetX() - ball_t0.GetX()) / GOALIE_SLEEP_TIME) / 3;
            v_y = ((ball_t3.GetY() - ball_t2.GetY()) / GOALIE_SLEEP_TIME
                   +(ball_t2.GetY() - ball_t1.GetY()) / GOALIE_SLEEP_TIME
                   +(ball_t1.GetY() - ball_t0.GetY()) / GOALIE_SLEEP_TIME) / 3;
            ball_velocity_vector.SetX(v_x);
            ball_velocity_vector.SetY(v_y);

            // calculate the intersection of the ball movement with the goal line
            m = v_y / v_x;
            x_G = goalline_x;
            x_0 = ball_t3.GetX();
            y_0 = ball_t3.GetY();
            y_G = m * (x_G - x_0) + y_0;

            calculus_countup++;

            cout << "Intersection calculus information" << endl;
            cout << "m: " << m << endl;
            cout << "x_G: " << x_G << endl;
            cout << "x_0: " << x_0 << endl;
            cout << "y_0: " << y_0 << endl;
            cout << "y_G: " << y_G << endl;
            cout << "v_x: " << v_x << endl;
            cout << "v_y: " << v_y << endl;
            cout << "Goalie Position  x: " << goalie_pos.GetX() << " y: " << goalie_pos.GetY() << endl;

            // enter possible movement not with every calculation update
            if ((calculus_countup % CALCULUS_RATE) == 0) {
                if ((y_G < 0.25) && (y_G > -0.25) && (abs(v_x) > V_X_THRESHOLD)) {
                    delta_y_G = y_G - GetPos().GetY();
                    cout << "delta_y_G: " << delta_y_G << endl;
                    cout << "Moving the goalie!-------------------" << endl;
                    this->drive_parallel(delta_y_G, false);
                } else {
                    cout << "y_G is not within the goal, therefore the goalie does not move." << endl;
                }
            }
        }

        if ((ball_t3.GetX() > 1.20)) {
            loop = false;
            cout << "Exiting loop because of ball position x: " << ball_t3.GetX() << endl;
        }
        if ((ball_t3.GetX() < -1.20)) {
            loop = false;
            cout << "Exiting loop because of ball position x: " << ball_t3.GetX() << endl;
        }
        if ((loop_countdown < MAX_LOOP_COUNTDOWN)) {
            loop = false;
            cout << "Exiting loop because of loop_countdown: " << loop_countdown << endl;
        }
        usleep(GOALIE_SLEEP_TIME * 1000000);
    }

    return 0;
}
 */
