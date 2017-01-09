#ifndef GOALIE_H
#define GOALIE_H


#include "robot.h"

class Goalie : public Robot
{
private:
    const double GOALIE_SLEEP_TIME = 0.15;
    const int CALCULUS_RATE = 1;
    const int MAX_LOOP_COUNTDOWN = -500;
    const double V_X_THRESHOLD = 0.01;


public:
    Goalie(RTDBConn DBC_in, int device_nr_in, int robot_array_index);

    void do_a_penalty_save(RawBall *datBall, Robot *opponent1, Robot *opponent2, Robot *opponent3);

    void do_the_goalkeepers_kick(RawBall *datBall, Robot *opponent1, Robot *opponent2, Robot *opponent3);

};

#endif // GOALIE_H
