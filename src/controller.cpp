#define K_ph 10

class Position;
class Angle;


void Robot::move_to_pos(Position pos, int speed){
	//Might update this every so often with a timer
	Angle ref_heading = GetPos().AngleOfLineToPos(pos);;
	Angle current_heading = ;
	int integratedDistance = 0;	

	//Set initial speed
	left_wheel_speed = speed;
	right_wheel_speed = speed;
	MoveMs(left_wheel_speed, right_wheel_speed, run_time, TURN_RAMP_UP);
	

	/*--------------Should be called every now and then-----------*/	
	//Heading controller
	adjust_wheel_speed(K_ph*(ref_heading - current_heading));
	
	//Translational controller. sampling_time is defined elsewhere
	integratedDistance += GetPos().distance(pos) * sampling_time;
	int wheel_speed = K_pt(GetPos().distance(pos)) + 
					  K_it(integratedDistance));
	left_wheel_speed += wheel_speed;
	right_wheel_speed += wheel_speed

	//Might have to change the last two arguments
	MoveMs(left_wheel_speed, right_wheel_speed, run_time, TURN_RAMP_UP);
	/*------------------------------------------------------------*/

}

//adjust turning
void Robot::adjust_wheel_diff(int input){
	//might have to change the order of these
	if(input > 0){
		left_wheel_speed += input;
		right_wheel_speed -= input;
	}
	else{
		left_wheel_speed -= input;
		right_wheel_speed += input;
	}
	
	//Might have to change the last two arguments
	MoveMs(left_wheel_speed, right_wheel_speed, run_time, TURN_RAMP_UP);

}


