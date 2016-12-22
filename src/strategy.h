#ifndef STRATEGY_H
#define STRATEGY_H

#include "goalie.h"
#include "striker.h"
#include "opponent.h"



#define POSITION_HISTORY_LENGTH 3
#define COLLISION_DISTANCE_THRESHOLD 0.2
#define COLLISION_CONSIDERATION_THESHOLD 0.2

// set rectangular field limits that are easy to check
#define FIELD_RECT_MAX_X 1.15
#define FIELD_RECT_MIN_X -1.15
#define FIELD_RECT_MAX_Y 0.9
#define FIELD_RECT_MIN_Y -0.9

#define VIA_POSITION_MAX_DIST 1.0

//Vector field indices
#define WALL_FIELD 6
#define TARGET_FIELD 7
#define N_ROBOTS 6

class Strategy {

private:
    Goalie* goalie;
    Striker* striker1;
    Striker* striker2;
    Opponent* opponent1;
    Opponent* opponent2;
    Opponent* opponent3;

    Robot* robots[6];
    RawBall* datBall;
    bool is_left_side;

    int attack(int closest_striker_idx);

    int defend();

    int bring_ball_in_opponents_field(int closest_striker_idx);


public:
    Strategy(){}

    Strategy(Goalie *goalie_in, Striker *striker1_in, Striker *striker2_in,
             Opponent *opponent1_in, Opponent *opponent2_in, Opponent *opponent3_in,
             RawBall *datBall_in, bool is_left_side_in);

    Strategy(Robot **robots_in, RawBall *datBall_in, bool is_left_side_in);

    int strat_move();

    void set_is_left_side(bool is_left_side_in);
    bool get_is_left_side();

    std::string matlsynt(Position pos);

    // Some Math Functions
    Position drop_perpendicular_of_point_on_line(Position point_to_drop_perpendiular_from, Position line_foot_point, Position linear_direction_vector);
    Position norm_dat_vector(Position vector_to_norm);

};



/*
class Strategy
{
private:
    // first index is goalie, striker1, striker2, opponent1, opponent2, opponent3
    // second index is how many time steps we go backward
    Position robot_position_history[6][POSITION_HISTORY_LENGTH];
    Position robot_velocity_estimation[6];
    Position robot_position_prediction[6];
    Position robot_last_prediction[6];

    Position robot_goal_positions[6];
    Position robot_extrapol_goal_positions[6];

    Position calculate_alternative_position(const Position& robot_pos, const Position& obstacle_pos);

public:
    Strategy();

    // movement prediction and velocity estimation
    void update_position_history();
    void update_estimation_and_prediction(const double ms_between_positions);
    void print_robot_position_history(const int robot_nr);
    void print_robot_velocity_estimation();
    void print_robot_position_prediction();

    // collision detection and handling
    double collision_propability(const Position& pos_robot, const Position& pos_obstacle);
    bool position_within_field(const Position& pos_in);
    void check_and_handle_collisions();

    // driving
    void update_extrapol_goal_position();
    void update_extrapol_goal_position_per_robot(const int robot_nr);
    Position extrapol_goal_position(const Position &cur_pos, const Position &goal_pos);
    void set_goal_pos(const Position& goal_pos, const int robot_nr);

    // development functions
    void command_drive();
};
*/
#endif // STRATEGY_H
