#ifndef STRIKER_H
#define STRIKER_H

#include "robot.h"

class Striker : public Robot
{
private:


public:
    Striker(RTDBConn DBC_in, int device_nr_in);
};

#endif // STRIKER_H