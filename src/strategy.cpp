#include "strategy.h"

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
        for (history_index = 0; history_index < POSITION_HISTORY_LENGTH; history_index++) {
            robot_position_history[robot_index][history_index].SetX(0.0);
            robot_position_history[robot_index][history_index].SetY(0.0);
        }
    }
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

void Strategy::check_and_handle_collisions()
{

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
