//============================================================================
// Name        : soccer_basics.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Client for RTDB which controls team A, Ansi-style
//============================================================================



#include "soccer_tests.h"
#include "kogmo_rtdb.hxx"
#include "robo_control.h"
#include "constants.h"
#include "drive_to_pos.h"

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

            /** Initialize a Test object */
            Soccer_Tests Test_Obj(&robo0, &robo1, &robo2,
                                  &robo3, &robo4, &robo5);

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



            Angle cur_phi;

            int runflag = 1;
            int SCENARIO = 2;

            if (SCENARIO == 1) {
                Test_Obj.move_in_out();
            }

            if (SCENARIO == 2) {
                Test_Obj.const_wheel_speed();
            }

            if (SCENARIO == 3) {

                int base_velo = 40;
                int ddeg, v_left, v_right;
                int goaldeg = 0;
                int run_ms = 200;
                int ramp_up = 0;

                while (runflag) {
                    /** Go to starting position and adjust orientation */
                    robo0.GotoXY(posstart.GetX(), posstart.GetY(), 40, true);
                    usleep(6000000);

                    cur_phi = robo0.GetPhi();
                    while (abs(cur_phi.Deg() - to_pc.Deg()) > 10) {
                        robo0.TurnAbs(to_pc);
                        cur_phi = robo0.GetPhi();
                        usleep(500000);
                    }

                    /** Drive while the distance is above a threshold */
                    while (abs(robo0.GetX() - posgoal.GetX()) > 0.1) {
                        cur_phi = robo0.GetPhi();
                        ddeg = goaldeg - cur_phi.Deg();
                        v_left = base_velo - (ddeg/2);
                        v_right = base_velo + (ddeg/2);

                        robo0.MoveMs(v_left, v_right, run_ms, ramp_up);

                    }

                    runflag = 0;
                }
            }

            if (SCENARIO == 4) {

                int r_value;

                while (runflag) {
                    r_value = drive_to_pos(robo0, posb0_out, 40, 6000000);
                    usleep(r_value);
                    r_value = drive_to_pos(robo0, posb0_in, 40, 6000000);
                    usleep(r_value);
                }


            }

            if (SCENARIO == 5) {

                int r_value;

                while (runflag) {
                    r_value = drive_to_pos(robo0, posstart, 40, 6000000);
                    usleep(r_value);
                    r_value = drive_to_pos(robo0, pos0, 40, 6000000);
                    usleep(r_value);
                    r_value = drive_to_pos(robo0, pos1, 40, 10000000);
                    usleep(r_value);
                    r_value = drive_to_pos(robo0, pos2, 40, 10000000);
                    usleep(r_value);
                    r_value = drive_to_pos(robo0, pos3, 40, 10000000);
                    usleep(r_value);
                    r_value = drive_to_pos(robo0, pos4, 40, 10000000);
                    usleep(r_value);
                    r_value = drive_to_pos(robo0, pos5, 40, 10000000);
                    usleep(r_value);
                    r_value = drive_to_pos(robo0, pos6, 40, 10000000);
                    usleep(r_value);

                    runflag = 0;
                }


            }

	} catch (DBError err) {
		cout << "Client died on Error: " << err.what() << endl;
	}
        cout << "End" << endl;
	return 0;
}


