#include "pathfinder.h"
#include "game.h"

//Constructors
Path_finder::Path_finder(){
    for(int i = 0; i < N_ROBOTS; i++){
        vector_fields.push_back(new ateam::Robot_vector_field(Game::robots[i]->GetPos()));
    }

    vector_fields.push_back(new ateam::Wall_vector_field());
    vector_fields.push_back(new ateam::Target_vector_field());


}

Path_finder::Path_finder(Position target_pos){
    this->target_pos = target_pos;

    for(int i = 0; i < N_ROBOTS; i++){
        vector_fields.push_back(new ateam::Robot_vector_field(Game::robots[i]->GetPos()));
    }

    vector_fields.push_back(new ateam::Wall_vector_field());
    vector_fields.push_back(new ateam::Target_vector_field(target_pos));


//mathematics
ateam::Vector Path_finder::sum_vector_field(Position current_pos){
    ateam::Vector summed_vector; //Automatically set to zero vector

    for(unsigned int i = 0; i < vector_fields.size(); i++){
        //put if here to skip robots own vector field

        summed_vector += vector_fields[i]->vector_at_pos(current_pos);
    }

    return summed_vector;
}
