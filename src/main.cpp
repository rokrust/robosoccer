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
        const int client_nr = 2;


	try {
            // Establish connection to the RTDB
            cout << endl << "Connecting to RTDB..." << endl;
            // Create the client name with the unique client number*/
            string client_name = "pololu_client_";
            client_name.push_back((char) (client_nr + '0'));
            RTDBConn DBC(client_name.data(), 0.1, "");

            // Create Robot objects
            Robot blue1(DBC, 0);
            Robot blue2(DBC, 1);
            Robot blue3(DBC, 2);
            Robot red1(DBC, 3);
            Robot red2(DBC, 4);
            Robot red3(DBC, 5);

            // Create Referee Object
            Referee ref_handler(DBC);
            ref_handler.Init();

            // Create Game object
            Game game_handler(&ref_handler,
                              &blue1, &blue2, &blue3,
                              &red1, &red2, &red3);


            /*
            // Initialize a Test object
            Soccer_Tests Test_Obj(&blue1, &blue2, &blue3,
                                  &red1, &red2, &red3);
            */

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

            //-------------------------------------- End Init ---------------------------------

            bool keep_running = 1;
            while (keep_running) {
                game_handler.step();
            }

            /*
            // select scenario
            int SCENARIO = 4;

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

            if (SCENARIO == 6) {
                Test_Obj.turn_experiments();
            }
            */


	} catch (DBError err) {
		cout << "Client died on Error: " << err.what() << endl;
	}
        cout << "End" << endl;
	return 0;
}


