#include "pathfinder.h"
#include "game.h"

//Constructors
Path_finder::Path_finder(){

	//Robot vector fields
    for(int i = 0; i < N_ROBOTS; i++){
        vector_fields.push_back(new ateam::Robot_vector_field
		(Game::robots[i]->GetPos()));
		vector_field_weights.push_back(0.0);
    }

	//Wall vector field    
	vector_fields.push_back(new ateam::Wall_vector_field());
	vector_field_weights.push_back(0.0);
    
	//Target position vector field
	vector_fields.push_back(new ateam::Target_vector_field());
	vector_field_weights.push_back(0.0);

}

Path_finder::Path_finder(Position target_pos, int robos_array_index){
    this->target_pos = target_pos;

    for(int i = 0; i < N_ROBOTS; i++){
        vector_fields.push_back(new ateam::Robot_vector_field
								(Game::robots[i]->GetPos()));
		
		//Some weight. Should be parameters
		vector_field_weights.push_back(10.0);
    }

	vector_field_weights[robot_array_index] = 0;

    vector_fields.push_back(new ateam::Wall_vector_field());
	vector_field_weights.push_back(10.0);

    vector_fields.push_back(new ateam::Target_vector_field(target_pos));
	vector_field_weights.push_back(10.0);
}

//mathematics
ateam::Vector Path_finder::sum_vector_field(Position current_pos){
    ateam::Vector summed_weighted_vector; //Automatically set to zero vector

    for(unsigned int i = 0; i < vector_fields.size(); i++){

        summed_weighted_vector += vector_fields_weights[i]*
						vector_fields[i]->vector_at_pos(current_pos);
    }

    return summed_weighted_vector;
}
