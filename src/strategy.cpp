#include "strategy.h"
#include "vectorfield.h"

#include <math.h>

Strategy::Strategy(Goalie* goalie_in, Striker* striker1_in, Striker* striker2_in,
                   Opponent* opponent1_in, Opponent* opponent2_in, Opponent* opponent3_in, RawBall* datBall_in, bool is_left_side_in)
{
    
    datBall = datBall_in;
    is_left_side = is_left_side_in;
}

Strategy::Strategy(Robot **robots_in, RawBall *datBall_in, bool is_left_side_in)
{
    
    datBall = datBall_in;
    is_left_side = is_left_side_in;
}

//Should find out which side of the ball the robot should be on
Position Strategy::determine_kick_position(Position target) {
	Position ball_pos = datBall.getPos(), kick_pos; //Probably wrong function name
	double a, b; //Linear parameters y = ax+b

	//To avoid a from going to infinity
	if(fabs(target.GetX() - ball_pos.GetX()) < 0.01){
		a = 0;
	}
	else {
		a = (target.GetY() - ball_pos->GetY()) / (target.GetX() - ball_pos->GetX());	
	}

	b = ball_pos->GetY() - a*ball_pos->GetX();
	
	kick_pos.setX(ball_pos.GetX() + KICK_DISTANCE);
	kick_pos.setY(a*kick_pos.GetX() + b);
	
	return kick_pos;
}

double Strategy::distance_of_point_to_line(Position point, Position line_start, Position line_end) {
	double x0 = point.GetX(), y0 = point.GetY();
	double x1 = line_start.GetX(), y1 = line_start.GetY();
	double x2 = line_end.GetX(), y2 = line_end.GetY();

	double distance = fabs((y2 - y1)*x0 - (x2 - x1)*y0 + x2*y1 - y2*x1) / sqrt(pow((y2 - y1), 2) + pow((x2 - x1), 2));

	return distance;
}

void Strategy::clear_scheduled_movement(int robot_index) {
	robot_target_positions[robot_index].clear();
}

void Strategy::clear_all_scheduled_movement(){
	for (int i = 0; i < N_ROBOTS; i++) {
		robot_target_positions[i].clear();
	}
}

bool* Strategy::robots_in_zone(Position top_left, Position bottom_right) {
	bool robot_is_in_zone[N_ROBOTS];

	for (int i = 0; i < N_ROBOTS; i++) {
		Position current_robot_position = robot_positions[i];
		
		if (current_robot_position.GetX() < top_left.GetX() &&
			current_robot_position.GetX() > bottom_right.GetX() &&
			current_robot_position.GetY() < top_left.GetY() &&
			current_robot_position.GetY() > bottom_right.GetY()) {

			robot_is_in_zone[i] = true;
		}

		else {
			robot_is_in_zone[i] = false;
		}
	}

	return robot_is_in_zone;
}

//Should determine if the robot needs a via pos to get behind the ball
//Find position to kick from, possibly add a via pos, push to target_pos vector
void Strategy::move_to_kick_position(int robot_index, Position target) {
	Position kick_pos = determine_kick_position(target);

	//add kick_pos to vector of target positions
	robot_target_positions[robot_index].push_back(kick_pos);

	add_via_position_if_necessary(kick_pos); //Might remove this function
}

//check y-coordinate distance between robots in opponents field half
bool Strategy::opening_detected() {
	bool *robots_blocking;

	if (is_left_side) {
		
	}

	else {

	}
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
		ball_to_kick.rotate(Angle angle(HALF_PI));

		Position via_pos = (Position)(ball_to_kick + to_ball);
		
		robot_target_positions[robot_index].push_back(via_pos);
	}
}




int Strategy::attack(int closest_striker_idx)
{
    cout << "Team is Attacking" << endl;

    int other_striker_idx = 2;
    if (closest_striker_idx == 2)
        other_striker_idx = 1;

    cout << "In attack: closest_striker_idx = " << closest_striker_idx << " and the other_striker_idx = " << other_striker_idx << endl;

    // WHERE TO SHOOT
    Position pos_in_goal_to_aim_for(0.0, 0.1);
    if (is_left_side) {
        pos_in_goal_to_aim_for.SetX(1.3);
    } else {
        pos_in_goal_to_aim_for.SetY(-1.3);
    }

    // WHERE TO SHOOT FROM
    Position ball_pos = datBall->GetPos();
    Position closest_striker_pos = robots[closest_striker_idx]->GetPos();

    Position direction_from_ball_to_goal(pos_in_goal_to_aim_for.GetX() - ball_pos.GetX() ,
                                         pos_in_goal_to_aim_for.GetY() - ball_pos.GetY());
    direction_from_ball_to_goal = norm_dat_vector(direction_from_ball_to_goal);

    Position pos_to_shoot_from(ball_pos.GetX() - 0.04 * direction_from_ball_to_goal.GetX() ,
                               ball_pos.GetY() - 0.04 * direction_from_ball_to_goal.GetY());


    cout << "closest_striker_pos = " << matlsynt(closest_striker_pos) << endl;
    cout << "ball pos = " << matlsynt(ball_pos) << endl;
    cout << "pos_in_goal = " << matlsynt(pos_in_goal_to_aim_for) << endl;
    cout << "pos_to_shoot_from = " << matlsynt(pos_to_shoot_from) << endl;


    robots[closest_striker_idx]->set_robot_target_pos(pos_to_shoot_from);

    // Other Striker going to origin
    robots[other_striker_idx]->set_robot_target_pos(Position(0.0, 0.0));

    cout << endl << endl;

    return 0;
}

int Strategy::defend()
{
    cout << "Team is Defending" << endl;
    return 0;
}

int Strategy::bring_ball_in_opponents_field(int closest_striker_idx)
{
    cout << "Bringing the Ball in Opponents field" << endl;

    int other_striker_idx = 2;
    if (closest_striker_idx == 2)
        other_striker_idx = 1;

    // Calculate a target position behind the ball
    Position ball_pos = datBall->GetPos();
    Position closest_striker_pos = robots[closest_striker_idx]->GetPos();
    Position normed_ball_pos = norm_dat_vector(ball_pos);
    Position target_pos_to_set(ball_pos.GetX() + 0.03 * normed_ball_pos.GetX() ,
                               ball_pos.GetY() + 0.03 * normed_ball_pos.GetY());
    /* Position target_pos_to_set = ball_pos;
    if (is_left_side) {
        target_pos_to_set.SetX(target_pos_to_set.GetX()-0.03); // Slightly behind the ball
    } else {
        target_pos_to_set.SetX(target_pos_to_set.GetX()+0.03); // Slightly behind the ball
    }
    if (ball_pos.GetY() >= 0.0) {
        target_pos_to_set.SetY(target_pos_to_set.GetY()+0.03); // Slightly diagonal to the ball so that the striker plays it into the middle of the field
    } else {
        target_pos_to_set.SetY(target_pos_to_set.GetY()-0.03); // Slightly diagonal to the ball so that the striker plays it into the middle of the field
    } */


    // Get Distance of ball to line between robot and target pos = length of perpendicualr
    Position line_robot_target_pos(target_pos_to_set.GetX()-closest_striker_pos.GetX(), target_pos_to_set.GetY()-closest_striker_pos.GetY());
    Position perpendicular_ball_on_line = drop_perpendicular_of_point_on_line(ball_pos, closest_striker_pos, line_robot_target_pos);
    double length_of_calced_perp = sqrt( pow(perpendicular_ball_on_line.GetX(),2) + pow(perpendicular_ball_on_line.GetY(),2) );

    cout << "Length_of_Perpendicular = " << length_of_calced_perp << endl;
    if (length_of_calced_perp <= 0.08) { // Ball is too close to the line between robot and its target Position, setting a via point then
        // Get the Via Pos that is far enough from the Ball
        double min_dist_of_robot_to_pass_the_ball = 0.25; // in m
        Position normed_perpendicular = norm_dat_vector(perpendicular_ball_on_line);

        Position via_pos_to_set(ball_pos.GetX() + min_dist_of_robot_to_pass_the_ball * normed_perpendicular.GetX(),
                                ball_pos.GetY() + min_dist_of_robot_to_pass_the_ball * normed_perpendicular.GetY());

        // SETTING THE PATH
        robots[closest_striker_idx]->set_robot_via_path(via_pos_to_set, target_pos_to_set);

        cout << "r_robot = " << matlsynt(closest_striker_pos) << endl;
        cout << "r_ball = " << matlsynt(ball_pos) << endl;
        cout << "r_targ = " << matlsynt(target_pos_to_set) << endl;
        cout << "r_via = " << matlsynt(via_pos_to_set) << endl;
    } else { // Ball is far enough from the line between robot and its target Position, setting no via point then

        // SETTING THE TARGET POSITION
        robots[closest_striker_idx]->set_robot_target_pos(target_pos_to_set);

        cout << "r_robot = " << matlsynt(closest_striker_pos) << endl;
        cout << "r_ball = " << matlsynt(ball_pos) << endl;
        cout << "r_targ = " << matlsynt(target_pos_to_set) << endl;
        cout << "r_via = r_targ; % via_pos = none" << endl;
    }

    // Other Striker going to origin
    robots[other_striker_idx]->set_robot_target_pos(Position(0.0, 0.0));

    cout << endl << endl;

    return 0;
}


// Determine if attack or defend respectively which kind of sick move to do
int Strategy::strat_move()
{
    // Determine which side of the Field the Ball is in
    bool ball_in_our_half = false;
    if (is_left_side) {
        if (datBall->GetX() >= 0.0) {
            ball_in_our_half = false;
            // cout << "Case 1" << endl;
        } else {
            ball_in_our_half = true;
            // cout << "Case 2" << endl;
        }
    } else {
        if (datBall->GetX() < 0.0) {
            ball_in_our_half = false;
            // cout << "Case 3" << endl;
        } else {
            ball_in_our_half = true;
            // cout << "Case 4" << endl;
        }
    }
    cout << setprecision(3) << fixed << showpos << "We play on the left side: " << is_left_side << " and the Ball is on our Half: " << ball_in_our_half << endl;

    // Get Robot closest to Ball
    int closest_robot_idx_of_all_robots = -1;
    int closest_own_robot_idx = -1;
    double closest_dist_of_all_robots = 10;
    double closest_dist_of_own_robots = 10;
    for (int i=0; i<6; i++) {
        double dist_robot_ball = datBall->GetPos().DistanceTo(robots[i]->GetPos());
        cout << "Curr Dist at Index " << i << " is " << dist_robot_ball << endl;
        if (dist_robot_ball < closest_dist_of_all_robots) {
            closest_dist_of_all_robots = dist_robot_ball;
            closest_robot_idx_of_all_robots = i;
        }
        if (closest_robot_idx_of_all_robots == 1 || closest_robot_idx_of_all_robots == 2) {
            closest_own_robot_idx = closest_robot_idx_of_all_robots;
            closest_dist_of_own_robots = closest_dist_of_all_robots;
        }
    }
    cout << "Closest robot with Index: " << closest_robot_idx_of_all_robots << " at a Distance of: " << closest_dist_of_all_robots << endl;
    cout << "Closest own robot with Index: " << closest_own_robot_idx << " at a Distance of: " << closest_dist_of_own_robots << endl;

    // WHAT TO DO?
    if (ball_in_our_half && ( closest_robot_idx_of_all_robots == 1 || closest_robot_idx_of_all_robots == 2 )) {
        bring_ball_in_opponents_field(closest_robot_idx_of_all_robots);
    }

    else if (ball_in_our_half && ( closest_robot_idx_of_all_robots != 1 || closest_robot_idx_of_all_robots != 2 )) {
        // defend();
        bring_ball_in_opponents_field(closest_own_robot_idx);
    }

    else {
        // Angriff ist die beste Verteidigung
        attack(closest_own_robot_idx);
    }

    return 0;
}

bool Strategy::striker_in_a_promising_position(Position striker_position, Position ball_position)
{
    // Maxs to be promising
    double diff_angle_max_to_be_promising = 20.0; // in deg
    double dist_ball_striker_max_to_be_promising = 0.20; // in m
    double dist_goal_ball_max_to_be_promising = 0.40; // in m

    // The Position where the opponent's goal is
    double goal_abs_xCoord = 1.40; // in m
    Position opp_goal_position(0.0, 0.0);
    if (is_left_side) {
        opp_goal_position.SetX(+goal_abs_xCoord);
    }
    else {
        opp_goal_position.SetX(-goal_abs_xCoord);
    }


    // dem Angles, using double because the Angle class doesnt work below or above +/- 180 degs
    double angle_goal_ball = opp_goal_position.AngleOfLineToPos(ball_position).getFloatDeg();
    if (angle_goal_ball < 0.0)
        angle_goal_ball = angle_goal_ball + 360.0;

    double angle_ball_striker = ball_position.AngleOfLineToPos(striker_position).getFloatDeg();
    if (angle_ball_striker < 0.0)
        angle_ball_striker = angle_ball_striker + 360.0;

    double diff_angle = angle_goal_ball - angle_ball_striker;

    // dat distance to the ball
    double dist_ball_striker = ball_position.DistanceTo(striker_position);

    // dat distance to the goal
    double dist_goal_ball = opp_goal_position.GetX() - ball_position.GetX();

    cout << "diff_angle = " << diff_angle << endl;
    cout << "dist_ball_striker = " << dist_ball_striker << endl;
    cout << "dist_goal_ball = " << dist_goal_ball << endl;

    // Determine if dat Position is promising
    bool position_is_promising = false;
    if (abs(diff_angle) <= diff_angle_max_to_be_promising &&
            (abs(dist_ball_striker) <= dist_ball_striker_max_to_be_promising &&
             abs(dist_goal_ball) <= dist_goal_ball_max_to_be_promising) )
    {
        position_is_promising = true;
    }

    cout << "position_is_promising = " << position_is_promising << endl << endl;
    return position_is_promising;
}

void Strategy::set_is_left_side(bool is_left_side_in)
{
    is_left_side = is_left_side_in;
}

bool Strategy::get_is_left_side()
{
    return is_left_side;
}

std::string Strategy::matlsynt(Position pos)
{
    int precision = 3; // after decimal point

    std::ostringstream stream_for_matlab_syntax;
    stream_for_matlab_syntax << "[" << setprecision(precision) << fixed << pos.GetX() << ", " << (double) pos.GetY() << "]; ";
    return stream_for_matlab_syntax.str();

    // Usage
    // Position pos2print(1, 0);
    // cout << "pos2print = " << game_handler.matlsynt(pos2print) << endl;
}

Position Strategy::drop_perpendicular_of_point_on_line(Position point_to_drop_perpendiular_from, Position line_foot_point, Position linear_direction_vector)
{
    // this function returns the perpendicular of a point on a line
    // length of this perpendicular is the distance of the point to the line
    // to get the point on the line, do: point + perpendicular

    // GET THE LINE FORMULA
    // Mathematics: line: y = mx + y0 -> mx + (-1)*y + y0 = ax + by + c = 0
    double a = linear_direction_vector.GetY() / linear_direction_vector.GetX();
    // double b = -1.0;
    double c = (line_foot_point.GetY()+linear_direction_vector.GetY()) -
            a * (line_foot_point.GetX()+linear_direction_vector.GetX());

    // DROP THE PERPENDICULAR
    // l is closest point for ball on line between robot and its target pos (the perpendicular)
    // this math was done on paper, to obtain dat formula
    double b_x = point_to_drop_perpendiular_from.GetX(); // pos x coord
    double b_y = point_to_drop_perpendiular_from.GetY(); // pos y coord
    double l_x = (b_x + a*b_y - a*c) / (pow(a,2) + 1);
    double l_y = c + (a*b_x + pow(a,2)*b_y - pow(a,2)*c) / (pow(a,2) + 1);

    Position the_perpendicular_to_return((l_x-b_x), (l_y-b_y));

    return the_perpendicular_to_return;
}

Position Strategy::norm_dat_vector(Position vector_to_norm)
{
    double length_of_vector = sqrt( pow(vector_to_norm.GetX(),2) + pow(vector_to_norm.GetY(),2) );
    Position normed_vector(vector_to_norm.GetX()/length_of_vector , vector_to_norm.GetY()/length_of_vector);
    return normed_vector;
}
