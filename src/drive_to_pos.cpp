//============================================================================
// Name        : soccer_basics.cpp
// Author      : Simon B. Gasse
// Version     :
// Copyright   : (C)
// Description : Basic driving function
//============================================================================


#include <time.h>
#include <iostream>
#include "kogmo_rtdb.hxx"
#include "robo_control.h"
#include <cmath>


void drive_to_pos(RoboControl &robo, Position &pos, int velo) {
    robo.GotoXY(pos.GetX(), pos.GetY(), velo, true);
}


