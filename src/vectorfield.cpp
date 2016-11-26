//Might use some built-in Angle and Position functions instead
//This will be directly piped in to the reference_angle
//of the controller
//
Angle Vector::vector_angle{
	if(x == 0.0){ //crap preventative measures
		return 0.0;
	}

	return Angle(atan(y/x)-180);
}
	

//Operators
Vector Vector::operator+(const Vector& vec){
	return Vector(x+vec.get_x(), y+vec.get_y());
}
	
Vector Vector::operator*(const double& scale){
	return Vector(scale*x, scale*y);
}
	
Vector Vector::operator*=(const double& scale){
	x *= scale;
	y *= scale;

	return *this;	
}

double Vector::operator*(const Vector& vec){
	return x*vec.get_x() + y*vec.get_y();
}

};


//Might be called as an array/std::vector of Vector_fields taking in 
//the current robots position
//Vectors from each field is calculated and added together with
//the overloaded operator +=

Vector Robot_vector_field::vector_at_pos(Position pos){
		double x = (pos.GetX()-origo.GetX()) / 
				   (pow(fabs(origo.GetX()+pos.GetX()), 3));
		double y = (pos.GetY()-origo.GetY()) / 
				   (pow(fabs(origo.GetY()+pos.GetY()), 3));

		return Vector(x, y);
	}
};

Vector Wall_vector_field::vector_at_pos(Position pos){
	//Effect of the field shrinks with the higher the power.
	double x = (pos.GetX() - X_MIN_COOR)/
				pow(fabs(pos.GetX() - X_MIN_COOR), 3) +
 				(pos.GetX() - X_MIN_COOR)/
				pow(fabs(pos.GetX() - X_MIN_COOR), 3);

	double y = (pos.GetY() - Y_MIN_COOR)/
				pow(fabs(pos.GetY() - Y_MIN_COOR), 3) +
 				(pos.GetY() - Y_MIN_COOR)/
				pow(fabs(pos.GetY() - Y_MIN_COOR), 3);
		
	return Vector(x, y);
}


Vector Target_vector_field::vector_at_pos(Position pos){
	//Effect of the field shrinks with the higher the power.
	double x = scale*(origo.GetX() - pos.GetX())/
				pow(fabs(pos.GetX() - X_MIN_COOR), 3);

	double y = (pos.GetY() - Y_MIN_COOR)/
				pow(fabs(pos.GetY() - Y_MIN_COOR), 3) +
 				(pos.GetY() - Y_MIN_COOR)/
				pow(fabs(pos.GetY() - Y_MIN_COOR), 3);
		
	return Vector(x, y);
}

