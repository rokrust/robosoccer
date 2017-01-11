#include "pathfinder.h"


//Constructors
//Initializes vector field weights and vector field positions
Path_finder::Path_finder(int robot_array_index){

    for(int i = 0; i < N_ROBOTS; i++){
        ateam::Vector_field* robot_field = new ateam::Robot_vector_field();
        vector_fields.push_back(robot_field);
		
        //Initialize all the robot vector field weights
        vector_field_weights.push_back(0.0); //fix hardcoding  0.005
    }

    //The robot's own vectorfield is ignored to avoid infinite values
    vector_field_weights[robot_array_index] = 0;

    //Initialize wall vector field weights
    vector_fields.push_back(new ateam::Wall_vector_field());
    vector_field_weights.push_back(0.0); //fix hardcoding  0.001

    //Initialize target vector field weight
    vector_fields.push_back(new ateam::Target_vector_field());
    vector_field_weights.push_back(1.0); //fix hardcoding  1.0
}

//mathematics
//Adds all the vector fields to find and returns the vector that
//the robot should follow.
ateam::Vector Path_finder::sum_vector_field(Position current_pos){
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

//Finds the angle of the vector from the summed vector field
Angle Path_finder::calculate_reference_angle(int current_pos_index, Position* robot_positions){
    update_vector_field_positions(robot_positions);
    return sum_vector_field(robot_positions[current_pos_index]).vector_angle();

}

//Changes the center point of the robot vector fields
void Path_finder::update_vector_field_positions(Position* robot_positions){
    for(int i = 0; i < N_ROBOTS; i++){
        vector_fields[i]->set_center_point(robot_positions[i]);
    }
}

//Changes the center point of the target vector field
void Path_finder::set_target_pos(Position pos){
    target_pos = pos;
    vector_fields[TARGET_FIELD]->set_center_point(pos);
}

Position Path_finder::get_target_pos(){
    return target_pos;
}

void Path_finder::set_robot_vector_field_weight(int robot_index, double weight){
	vector_field_weights[robot_index] = weight;
}
