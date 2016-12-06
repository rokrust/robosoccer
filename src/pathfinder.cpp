#include "pathfinder.h"
#include "game.h"

//Constructors

Path_finder::Path_finder(int robots_array_index, Position* robot_positions){
    target_pos = robot_positions[robots_array_index];

    for(int i = 0; i < N_ROBOTS; i++){
        ateam::Vector_field* robot_field = new ateam::Robot_vector_field(robot_positions[i]);
        vector_fields.push_back(robot_field);
		
        //Some weight. Should be parameters
        vector_field_weights.push_back(0.0); //fix hardcoding  0.005
    }

    //Current robot's own position
    vector_field_weights[robots_array_index] = 0;

    vector_fields.push_back(new ateam::Wall_vector_field());
    vector_field_weights.push_back(0.0); //fix hardcoding  0.001

    vector_fields.push_back(new ateam::Target_vector_field(target_pos));
    vector_field_weights.push_back(1.0); //fix hardcoding  1.0
}

//mathematics
ateam::Vector Path_finder::sum_vector_field(Position* robot_positions){
    ateam::Vector summed_weighted_vector(0, 0);

    for(unsigned int i = 0; i < vector_fields.size(); i++){

        //Skip the robot's personal vector field
        if(vector_field_weights[i] == 0){
            continue;
        }

        summed_weighted_vector += vector_field_weights[i] * vector_fields[i]->vector_at_pos(current_pos);
    }

    return summed_weighted_vector;
}

Angle Path_finder::calculate_reference_angle(int current_pos_index, Position* robot_positions){
    update_vector_field_positions(robot_positions);
    return sum_vector_field(robot_positions[current_pos_index]).vector_angle();

}

void Path_finder::update_vector_field_positions(Position* robot_positions){
    for(int i = 0; i < N_ROBOTS; i++){
        vector_fields[i]->set_center_point(robot_positions[i]);
    }
}

void Path_finder::set_target_pos(Position pos){
    target_pos = pos;
    vector_fields[TARGET_FIELD]->set_center_point(pos);
}

Position Path_finder::get_target_pos(){
    return target_pos;
}
