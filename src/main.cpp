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


// Function declaration to call from Scenario
// Code is below main Function
void collision_avoidance_ball_tracking(Game& game_handler_in);
void collision_avoidance_corner_driving(Game& game_handler_in);
void test_controller(Game& game_handler_in);
void test_extrapol_shit(Game& game_handler_in);
void tactic_bring_ball_back_in_opp_field(Game& game_handler_in);
void test_via_pos_shit(Game& game_handler_in);
void test_striker_in_a_promising_position(Game& game_handler_in);




int main(void) {
    try {
        Game game_handler;
        game_handler.take_kick_off_positions();

        while(1){
            game_handler.test();
        }


    } catch (DBError err) {
        cout << "Client died on Error: " << err.what() << endl;
    }
    cout << "End" << endl;
    return 0;
}
