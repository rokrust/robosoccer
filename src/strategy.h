#ifndef STRATEGY_H
#define STRATEGY_H

#include "game.h"

#define POSITION_HISTORY_LENGTH 3
#define COLLISION_DISTANCE_THRESHOLD 0.3

// set rectangular field limits that are easy to check
#define FIELD_RECT_MAX_X 1.15
#define FIELD_RECT_MIN_X -1.15
#define FIELD_RECT_MAX_Y 0.9
#define FIELD_RECT_MIN_Y -0.9

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

    double collision_propability(const Position& pos_robot, const Position& pos_obstacle);
    bool position_within_field(const Position& pos_in);
    void check_and_handle_collisions();

    void command_drive();
};

#endif // STRATEGY_H
