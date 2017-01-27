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
    try {
        Game game_handler;
        game_handler.test_init();


        while(1){
            game_handler.test_loop();
        }


    } catch (DBError err) {
        cout << "Client died on Error: " << err.what() << endl;
    }
    cout << "End" << endl;
    return 0;
}
