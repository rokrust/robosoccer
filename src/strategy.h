#ifndef STRATEGY_H
#define STRATEGY_H

#include "goalie.h"
#include "striker.h"
#include "opponent.h"
#include "robot.h"
#include "timer.h"

#include <vector>

#define COLLISION_DISTANCE_THRESHOLD 0.2
#define COLLISION_CONSIDERATION_THESHOLD 0.2
#define KICK_DISTANCE 0.15

//rectangular field limits
#define FIELD_RECT_MAX_X 1.15
#define FIELD_RECT_MIN_X -1.15
#define FIELD_RECT_MAX_Y 0.9
#define FIELD_RECT_MIN_Y -0.9


#define VIA_POSITION_MAX_DIST 1.0

#define GOALIE 0
#define STRIKER1 1
#define STRIKER2 2
#define OPPONENT1 3
#define OPPONENT2 4
#define OPPONENT3 5
#define N_ROBOTS 6

//Tactics:
//	Take advantage of an opening
//	Block an opponent

//Strategy:
//	Play defensively if in the lead
//	Play aggressively if score is equal or less
class Strategy {

private:
    
    Robot** robots;
	Position robot_positions[N_ROBOTS];
	double robot_estimated_speeds[N_ROBOTS]; //Might not be used

    //Used to set via positions for the robots
    vector<Position> robot_target_positions[N_ROBOTS];

    RawBall* datBall;
    Timer movement_tick_timer;

    bool is_left_side;


	Position determine_kick_position(Position target);
	double distance_of_point_to_line(Position point, Position line_start, Position line_end);
	void add_via_position_if_necessary(int robot_index, Position kick_pos);
	void clear_scheduled_movement(int robot_index);
	void clear_all_scheduled_movement();
    void update_robot_positions();



public:

    Strategy(){}
    Strategy(Robot **robots, RawBall *datBall, bool is_left_side); //ugh

    void set_avoidance_degree(int robot, int robot_to_avoid, double avoidance_degree);

    void move_robots();

	//Set new target position for both robots based on position of opponents
    void pass_ball(int passing_robot_index, int recieving_robot_index);
	void move_to_kick_position(int robot_index, Position target);

    void set_is_left_side(bool is_left_side);
    bool get_is_left_side();
};


#endif
