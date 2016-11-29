#include "pathfinder.h"
#include "game.h"

//Constructors

Path_finder::Path_finder(int robots_array_index, Position pos){
    target_pos = pos;

    for(int i = 0; i < N_ROBOTS; i++){
        ateam::Vector_field* robot_field = new ateam::Robot_vector_field(pos);
        vector_fields.push_back(robot_field);
		
        //Some weight. Should be parameters
        vector_field_weights.push_back(0.005); //fix hardcoding  0.005
    }

    cout << "Index: " << endl;
    vector_field_weights[robots_array_index] = 0;

    vector_fields.push_back(new ateam::Wall_vector_field());
    vector_field_weights.push_back(0.001); //fix hardcoding  0.001

    vector_fields.push_back(new ateam::Target_vector_field(target_pos));
    vector_field_weights.push_back(1.0); //fix hardcoding  1.0
}

//mathematics
ateam::Vector Path_finder::sum_vector_field(Position current_pos){
    ateam::Vector summed_weighted_vector(0, 0); //Automatically set to zero vector

    cout << endl;
    for(unsigned int i = 0; i < vector_fields.size(); i++){
        //Skip the robot's personal vector field
        if(vector_field_weights[i] == 0){
            continue;
        }

        summed_weighted_vector += vector_field_weights[i] * vector_fields[i]->vector_at_pos(current_pos);
    }

    /*
    // calculate resulting vector
    ateam::Vector v_o = summed_weighted_vector;
    double scale = -((v_o * vector_fields[TARGET_FIELD]->vector_at_pos(current_pos)) / pow(sqrt(pow(v_o.get_x(), 2) + pow(v_o.get_y(), 2)), 2));
    ateam::Vector v_diff = v_o + scale*v_o;

    ateam::Vector v_res = vector_fields[TARGET_FIELD]->vector_at_pos(current_pos) + v_diff;

    return v_res; */
    return summed_weighted_vector;
}

void Path_finder::update_vector_field_positions(){
    for(int i = 0; i < N_ROBOTS; i++){
        vector_fields[i]->set_center_point(Game::robot_positions[i]);
    }
}

void Path_finder::set_target_pos(Position pos){
    target_pos = pos;
    vector_fields[TARGET_FIELD]->set_center_point(pos);
}
