#include "striker.h"

Striker::Striker(RTDBConn DBC_in, int device_nr_in, RawBall *datBall_in) : Robot(DBC_in, device_nr_in, datBall_in)
{

}

Striker::shoot_penalty()
{
    Position pos_ball(datBall->GetPos());

    GotoPos(pos_ball);

    return 0;
}
