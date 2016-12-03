#include "striker.h"
#include "game.h"

Striker::Striker(RTDBConn DBC_in, int device_nr_in, int robot_array_index, Position pos) : Robot(DBC_in, device_nr_in, robot_array_index, pos)
{

}

void Striker::do_a_shot_at_goal(bool is_left_side){
    Position shooting_pos;
    Position goal;

    // Find a random position in goal
    srand(time(NULL));

    double f = 0;
    while(f == 0){
        f = ((double)rand()) / (RAND_MAX);
    }
    goal.SetY(GOAL_MIN_YPOS+f*(GOAL_MAX_YPOS-GOAL_MIN_YPOS));
    std::cout << "goal y-coordinate: " << goal.GetY() << std::endl;

    // Set a flat line under the ball to shoot from
    if(is_left_side){
        goal.SetX(GOAL_RIGHT_XPOS);
        shooting_pos.SetX(Game::datBall->GetX() - 0.15);
    }
    else{
        goal.SetX(GOAL_LEFT_XPOS);
        shooting_pos.SetX(Game::datBall->GetX() + 0.15);
    }

    //Find equation for the line between ball and goal
    //y = ascent*x + constant
    double ascent = (goal.GetY()-Game::datBall->GetY())/(goal.GetX()-Game::datBall->GetX());
    double constant = Game::datBall->GetY() - ascent*Game::datBall->GetX();

    //find y-coordinate where estimation line and shooting line crosses
    shooting_pos.SetY(ascent*shooting_pos.GetX() + constant);


    //this->GotoXY(shooting_pos.GetX(), shooting_pos.GetY(), 160, true);
    this->GotoPos(shooting_pos);

    //wait until position is reached
    usleep(5000000);
    //while(this->GetMovingStatus() == STATUS_MOVING){ cout << "In while\n";}

    //Turn towards the ball and kick
    Angle angle_to_ball = GetPos().AngleOfLineToPos(Game::datBall->GetPos());

    usleep(spot_turn(angle_to_ball));
    usleep(1000000);

    // SHOOT THE BALL
    MoveMs(180, 180, 700, 0);
}

