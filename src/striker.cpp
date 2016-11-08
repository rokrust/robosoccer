#include "striker.h"

Striker::Striker(RTDBConn DBC_in, int device_nr_in, RawBall *datBall_in) : Robot(DBC_in, device_nr_in, datBall_in)
{

}

int Striker::shoot_penalty()
{
    Position pos_ball(datBall->GetPos());
    Position strikerPos(GetPos());

    double dist = abs(calc_dist(strikerPos, pos_ball));
    if (dist < 0.4) {
        GotoPos(pos_ball);
        cout << "Shoot" << endl;
    }
    else {
        cout << "Ball too far away" << endl;
    }

    return 0;
}

void Striker::shoot_ball_at_goal(bool is_left_side){
    Position shooting_pos;
    Position goal;

    //Find a random position in goal
    srand(time(NULL));

    double f = 0;
    while(f == 0){
        f = ((double)rand()) / (RAND_MAX);
    }
    goal.SetY(GOAL_MIN_YPOS+f*(GOAL_MAX_YPOS-GOAL_MIN_YPOS));

    //Set a flat line under the ball to shoot from
    if(is_left_side){
        goal.SetX(GOAL_RIGHT_XPOS);
        shooting_pos.SetX(datBall->GetX() - 0.15);
    }
    else{
        goal.SetX(GOAL_LEFT_XPOS);
        shooting_pos.SetX(datBall->GetX() + 0.15);
    }

    //Find equation for the line between ball and goal
    //y = ascent*x + constant
    double ascent = (goal.GetY()-datBall->GetY())/(goal.GetX()-datBall->GetX());
    double constant = datBall->GetY() - ascent*datBall->GetX();

    //find y-coordinate where estimation line and shooting line crosses
    shooting_pos.SetY(ascent*shooting_pos.GetX() + constant);

    this->GotoPos(shooting_pos);

    //wait until position is reached
    usleep(5000000);
    //while(this->GetMovingStatus() == STATUS_MOVING){ cout << "In while\n";}

    this->GotoPos(datBall->GetPos());
}

