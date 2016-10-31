//============================================================================
// Name        : drive_to_pos.h
// Author      : Simon B. Gasse
// Version     :
// Copyright   : (C)
// Description : Basic driving function header file
//============================================================================


#include <time.h>
#include <iostream>
#include "kogmo_rtdb.hxx"
#include "robo_control.h"
#include <cmath>


void drive_to_pos(RoboControl &robo, Position &pos, int velo);
