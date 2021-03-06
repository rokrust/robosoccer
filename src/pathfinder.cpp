#include "pathfinder.h"


//Constructors
//Initializes vector field weights and vector field positions
/**
 * @brief Constructor of the path_finder object. Sets the vector field weights determining the "strength" of each vector field.
 *
 * @param robot_array_index Position in the robot array used to reference the robots
 */
Path_finder::Path_finder(int robot_array_index){

    for(int i = 0; i < N_ROBOTS; i++){
        ateam::Vector_field* robot_field = new ateam::Robot_vector_field();
        vector_fields.push_back(robot_field);
		
        //Initialize all the robot vector field weights
        vector_field_weights.push_back(0.5);
    }

    //The robot's own vectorfield is ignored to avoid infinite values
    vector_field_weights[robot_array_index] = 0;

    //Initialize wall vector field weights
    vector_fields.push_back(new ateam::Wall_vector_field());
    vector_field_weights.push_back(0.1);

    //Initialize target vector field weight
    vector_fields.push_back(new ateam::Target_vector_field());
    vector_field_weights.push_back(1.0);
}

/**
 * @brief Mostly used for debugging. Prints the length of the vector at pos in the field given by index.
 *
 * @param index Index of the vector field
 * @param pos Position in vector field
 */
void Path_finder::print_vector_length(int index, Position pos){
    ateam::Vector current_vec = vector_fields[index]->vector_at_pos(pos);


    cout << "Length: " << current_vec*current_vec << endl;

}


//mathematics
//Adds all the vector fields to find and returns the vector that
//the robot should follow.
/**
 * @brief Adds all the vector fields to find and returns the vector that the robot should follow.
 *
 * @param current_pos Position to calculate the vectors at
 * @return ateam::Vector
 */
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
/**
 * @brief Finds the angle of the vector from the summed vector field
 *
 * @param current_pos_index Index of robot's personal position
 * @param robot_positions Array that keeps track of all the robot positions
 * @return Angle
 */
Angle Path_finder::calculate_reference_angle(int current_pos_index, Position* robot_positions){
    update_vector_field_positions(robot_positions);

    ateam::Vector direction_vector = sum_vector_field(robot_positions[current_pos_index]);
    Angle heading = direction_vector.vector_angle();

    cout << current_pos_index << "Angle: " << heading.Deg() << endl;
    cout << current_pos_index << "Length: " << direction_vector.length() << endl << endl;


    return heading;

}

//Changes the center point of the robot vector fields
/**
 * @brief Changes the center point of the robot vector fields
 *
 * @param robot_positions Array that keeps track of all the robot positions
 */
void Path_finder::update_vector_field_positions(Position* robot_positions){
    for(int i = 0; i < N_ROBOTS; i++){
        vector_fields[i]->set_center_point(robot_positions[i]);
    }
}

//Changes the center point of the target vector field
/**
 * @brief Changes the center point of the target vector field
 *
 * @param pos Position that is set
 */
void Path_finder::set_target_pos(Position pos){

    target_pos = pos;

    vector_fields[TARGET_FIELD]->set_center_point(pos);

    //Needed to allign the fields towards the robot's target position
    for(int i = 0; i < N_ROBOTS; i++){
        vector_fields[i]->set_target_pos(pos);
    }
}

/**
 * @brief
 *
 * @return Position
 */
Position Path_finder::get_target_pos(){
    return target_pos;
}

/**
 * @brief Changes the weight of the robot field given by robot_index.
 *
 * @param robot_index Index of the field we want to change
 * @param weight The value we want to change it to
 */
void Path_finder::set_robot_vector_field_weight(int robot_index, double weight){
	vector_field_weights[robot_index] = weight;
}
