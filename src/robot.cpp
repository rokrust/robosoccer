#include "robot.h"

Robot::Robot(RTDBConn DBC_in, int device_nr_in) : RoboControl(DBC_in, device_nr_in)
{
    device_nr = device_nr_in;
}

Robot::~Robot()
{

}
