#include "pathfinder.h"
#include "game.h"

//Constructors

Path_finder::Path_finder(int robots_array_index, Position pos){
    //this->target_pos = target_pos;
    cout << "In pathfinder constructor" << endl;

    for(int i = 0; i < N_ROBOTS; i++){
        ateam::Vector_field* robot_field = new ateam::Robot_vector_field(pos);
        vector_fields.push_back(robot_field);
		
        //Some weight. Should be parameters
        vector_field_weights.push_back(0.0); //fix hardcoding
    }

    cout << "Index: " << endl;
    vector_field_weights[robots_array_index] = 0;

    vector_fields.push_back(new ateam::Wall_vector_field());
    vector_field_weights.push_back(0.0); //fix hardcoding

    vector_fields.push_back(new ateam::Target_vector_field(target_pos));
    vector_field_weights.push_back(1.0); //fix hardcoding
    cout << "Out of pathfinder constructor" << endl;
}

//mathematics
ateam::Vector Path_finder::sum_vector_field(Position current_pos){
    ateam::Vector summed_weighted_vector; //Automatically set to zero vector

    for(unsigned int i = 0; i < vector_fields.size(); i++){
        cout << "x: " << vector_fields[i]->vector_at_pos(current_pos).get_x() << ", y: " << vector_fields[i]->vector_at_pos(current_pos).get_y() << endl;
        summed_weighted_vector += vector_fields[i]->vector_at_pos(current_pos) *
                                  vector_field_weights[i];
    }
    cout << "x: " << (summed_weighted_vector*1.0).get_x() << ", y: " << (summed_weighted_vector*1.0).get_y() << endl;
    return summed_weighted_vector;
}

void Path_finder::update_vector_field_positions(){
    for(int i = 0; i < N_ROBOTS; i++){
        //vector_fields[i]->set_center_point(Game::robot_positions[i]);
    }
}

void Path_finder::set_target_pos(Position pos){
    target_pos = pos;
    vector_fields[TARGET_FIELD]->set_center_point(pos);
}
