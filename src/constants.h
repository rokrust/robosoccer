#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "kogmo_rtdb.hxx"
#include "robo_control.h"

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

/** Define other position for robot 0 */
Position posstart(-0.7, 0.0);
Position posgoal(0.7, 0.0);

Position pos0(0.7, 0.0);
Position pos1(0.7, 0.2);
Position pos2(0.7, 0.7);
Position pos3(0.2, 0.7);
Position pos4(-0.2, 0.7);
Position pos5(-0.7, 0.7);
Position pos6(-0.7, 0.0);

/** Define orientations */
Angle to_pc(0);
Angle to_wall(180);

#endif // CONSTANTS_H
