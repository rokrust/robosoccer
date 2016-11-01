#ifndef ROBOT_H
#define ROBOT_H

#include "kogmo_rtdb.hxx"
#include "robo_control.h"

class Robot : public RoboControl
{
private:
    int device_nr;

public:
    Robot(RTDBConn DBC_in, int device_nr_in);
    ~Robot();
};

#endif // ROBOT_H
