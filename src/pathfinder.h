#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "vectorfield.h"
#include "position.h"

#define WALL_FIELD 6
#define TARGET_FIELD 7
#define N_ROBOTS 6

class Path_finder{


private:
	//Used to calculate reference heading with collision avoidance
    std::vector<ateam::Vector_field*> vector_fields;
	std::vector<double> vector_field_weights;

    Position target_pos;

    ateam::Vector sum_vector_field(Position current_pos);
    void update_vector_field_positions(Position* robot_positions);

public:
    Path_finder(){}
    Path_finder(int robots_array_index);

    Angle calculate_reference_angle(int current_pos_index, Position* robot_positions);

    void set_target_pos(Position pos);
    Position get_target_pos();

	void set_robot_vector_field_weight(int robot_index, double weight);
};

#endif // PATHFINDER_H
