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
	std::vector<double> vector_field_weights;
    //Position robot_current_positions[N_ROBOTS];

    Position target_pos;
	int robots_array_index;

public:
    Path_finder(){}
    Path_finder(int robots_array_index, Position pos);

    ateam::Vector sum_vector_field(Position current_pos);
    void update_vector_field_positions();

    void set_target_pos(Position pos);
    Position get_target_pos(){return target_pos;}
};

#endif // PATHFINDER_H
