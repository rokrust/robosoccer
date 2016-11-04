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
