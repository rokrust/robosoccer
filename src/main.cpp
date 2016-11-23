//============================================================================
// Name        : main.cpp
// Author      :
// Version     :
// Copyright   : (C)
// Description : Main Project File
//============================================================================


#include "game.h"
#include "robot.h"
#include "soccer_tests.h"
#include "kogmo_rtdb.hxx"
#include "robo_control.h"
#include "referee.h"
#include "timer.h"

#include <time.h>
#include <iostream>
#include <cmath>

using namespace std;


RawBall* Game::datBall = NULL;
Goalie* Game::goalie = NULL;
Striker* Game::striker1 = NULL;
Striker* Game::striker2 = NULL;
Opponent* Game::opponent1 = NULL;
Opponent* Game::opponent2 = NULL;
Opponent* Game::opponent3 = NULL;

Robot* Game::robots[6] = {0};


int main(void) {

    //--------------------------------- Init --------------------------------------------------

    /** Use client number according to your account number!
     *
     *	This is necessary in order to assure that there are unique
     *	connections to the RTDB.
     *
     */
    const int client_nr = 222;


    try {
        // Establish connection to the RTDB
        cout << endl << "Connecting to RTDB..." << endl;
        // Create the client name with the unique client number*/
        string client_name = "pololu_client_";
        client_name.push_back((char) (client_nr + '0'));
        RTDBConn DBC(client_name.data(), 0.1, "");

        // Create Referee Object
        /* Referee ref_handler(DBC);
        ref_handler.Init(); */

        // Get Robot Colour
        bool is_team_blue = true;
        cout << "Which Team? (0: red - 1: blue) ";
        cin >> is_team_blue;

        Game game_handler(DBC, is_team_blue);



        // Initialize a Test object
        /* Soccer_Tests Test_Obj(&myGoalie, &myStriker1, &myStriker2,
                              &theOpponent1, &theOpponent2, &theOpponent3); */


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
            game_handler.strategy_modul->command_drive();
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

        /* if (SCENARIO == 30){
            //Position pos(0.0, 0.0);

            int timer_duration = 250;
            Timer datTimer(timer_duration);

            while(1) {
                if (datTimer.timeout()) {
                    myStriker1.set_target_pos(Game::datBall->GetPos());
                    myStriker1.set_wheelspeed(timer_duration);
                }
            }
        }*/

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
