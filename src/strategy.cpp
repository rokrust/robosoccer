#include "strategy.h"


Strategy::Strategy(Goalie* goalie_in, Striker* striker1_in, Striker* striker2_in,
                   Opponent* opponent1_in, Opponent* opponent2_in, Opponent* opponent3_in, RawBall* datBall_in, bool is_left_side_in)
{
    goalie = goalie_in;
    striker1 = striker1_in;
    striker2 = striker2_in;
    opponent1 = opponent1_in;
    opponent2 = opponent2_in;
    opponent3 = opponent3_in;

    robots[0] = goalie;
    robots[1] = striker1;
    robots[2] = striker2;
    robots[3] = opponent1;
    robots[4] = opponent2;
    robots[5] = opponent3;

    datBall = datBall_in;
    is_left_side = is_left_side_in;
}

Strategy::Strategy(Robot **robots_in, RawBall *datBall_in, bool is_left_side_in)
{
    robots[0] = *(robots_in);
    robots[1] = *(robots_in + 1);
    robots[2] = *(robots_in + 2);
    robots[3] = *(robots_in + 3);
    robots[4] = *(robots_in + 4);
    robots[5] = *(robots_in + 5);

    goalie = static_cast<Goalie*>(robots[0]);
    striker1 = static_cast<Striker*>(robots[1]);
    striker2 = static_cast<Striker*>(robots[2]);
    opponent1 = static_cast<Opponent*>(robots[3]);
    opponent2 = static_cast<Opponent*>(robots[4]);
    opponent3 = static_cast<Opponent*>(robots[5]);

    datBall = datBall_in;
    is_left_side = is_left_side_in;
}


int Strategy::attack()
{
    cout << "Team is Attacking" << endl;
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
    Position target_pos_to_set = ball_pos;
    if (is_left_side) {
        target_pos_to_set.SetX(target_pos_to_set.GetX()-0.08); // Slightly behind the ball
    } else {
        target_pos_to_set.SetX(target_pos_to_set.GetX()+0.08); // Slightly behind the ball
    }
    if (ball_pos.GetY() >= 0.0) {
        target_pos_to_set.SetY(target_pos_to_set.GetY()+0.03); // Slightly diagonal to the ball so that the striker plays it into the middle of the field
    } else {
        target_pos_to_set.SetY(target_pos_to_set.GetY()-0.03); // Slightly diagonal to the ball so that the striker plays it into the middle of the field
    }

    // Get Distance of ball to line between robot and target pos (mx + (-1)*y + y0 = ax + by + c = 0)
    Position direction_robot_target_pos(target_pos_to_set.GetX()-closest_striker_pos.GetX(), target_pos_to_set.GetY()-closest_striker_pos.GetY());
    double a = direction_robot_target_pos.GetY() / direction_robot_target_pos.GetX();
    double b = -1.0;
    double c = target_pos_to_set.GetY() - a * target_pos_to_set.GetX();

    double dist = abs(a*ball_pos.GetX() + b*ball_pos.GetY() + c) / sqrt(pow(a,2) + pow(b,2)); // d = abs(a*x + by + c)/sqrt(a*a+b*b) - see wikipedia: Distance from a point to a line
    cout << "dist = " << dist << endl;

    if (dist <= 0.08) { // Ball is too close to the line between robot and its target Position, setting a via point then
        // l is closest point for ball on line between robot and its target pos
        // this math was done on paper, to obtain dat formula
        double b_x = ball_pos.GetX(); // Balls x coord
        double b_y = ball_pos.GetY(); // Balls y coord
        double l_x = (b_x + a*b_y - a*c) / (pow(a,2) + 1);
        double l_y = c + (a*b_x + pow(a,2)*b_y - pow(a,2)*c) / (pow(a,2) + 1);
        Position direction_ball_to_line((l_x-b_x) / dist, (l_y-b_y) / dist); // normed direction from ball to closest point on line between robot and target pos

        double min_dist_of_robot_to_pass_the_ball = 0.25; // in m

        Position via_pos_to_set(b_x + min_dist_of_robot_to_pass_the_ball * direction_ball_to_line.GetX(),
                                b_y + min_dist_of_robot_to_pass_the_ball * direction_ball_to_line.GetY());

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
    bool ball_in_our_half= false;
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
    int closest_robot_idx = -1;
    double closest_dist = 10;
    for (int i=0; i<6; i++) {
        double dist_robot_ball = datBall->GetPos().DistanceTo(robots[i]->GetPos());
        // cout << "Curr Dist at Index " << i << " is " << dist_robot_ball << endl;
        if (dist_robot_ball < closest_dist) {
            closest_dist = dist_robot_ball;
            closest_robot_idx = i;
        }
    }
    cout << "Closest robot with Index: " << closest_robot_idx << " at a Distance of: " << closest_dist << endl;


    if (ball_in_our_half && ( closest_robot_idx == 1 || closest_robot_idx == 2 )) {
        bring_ball_in_opponents_field(closest_robot_idx);
    }
    else if (ball_in_our_half && ( closest_robot_idx != 1 || closest_robot_idx != 2 )) {
        // defend();
        bring_ball_in_opponents_field(closest_robot_idx);
    }
    else {
        // attack();
        bring_ball_in_opponents_field(closest_robot_idx);
    }

    return 0;
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


/*
Strategy::Strategy()
{
    int robot_index, history_index;
    for (robot_index = 0; robot_index < 6; robot_index++) {
        robot_last_prediction[robot_index].SetX(0.0);
        robot_last_prediction[robot_index].SetY(0.0);
        robot_position_prediction[robot_index].SetX(0.0);
        robot_position_prediction[robot_index].SetY(0.0);
        robot_velocity_estimation[robot_index].SetX(0.0);
        robot_velocity_estimation[robot_index].SetY(0.0);
        robot_goal_positions[robot_index].SetX(0.0);
        robot_goal_positions[robot_index].SetY(0.0);
        robot_extrapol_goal_positions[robot_index].SetX(0.0);
        robot_extrapol_goal_positions[robot_index].SetY(0.0);
        for (history_index = 0; history_index < POSITION_HISTORY_LENGTH; history_index++) {
            robot_position_history[robot_index][history_index].SetX(0.0);
            robot_position_history[robot_index][history_index].SetY(0.0);
        }
    }
}

Position Strategy::calculate_alternative_position(const Position &robot_pos, const Position &obstacle_pos)
{
    // TODO: Actually calculate alternative positions
    Position pos_dummy(0.0, 0.0);
    return pos_dummy;
}

void Strategy::update_estimation_and_prediction(const double ms_between_positions)
{
    int robot_index, history_index;
    double dist_x, dist_y;

    for (robot_index = 0; robot_index <= 5; robot_index++) {
        // velocity estimation
        dist_x = 0.0;
        dist_y = 0.0;
        // POSITION_HISTORY_LENGTH - 1 because of index shift internally
        for (history_index = 0; history_index < POSITION_HISTORY_LENGTH - 1; history_index++) {
            dist_x = dist_x + robot_position_history[robot_index][history_index].GetX()
                    - robot_position_history[robot_index][history_index+1].GetX();
            dist_y = dist_y + robot_position_history[robot_index][history_index].GetY()
                    - robot_position_history[robot_index][history_index+1].GetY();
        }
        robot_velocity_estimation[robot_index].SetX(dist_x /
                                                    ((ms_between_positions / 1000)* (double)(POSITION_HISTORY_LENGTH - 1)));
        robot_velocity_estimation[robot_index].SetY(dist_y /
                                                    ((ms_between_positions / 1000) * (double)(POSITION_HISTORY_LENGTH - 1)));

        // save old prediction
        robot_last_prediction[robot_index].SetX(robot_position_prediction[robot_index].GetX());
        robot_last_prediction[robot_index].SetY(robot_position_prediction[robot_index].GetY());

        // calculate new prediction
        robot_position_prediction[robot_index].SetX(robot_position_history[robot_index][0].GetX()
                + (ms_between_positions / 1000)*robot_velocity_estimation[robot_index].GetX());
        robot_position_prediction[robot_index].SetY(robot_position_history[robot_index][0].GetY()
                + (ms_between_positions / 1000)*robot_velocity_estimation[robot_index].GetY());
    }
}

void Strategy::update_position_history()
{
    int robot_index, history_index;

    for (robot_index = 0; robot_index <= 5; robot_index++) {
        for (history_index = POSITION_HISTORY_LENGTH - 1; history_index >= 1; history_index--) {
            //cout << "robot_index: " << robot_index << " history_index: " << history_index << endl;
            robot_position_history[robot_index][history_index] = robot_position_history[robot_index][history_index-1];
        }
        robot_position_history[robot_index][0] = Game::robots[robot_index]->GetPos();
    }
}

void Strategy::print_robot_position_history(const int robot_nr)
{
    int precision = 3;
    cout << "Robot position history for robot" << robot_nr << endl;
    int history_index;
    for (history_index = 0; history_index < POSITION_HISTORY_LENGTH; history_index++) {
        cout << "Position at t-" << history_index << " x: " << setprecision(precision) << fixed
             << robot_position_history[robot_nr][history_index].GetX() << " y: "
             << robot_position_history[robot_nr][history_index].GetY() << endl;
    }
}

void Strategy::print_robot_velocity_estimation()
{
    int precision = 3;
    cout << "Robot velocity estimation in m/s" << endl;
    cout << "Goalie    x: " << setprecision(precision) << fixed
         << robot_velocity_estimation[0].GetX() << " y: " << robot_velocity_estimation[0].GetY() << endl;
    cout << "Striker1  x: " << setprecision(precision) << fixed
         << robot_velocity_estimation[1].GetX() << " y: " << robot_velocity_estimation[1].GetY() << endl;
    cout << "Striker2  x: " << setprecision(precision) << fixed
         << robot_velocity_estimation[2].GetX() << " y: " << robot_velocity_estimation[2].GetY() << endl;
    cout << "Opponent1 x: " << setprecision(precision) << fixed
         << robot_velocity_estimation[3].GetX() << " y: " << robot_velocity_estimation[3].GetY() << endl;
    cout << "Opponent2 x: " << setprecision(precision) << fixed
         << robot_velocity_estimation[4].GetX() << " y: " << robot_velocity_estimation[4].GetY() << endl;
    cout << "Opponent3 x: " << setprecision(precision) << fixed
         << robot_velocity_estimation[5].GetX() << " y: " << robot_velocity_estimation[5].GetY() << endl;
}

void Strategy::print_robot_position_prediction()
{
    cout << "Robot position prediction" << endl;
    int robot_index;
    for (robot_index = 0; robot_index <= 5; robot_index++) {
        cout << "Robot" << robot_index << " predicted x: " << robot_last_prediction[robot_index].GetX()
                << " y: " << robot_last_prediction[robot_index].GetY() << endl;
        cout << "Robot" << robot_index << " current   x: " << robot_position_history[robot_index][0].GetX()
                << " y: " << robot_position_history[robot_index][0].GetY() << endl;
        cout << "Robot" << robot_index << " error     x: "
             << robot_position_history[robot_index][0].GetX() - robot_last_prediction[robot_index].GetX()
             << " y: "
             << robot_position_history[robot_index][0].GetY() - robot_last_prediction[robot_index].GetY()
             << endl;
    }
}

double Strategy::collision_propability(const Position &pos_robot, const Position &pos_obstacle)
{
    double dist = pos_robot.DistanceTo(pos_obstacle);
    if (dist > COLLISION_DISTANCE_THRESHOLD) {
        return 0.0;
    } else {
        // COLLISION_DISTANCE_THRESHOLD -> 1.0
        // 0.0 -> 1.0
        return (1.0 - (dist / COLLISION_DISTANCE_THRESHOLD));
    }
}

bool Strategy::position_within_field(const Position &pos_in)
{
    // quick check if position is within the rectangular field
    if ((pos_in.GetX() < FIELD_RECT_MAX_X)
            && (pos_in.GetX() > FIELD_RECT_MIN_X)
            && (pos_in.GetY() < FIELD_RECT_MAX_Y)
            && (pos_in.GetY() > FIELD_RECT_MIN_Y)) {
        return true;
    } else {
        // TODO: sophisticated check
        return false;
    }
}

void Strategy::check_and_handle_collisions()
{
    bool collisions[3] = {true, true, true};
    int collision_with[3] = {-1, -1, -1};
    int robot_index;
    int obstacle_index;

    while(collisions[0] || collisions[1] || collisions[2]) {
        collisions[0] = false;
        collisions[1] = false;
        collisions[2] = false;
        // First check whether a robot would be in a forbidden zone,
        // such as the wall or the penalty zone
        for (robot_index = 0; robot_index <= 2; robot_index++) {
            // iterate over our own robots, we cannot handle collisions among the others
            if (!!position_within_field(robot_position_prediction[robot_index])) {
                collisions[robot_index] = true;
                collision_with[robot_index] = -2; // wall collision
            }

            // Second check the distances among the robots and if there is a collision propability
            // above a certain threshold, if so, calculate alternative positions until there are
            // no collisions
            for (obstacle_index = 0; obstacle_index < 6; obstacle_index++) {
                // prevent to check self-collision
                if (obstacle_index != robot_index) {
                    double col_prob = collision_propability(robot_position_prediction[robot_index],
                                                            robot_position_prediction[obstacle_index]);
                    if (col_prob > COLLISION_CONSIDERATION_THESHOLD) {
                        collisions[robot_index] = true;
                        collision_with[robot_index] = obstacle_index;
                    }
                }
            }
        }


        for (robot_index = 0; robot_index <= 2; robot_index++) {
            if (collisions[robot_index]) {
                // handle collisions
                Position obstacle_pos;
                //Position robot_pos_cur = robot_position_history[robot_index][0];
                if (collision_with[robot_index] == -2) {
                    obstacle_pos = robot_position_prediction[robot_index];
                } else {
                    obstacle_pos = robot_position_prediction[collision_with[robot_index]];
                }
                cout << "Collision danger detected for robot " << robot_index << " with robot " << collision_with[robot_index] << endl;


                These two lines should be commented out
                Position alternative_pos = calculate_alternative_position(robot_pos_cur, obstacle_pos);
                robot_position_prediction[robot_index] = alternative_pos;

            }
        }

        // Breakout
        break;
    }

    // TODO: Assign alternative positions OR DO NOTHING
}

void Strategy::update_extrapol_goal_position()
{
    int robot_index;
    for (robot_index = 0; robot_index < 3; robot_index++) {
        Position cur_pos = Game::robots[robot_index]->GetPos();
        Position goal_pos = robot_goal_positions[robot_index];

        if ((cur_pos.DistanceTo(goal_pos) > VIA_POSITION_MAX_DIST)) {
            Position via_pos = extrapol_goal_position(cur_pos, goal_pos);
            Game::robots[robot_index]->set_target_pos(via_pos);
        } else {
            Game::robots[robot_index]->set_target_pos(goal_pos);
        }
    }
}

void Strategy::update_extrapol_goal_position_per_robot(const int robot_nr)
{
    Position cur_pos = Game::robots[robot_nr]->GetPos();
    Position goal_pos = robot_goal_positions[robot_nr];

    if ((cur_pos.DistanceTo(goal_pos) > VIA_POSITION_MAX_DIST)) {
        Position extrapol_goal_pos = extrapol_goal_position(cur_pos, goal_pos);
        Game::robots[robot_nr]->set_target_pos(extrapol_goal_pos);
        // cout << "EP\t";
    } else {
        Game::robots[robot_nr]->set_target_pos(goal_pos);
        // cout << "nEP\t";
    }
}

Position Strategy::extrapol_goal_position(const Position& cur_pos, const Position& goal_pos)
{
    double length = goal_pos.DistanceTo(cur_pos);
    Position direction_to_goal_pos((goal_pos.GetX() - cur_pos.GetX()) / length, (goal_pos.GetY() - cur_pos.GetY()) / length);

    Position via_pos((cur_pos.GetX() + VIA_POSITION_MAX_DIST * direction_to_goal_pos.GetX()),
                     (cur_pos.GetY() + VIA_POSITION_MAX_DIST * direction_to_goal_pos.GetY()));

    return via_pos;
}

void Strategy::set_goal_pos(const Position &goal_pos, const int robot_nr)
{
    robot_goal_positions[robot_nr] = goal_pos;
    update_extrapol_goal_position_per_robot(robot_nr);
}

void Strategy::command_drive()
{
    double x_in, y_in;
    int robot_choice_in;

    while(true) {
        Robot* my_robot;
        cout << "Select robot to control:" << endl;
        cout << "1: Goalie" << endl;
        cout << "2: Striker1" << endl;
        cout << "3: Striker2" << endl;
        cout << "4: Opponent1" << endl;
        cout << "5: Opponent2" << endl;
        cout << "6: Opponent3" << endl;
        cin >> robot_choice_in;
        switch (robot_choice_in) {
        case 1: my_robot = Game::goalie; break;
        case 2: my_robot = Game::striker1; break;
        case 3: my_robot = Game::striker2; break;
        case 4: my_robot = Game::opponent1; break;
        case 5: my_robot = Game::opponent2; break;
        case 6: my_robot = Game::opponent3; break;
        default: cout << "Wrong input. Setting default to goalie" << endl;
            my_robot = Game::goalie; break;
        }

        cout << "X: " << endl;
        cin >> x_in;
        cout << "Y: " << endl;
        cin >> y_in;
        Position goal_pos(x_in, y_in);
        cout << "Going to position x:" << x_in << " y:" << y_in << endl;
        my_robot->drive_to_pos(goal_pos, true);
        cin.get();
    }
}
*/
