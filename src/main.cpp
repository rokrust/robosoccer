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
            /* int i = 1;
            Timer utimer(100);
            utimer.enable();

            while(1){
                if(utimer.timeout()){
                    cout << "Timeout: " << i++ << " detected" << std::endl;
                    utimer.enable();
                }

            }
            cout << i; */



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
            Goalie myGoalie(DBC, myGoalieDvNr, &ball);
            Striker myStriker1(DBC, myStriker1DvNr, &ball);
            Striker myStriker2(DBC, myStriker1DvNr+1, &ball);
            Opponent theOpponent1(DBC, theOpponent1DvNr, &ball);
            Opponent theOpponent2(DBC, theOpponent1DvNr+1, &ball);
            Opponent theOpponent3(DBC, theOpponent1DvNr+2, &ball);


            // Create Game object
            Game game_handler(&ref_handler, is_team_blue, &ball,
                              &myGoalie, &myStriker1, &myStriker2,
                              &theOpponent1, &theOpponent2, &theOpponent3);

            game_handler.set_is_left_side(true);


            // Initialize a Test object
            Soccer_Tests Test_Obj(&myGoalie, &myStriker1, &myStriker2,
                                  &theOpponent1, &theOpponent2, &theOpponent3,
                                  &ball);


            //-------------------------------------- End Init ---------------------------------

            // select scenario
            int SCENARIO = 8;


            if (SCENARIO == 1) {
                Test_Obj.move_in_out();
            }

            if (SCENARIO == 2) {
                Test_Obj.const_wheel_speed();
            }

            if (SCENARIO == 3) {
                Test_Obj.easy_p_ctrl();
            }

            if (SCENARIO == 4) {
                Test_Obj.drive_to_pos();
            }

            if (SCENARIO == 5) {
                Test_Obj.test_turns();
            }

            // Goalie testing
            if (SCENARIO == 6) {
                Test_Obj.test_goalie();
            }

            if (SCENARIO == 7) {
                Test_Obj.turn_experiments();
            }

            if (SCENARIO == 8) {
                bool keep_running = 1;
                while (keep_running) {
                    game_handler.step();
                }
            }

            if (SCENARIO == 10) {
                game_handler.take_kick_off_position();
            }

            if (SCENARIO == 21) {
                game_handler.take_penalty_position();
                usleep(1000 * 1000);
            }


	} catch (DBError err) {
		cout << "Client died on Error: " << err.what() << endl;
	}
        cout << "End" << endl;
	return 0;
}


