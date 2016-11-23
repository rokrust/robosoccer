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
        Referee ref_handler(DBC);
        ref_handler.Init();

        // Get Robot Colour
        bool is_team_blue = true;
        cout << "Which Team? (0: red - 1: blue) ";
        cin >> is_team_blue;

        // Get Side
        // bool is_left_side = ref_handler.GetSide(); // 0: left side (Opp. PCs) - 1: right side (PCs)

        // cout << "My Team is blue: " << is_team_blue << " - Playing on the side Opp. PCs: " << is_left_side << endl;

        /** Create a ball object
             *
             *  This ball abject gives you access to all information about the ball
             *  which is extracted from the cam.
             *
             */
        RawBall ball(DBC);
        cout << "Ball informations:" << endl;
        cout << "\t initial position: " << ball.GetPos() << endl;
        /** Notice that the rotation here refers to the moving direction of the ball.
             *  Therefore if the ball does not move the rotation is not defined.
             */
        cout << "\t initial direction: " << ball.GetPhi() << endl;
        cout << "\t initial velocity: " << ball.GetVelocity() << endl;

        // derive bot numbers from the team color
        int myGoalieDvNr;
        int myStriker1DvNr;
        int theOpponent1DvNr;
        if (is_team_blue) {
            myGoalieDvNr = 0;
            myStriker1DvNr = 1;
            theOpponent1DvNr = 3;
        }
        else {
            myGoalieDvNr = 3;
            myStriker1DvNr = 4;
            theOpponent1DvNr = 0;
        }

        // Create Robot objects
        Goalie myGoalie(DBC, myGoalieDvNr);
        Striker myStriker1(DBC, myStriker1DvNr);
        Striker myStriker2(DBC, myStriker1DvNr+1);
        Opponent theOpponent1(DBC, theOpponent1DvNr);
        Opponent theOpponent2(DBC, theOpponent1DvNr+1);
        Opponent theOpponent3(DBC, theOpponent1DvNr+2);


        // Create Game object
        Game game_handler(&ref_handler, is_team_blue, &ball,
                          &myGoalie, &myStriker1, &myStriker2,
                          &theOpponent1, &theOpponent2, &theOpponent3);



        // Initialize a Test object
        Soccer_Tests Test_Obj(&myGoalie, &myStriker1, &myStriker2,
                              &theOpponent1, &theOpponent2, &theOpponent3);


        //-------------------------------------- End Init ---------------------------------

        int SCENARIO;
        cout << "Select scenario..." << endl;
        cout << "Play scenarios:" << endl;
        cout << "1: Run the state machine" << endl;
        cout << "2: Run the goalie ball stopping scenario" << endl;
        cout << "3: Run the striker penalty shooting scenario" << endl;
        cout << "Development and debug scenarios:" << endl;
        cout << "4: Run the old state machine" << endl;
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
            myStriker1.shoot_ball_at_goal(game_handler.get_is_left_side());
        }

        if (SCENARIO == 4) {
            bool keep_running = 1;
            while (keep_running) {
                game_handler.step();
            }
        }

        if (SCENARIO == 5) {
            Position pos2print(1, 0);
            cout << "pos2print = " << game_handler.matlsynt(pos2print) << endl;

            Test_Obj.test_command_driving();
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
                    game_handler.update_position_history();
                    game_handler.update_estimation_and_prediction(time_step_size);
                    game_handler.print_robot_position_history(3);
                    //game_handler.print_robot_velocity_estimation();
                }
            }
        }

        if (SCENARIO == 99) {
            game_handler.update_position_history();
        }

        if (SCENARIO == 100) {
            Test_Obj.move_in_out();
        }

        if (SCENARIO == 400) {
            Test_Obj.drive_to_pos();
        }

        if (SCENARIO == 700) {
            Test_Obj.turn_experiments();
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
            //Position pos(0.0, 0.0);

            int timer_duration = 250;
            Timer datTimer(timer_duration);

            while(1) {
                if (datTimer.timeout()) {
                    myStriker1.set_target_pos(Game::datBall->GetPos());
                    myStriker1.set_wheelspeed(timer_duration);
                }
            }
        }
                /*
                    //Angle ang = myStriker1.GetPos().AngleOfLineToPos(Position());

                while(1){
                    //				if(robot.controller_timer.timeout()){
                    //					robot.set_wheelspeed();
                } */


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
