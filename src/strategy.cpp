#include "strategy.h"
#include "vectorfield.h"

#include <math.h>


Strategy::Strategy(Robot **robots, RawBall *datBall, bool is_left_side)
{
    this->robots = robots;
    this->datBall = datBall;
    this->is_left_side = is_left_side;

    this->movement_tick_timer = Timer(100); //Fix hard coding
}

//Should find out which side of the ball the robot should be on
Position Strategy::determine_kick_position(Position target) {
    Position ball_pos = datBall->GetPos(), kick_pos; //Probably wrong function name
	double a, b; //Linear parameters y = ax+b

	//To avoid a from going to infinity
	if(fabs(target.GetX() - ball_pos.GetX()) < 0.01){
		a = 0;
	}
	else {
        a = (target.GetY() - ball_pos.GetY()) / (target.GetX() - ball_pos.GetX());
	}

    b = ball_pos.GetY() - a*ball_pos.GetX();
	
    kick_pos.SetX(ball_pos.GetX() + KICK_DISTANCE);
    kick_pos.SetY(a*kick_pos.GetX() + b);
	
	return kick_pos;
}

double Strategy::distance_of_point_to_line(Position point, Position line_start, Position line_end) {
	double x0 = point.GetX(), y0 = point.GetY();
	double x1 = line_start.GetX(), y1 = line_start.GetY();
	double x2 = line_end.GetX(), y2 = line_end.GetY();

	double distance = fabs((y2 - y1)*x0 - (x2 - x1)*y0 + x2*y1 - y2*x1) / sqrt(pow((y2 - y1), 2) + pow((x2 - x1), 2));

	return distance;
}

void Strategy::update_robot_positions(){
    for(int i = 0; i < N_ROBOTS; i++){
        robot_positions[i] = robots[i]->GetPos();
    }
}

void Strategy::clear_scheduled_movement(int robot_index) {
	robot_target_positions[robot_index].clear();
}

void Strategy::clear_all_scheduled_movement(){
	for (int i = 0; i < N_ROBOTS; i++) {
		robot_target_positions[i].clear();
	}
}

//Should determine if the robot needs a via pos to get behind the ball
//Find position to kick from, possibly add a via pos, push to target_pos vector
void Strategy::move_to_kick_position(int robot_index, Position target) {
	Position kick_pos = determine_kick_position(target);

	//add kick_pos to vector of target positions
	robot_target_positions[robot_index].push_back(kick_pos);

    add_via_position_if_necessary(robot_index, kick_pos); //Might remove this function
}


//Pass the ball from one robot to another
void Strategy::pass_ball(int passing_robot_index, int recieving_robot_index) {
}

void Strategy::add_via_position_if_necessary(int robot_index, Position kick_pos) {
	Position current_pos = robot_positions[robot_index];

	//Check if kick_pos is on the other side of the ball or not
	if (current_pos.DistanceTo(kick_pos) < current_pos.DistanceTo(datBall->GetPos())) {
		return;
	}


	double kick_pos_perpendicular_distance = distance_of_point_to_line(kick_pos, current_pos, datBall->GetPos());

	//Via position necessary
	if (kick_pos_perpendicular_distance < KICK_DISTANCE) {
		ateam::Vector to_ball(datBall->GetPos());
		ateam::Vector to_kick(kick_pos);
		ateam::Vector ball_to_kick = to_ball - to_kick;
		
		//via pos is rotated around the ball
        ball_to_kick.rotate(Angle(M_PI/2));

		Position via_pos = (Position)(ball_to_kick + to_ball);
		
		robot_target_positions[robot_index].push_back(via_pos);
	}
}


void Strategy::set_is_left_side(bool is_left_side_in)
{
    is_left_side = is_left_side_in;
}

bool Strategy::get_is_left_side()
{
    return is_left_side;
}

void Strategy::set_avoidance_degree(int robot, int robot_to_avoid, double avoidance_degree){
    robots[robot]->set_avoidance_degree(robot_to_avoid, avoidance_degree);
}


void Strategy::move_robots(){
    update_robot_positions();
    //robots[GOALIE]->set_target_pos(robot_positions[STRIKER1]);

    if(movement_tick_timer.timeout()){
        movement_tick_timer.enable();

        for(int i = 0; i < 3; i++){
            robots[i]->set_wheelspeed(robot_positions);
        }

    }
}
