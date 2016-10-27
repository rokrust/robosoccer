//============================================================================
// Name        : soccer_basics.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Client for RTDB which controls team A, Ansi-style
//============================================================================


#include <time.h>
#include <iostream>
#include "kogmo_rtdb.hxx"
#include "robo_control.h"


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

/*
        //Activate the project option "Run in terminal" in Ctrl+5 (Ctrl+2 to come back here)
        //to use "cin" function.

        char cInput;

        cout << "Specify rfcomm number of the robot you want to move. Blue 0-2, Red 3-5: ";
	while (1) {                
                cin >> cInput;
                if (!((cInput >= '0') && (cInput <= '7'))) {
			cout << "Please specify valid rfcomm number between 0 and 7"
					<< endl;
		} else {
                        rfcomm_nr = cInput-48; //simple "char to int" function
			break;
		}
        }
*/
	try {

                /** Establish connection to the RTDB. */
		cout << endl << "Connecting to RTDB..." << endl;
		/** Create the client name with the unique client number*/
		string client_name = "pololu_client_";
		client_name.push_back((char) (client_nr + '0'));
		RTDBConn DBC(client_name.data(), 0.1, "");

                /** Create RoboControl objects for all six robots */
                RoboControl robo0(DBC, 0);
                RoboControl robo1(DBC, 1);
                RoboControl robo2(DBC, 2);
                RoboControl robo3(DBC, 3);
                RoboControl robo4(DBC, 4);
                RoboControl robo5(DBC, 5);

                /*

                //Now let's print out some information about the robot...
                //uint8_t mac[6];
                //robo.GetMac(mac);
                cout << "Robo @ rfcomm" << rfcomm_nr <<" with Mac: ";


		for (int j = 0; j < 5; j++)
			cout << hex << (int) mac[j] << ":";
		cout << hex << (int) mac[5] << endl;

                cout << "\t Battery Voltage: " << dec << (int) robo.GetAccuVoltage()
                                << "mV" << endl;
		cout << "\t initial position: " << robo.GetPos() << endl;
		cout << "\t initial rotation: " << robo.GetPhi() << endl;

                */

		/** Create a ball object
		 *
		 *  This ball abject gives you access to all information about the ball
		 *  which is extracted from the cam.
		 *
		 */
		RawBall ball(DBC);
		/** lets print this information: */
		cout << "Ball informations:" << endl;
		cout << "\t initial position: " << ball.GetPos() << endl;
		/** Notice that the rotation here refers to the moving direction of the ball.
		 *  Therefore if the ball does not move the rotation is not defined.
		 */
		cout << "\t initial direction: " << ball.GetPhi() << endl;
		cout << "\t initial velocity: " << ball.GetVelocity() << endl;

		//-------------------------------------- Ende Init ---------------------------------

                /** Define outer positions for all six robots */
                Position posb0_out(-0.5, -0.6);
                Position posb1_out(0.0, -0.6);
                Position posb2_out(0.5, -0.6);
                Position posr0_out(-0.5, 0.6);
                Position posr1_out(0.0, 0.6);
                Position posr2_out(0.5, 0.6);

                /** Define inner positions for all six robots */
                Position posb0_in(-0.2, -0.1);
                Position posb1_in(0.0, -0.1);
                Position posb2_in(0.2, -0.1);
                Position posr0_in(-0.2, 0.1);
                Position posr1_in(0.0, 0.1);
                Position posr2_in(0.2, 0.1);

                int runflag = 1;
                int SCENARIO = 1;

                while (runflag) {

                        cout << "Moving all six robots to the outer positions" << endl;

                        robo0.GotoXY(posb0_out.GetX(), posb0_out.GetY(), 40, true);
                        robo1.GotoXY(posb1_out.GetX(), posb1_out.GetY(), 40, true);
                        robo2.GotoXY(posb2_out.GetX(), posb2_out.GetY(), 40, true);
                        robo3.GotoXY(posb0_out.GetX(), posr0_out.GetY(), 40, true);
                        robo4.GotoXY(posb1_out.GetX(), posr1_out.GetY(), 40, true);
                        robo5.GotoXY(posb2_out.GetX(), posr2_out.GetY(), 40, true);

                        usleep(6000000);
                        cout << "Moving all six robots to the inner positions" << endl;


                        robo0.GotoXY(posb0_out.GetX(), posb0_in.GetY(), 40, true);
                        robo1.GotoXY(posb1_out.GetX(), posb1_in.GetY(), 40, true);
                        robo2.GotoXY(posb2_out.GetX(), posb2_in.GetY(), 40, true);
                        robo3.GotoXY(posb0_out.GetX(), posr0_in.GetY(), 40, true);
                        robo4.GotoXY(posb1_out.GetX(), posr1_in.GetY(), 40, true);
                        robo5.GotoXY(posb2_out.GetX(), posr2_in.GetY(), 40, true);

                        runflag = 1;
                        usleep(6000000);

		}

	} catch (DBError err) {
		cout << "Client died on Error: " << err.what() << endl;
	}
        cout << "End" << endl;
	return 0;
}


