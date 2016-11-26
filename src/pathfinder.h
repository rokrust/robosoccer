#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "vectorfield.h"
#include "position.h"

#define WALL_FIELD 6
#define TARGET_FIELD 7
#define N_ROBOTS 6


class Path_finder{


private:
    std::vector<ateam::Vector_field*> vector_fields;
    Position target_pos;

public:
    Path_finder();
    Path_finder(Position target_pos = Position(0, 0));

    ateam::Vector sum_vector_field(Position current_pos);

    void set_target_pos(Position pos){target_pos = pos;}
    Position get_target_pos(){return target_pos;}
};

#endif // PATHFINDER_H
