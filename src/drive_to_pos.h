//============================================================================
// Name        : drive_to_pos.h
// Author      : Simon B. Gasse
// Version     :
// Copyright   : (C)
// Description : Basic driving function header file
//============================================================================

#ifndef DRIVE_TO_POS
#define DRIVE_TO_POS

#include "kogmo_rtdb.hxx"
#include "robo_control.h"

#include <time.h>
#include <iostream>
#include <cmath>


int drive_to_pos(RoboControl &robo, Position &pos, int velo, int move_time);

#endif
