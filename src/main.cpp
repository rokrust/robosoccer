//============================================================================
// Name        : main.cpp
// Author      :
// Version     :
// Copyright   : (C)
// Description : Main Project File
//============================================================================


#include "game.h"
#include "robot.h"
#include "kogmo_rtdb.hxx"
#include "robo_control.h"
#include "referee.h"
#include "timer.h"

#include <time.h>
#include <iostream>
#include <cmath>

using namespace std;



int main(void) {

    //--------------------------------- Init --------------------------------------------------
    const int client_nr = 222;

    try {
        // Establish connection to the RTDB
        cout << endl << "Connecting to RTDB..." << endl;
        // Create the client name with the unique client number*/
        string client_name = "pololu_client_";
        client_name.push_back((char) (client_nr + '0'));
        RTDBConn DBC(client_name.data(), 0.1, "");

        // Get Robot Colour
        bool is_team_blue = true;
        cout << "Which Team? (0: red - 1: blue) ";
        cin >> is_team_blue;

        Game game_handler(DBC, is_team_blue);

        //-------------------------------------- End Init ---------------------------------

        int SCENARIO;
        cout << "Select scenario..." << endl;
        cout << "Play scenarios:" << endl;
        cout << "1: Run the state machine" << endl;
        cout << "2: Run the goalie ball stopping scenario" << endl;
        cout << "3: Run the striker penalty shooting scenario" << endl;
        cout << "Development and debug scenarios:" << endl;
        cout << "5: Run the manual robot to position drive function" << endl;
        cout << "6: Run the goalie drive parallel test loop" << endl;
        cout << "7: Run the goalkeepers kick scenario" << endl;
        cout << "8: Run the side and kick-off check function" << endl;
        cout << "9: Run the prediction and velocity estimation scenario" << endl;
        cout << "...or enter another scenario number known to you" << endl;
        cout << "SCENARIO = ";
        cin >> SCENARIO;

        // select scenario

        if (SCENARIO == 1) {
            game_handler.state_machine(true);
        }

        if (SCENARIO == 2) {
            game_handler.goalie->GotoPos(Position(-1.3, 0.0));
            usleep(4000 * 1000);
            game_handler.goalie->spot_turn(90);
            game_handler.goalie->go_to_penalty_save_position();
        }

        if (SCENARIO == 3){
            Game::striker1->shoot_ball_at_goal(game_handler.get_is_left_side());
        }

        if (SCENARIO == 5) {
            //game_handler.strategy_modul->command_drive();
        }

        if (SCENARIO == 6) {
            game_handler.goalie->test_loop_drive_parallel();
        }

        // Goalkeepers Kick
        if (SCENARIO == 7) {
            game_handler.goalie->do_the_goalkeepers_kick();
        }


        if (SCENARIO == 8) {
            while(1) {
                game_handler.update_side();
                game_handler.update_kick_off();
                cout << "----" << endl;
                cout << "is_left_side: " << game_handler.get_is_left_side() << endl;
                cout << "has_kick_off: " << game_handler.get_has_kick_off() << endl;
                cin.get();
            }
        }
/*
        if (SCENARIO == 9) {
            cout << "Prediction and estimation testing" << endl;
            int time_step_size;
            cout << "Type desired time step size in ms: ";
            cin >> time_step_size;
            Timer test_timer(time_step_size);
            while(1) {
                if(test_timer.timeout()) {
                    game_handler.strategy_modul->update_position_history();
                    game_handler.strategy_modul->update_estimation_and_prediction(time_step_size);
                    game_handler.strategy_modul->print_robot_position_history(0);
                    game_handler.strategy_modul->print_robot_position_history(1);
                    game_handler.strategy_modul->print_robot_position_history(2);
                    game_handler.strategy_modul->print_robot_position_history(3);
                    game_handler.strategy_modul->print_robot_position_history(4);
                    game_handler.strategy_modul->print_robot_position_history(5);
                    game_handler.strategy_modul->print_robot_velocity_estimation();
                    game_handler.strategy_modul->print_robot_position_prediction();
                }
            }
        }
*/
        // bigger collision avoidance debugging scenario
        if (SCENARIO == 10) {
            Position goal_left(-1.3, 0.0);
            Position goal_right(1.3, 0.0);
            Position left_middle(0.0, 0.4);
            bool is_left = true;
            bool can_turn = true;

            Game::striker2->get_path_finder().set_target_pos(left_middle);

            cout << "Collision Avoidance Scenario" << endl;
            int time_step_strategy, time_step_driving;
            cout << "Please enter the time step size for the strategy, including estimation, prediction and collision avoidance" << endl;
            cout << "time_step_strategy = ";
            cin >> time_step_strategy;
            cout << "Please enter the time step size for the robot driving" << endl;
            cout << "time_step_driving = ";
            cin >> time_step_driving;
            Timer strategy_timer(time_step_strategy, 10);
            Timer goalie_timer(time_step_driving, 0);
            Timer striker1_timer(time_step_driving, 1);
            Timer striker2_timer(time_step_driving, 2);
            Timer goalie_switch_timer(8000, 2);
            while(1) {
                // Strategy tasks
/*
                if (strategy_timer.timeout()) {
                    game_handler.strategy_modul->update_position_history();
                    game_handler.strategy_modul->update_estimation_and_prediction(time_step_strategy);
                    game_handler.strategy_modul->check_and_handle_collisions();
                }
*/
                // Goal position switching
                // goalie alternates between the two goals in a ten seconds pace
                if (goalie_switch_timer.timeout()) {
                    if (is_left) {
                        Game::goalie->get_path_finder().set_target_pos(goal_left);
                        can_turn = true;
                        is_left = false;
                    } else {
                        Game::goalie->get_path_finder().set_target_pos(goal_right);
                        can_turn = true;
                        is_left = true;
                    }
                }

                // Driving
                if (goalie_timer.timeout()) {
                    Angle goal_phi = Game::goalie->GetPos().AngleOfLineToPos(Game::goalie->get_path_finder().get_target_pos());
                    int ddeg = Game::goalie->calc_ddeg(goal_phi);
                    if (abs(ddeg) > 60 && can_turn) {
                        int wait_time = Game::goalie->spot_turn(goal_phi);
                        goalie_timer.enable_manually(wait_time);
                        can_turn = false;

                        cout << "Goalie turned on the spot" << endl;
                    } else {
                        Game::goalie->set_wheelspeed(time_step_driving);
                    }
                }

                if (striker1_timer.timeout()) {
                    // striker1 follows the ball
                    Game::striker1->get_path_finder().set_target_pos(Game::datBall->GetPos());
                    Game::striker1->set_wheelspeed(time_step_driving);

                }

                if (striker2_timer.timeout()) {
                    // striker2 stays on one spot
                    Game::striker2->set_wheelspeed(time_step_driving);
                }
            }
        }


        if (SCENARIO == 1000) {
            game_handler.take_kick_off_position();
        }

        if (SCENARIO == 21) {
            game_handler.take_penalty_position();
            usleep(1000 * 1000);
        }

        if (SCENARIO == 22) {
            game_handler.goalie->drive_parallel(+10, false);
        }

        if (SCENARIO == 30){
            int timer_duration = 250;
            Timer datTimer(timer_duration);

            while(1) {
                if (datTimer.timeout()) {
                    Game::striker1->get_path_finder().set_target_pos(Game::datBall->GetPos());
                    Game::striker1->set_wheelspeed(timer_duration);
                }
            }
        }

        if (SCENARIO == 31){
            int timer_duration = 250;
            Timer datTimer(timer_duration);

            while(1) {
                if (datTimer.timeout()) {
                    //Position cur_pos = Game::striker1->GetPos();
                    //Position goal_pos = Game::datBall->GetPos();
//                    Position via_pos = game_handler.strategy_modul->extrapol_goal_position(cur_pos, goal_pos);

//                    Game::striker1->set_target_pos(via_pos);
                    Game::striker1->set_wheelspeed(timer_duration);
                }
            }
        }

        if (SCENARIO == 32){
            int timer_duration = 250;
            Timer datTimer(timer_duration);

            while(1) {
                game_handler.update_robot_positions();

                if (datTimer.timeout()) {
 //                   game_handler.strategy_modul->set_goal_pos(Game::datBall->GetPos(), 1);
                    Game::striker1->set_wheelspeed(timer_duration);
                }
            }
        }

        if (SCENARIO == 33){
            int timer_duration = 250;
            Timer datTimer(timer_duration);

            while(1) {
                game_handler.update_robot_positions();
                Game::striker1->get_path_finder().set_target_pos(Game::datBall->GetPos());
                cout << "x: " << Game::striker1->GetX() << "y: ", Game::striker1->GetY() << endl;
                if (datTimer.timeout()) {
 //                   game_handler.strategy_modul->set_goal_pos(Game::datBall->GetPos(), 1);
                    Game::striker1->set_wheelspeed(timer_duration);
                }
            }

        }

        if (SCENARIO == 123456) {
            int timer_duration;
            cout << "Type desired timer_duration in ms: ";
            cin >> timer_duration;
            Timer test_timer(timer_duration);
            while(1) {
                if(test_timer.timeout())
                    cout << "timed out" << endl;
            }
        }

    } catch (DBError err) {
        cout << "Client died on Error: " << err.what() << endl;
    }
    cout << "End" << endl;
    return 0;
}
