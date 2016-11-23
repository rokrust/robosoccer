#ifndef STRATEGY_H
#define STRATEGY_H

#include "game.h"

#define POSITION_HISTORY_LENGTH 3

class Strategy
{
private:
    // first index is goalie, striker1, striker2, opponent1, opponent2, opponent3
    // second index is how many time steps we go backward
    Position robot_position_history[6][POSITION_HISTORY_LENGTH];
    Position robot_velocity_estimation[6];
    Position robot_position_prediction[6];
    Position robot_last_prediction[6];

public:
    Strategy();

    // movement prediction and velocity estimation
    void update_position_history();
    void update_estimation_and_prediction(const double ms_between_positions);
    void print_robot_position_history(const int robot_nr);
    void print_robot_velocity_estimation();
    void print_robot_position_prediction();
};

#endif // STRATEGY_H
