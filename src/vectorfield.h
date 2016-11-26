#ifndef VECTORFIELD_H
#define VECTORFIELD_H

#include "position.h"


#include <math.h>
#include <vector>
#include <cstdlib>

class Vector{
private:
	double x, y;

public:
	Vector(){;}
	Vector(double x_in = 0.0, double y_in = 0.0): x(x_in), y(y_in)
	

	double get_x(){return x;}
	double get_y(){return y;}
	void set_x(double x){this->x = x;}
	void set_y(double y){this->y = y;}
	
	//Might use some built-in Angle and Position functions instead
	//This will be directly piped in to the reference_angle
	//of the controller
	//
	Angle vector_angle{
		if(x == 0.0){ //crap preventative measures
			return 0.0;
		}

		return Angle(atan(y/x)-180);
	}
	

	//Operators
	Vector operator+(const Vector& vec){
		return Vector(x+vec.get_x(), y+vec.get_y());
	}
	
	Vector operator*(const double& scale){
		return Vector(scale*x, scale*y);
	}
	
	Vector operator*=(const double& scale){
		x *= scale;
		y *= scale;

		return *this;
	}

	double operator*(const Vector& vec){
		return x*vec.get_x() + y*vec.get_y();
	}

};

//Might be called as an array/std::vector of Vector_fields taking in 
//the current robots position
//Vectors from each field is calculated and added toGether with
//the overloaded operator +=
class Vector_field{
private:


public:
	Vector_field(double x, double y): origo(x, y){}
	Vector_field(Position pos = Position(0,0)): origo(pos){}
	
	//Maybe make this virtual and create subclasses for robots, walls 
	//and tarGet position
	virtual Vector vector_at_pos(Position pos) = 0;
	

protected:
	Position origo;

};


class Robot_vector_field: public Vector_field{
private:
	

public:
	Robot_vector_field(Position pos = Position(0,0)): origo(pos){}
	
	Vector vector_at_pos(Position pos){
		double x = (pos.GetX()-origo.GetX()) / 
				   (pow(fabs(origo.GetX()+pos.GetX()), 3));
		double y = (pos.GetY()-origo.GetY()) / 
				   (pow(fabs(origo.GetY()+pos.GetY()), 3));

		return Vector(x, y);
	}
};

class Wall_vector_field: public Vector_field{
private:


public:	
	Wall_vector_field(Position pos = Position(0,0)): origo(pos){}

	Vector vector_at_pos(Position pos){
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
};


class Target_vector_field{

private:
	double scale;

public:
	Target_vector_field(Position pos = Position(0,0)): origo(pos){}

	Vector vector_at_pos(Position pos){
		//Effect of the field shrinks with the higher the power.
		double x = scale*(origo.GetX() - pos.GetX())/
					pow(fabs(pos.GetX() - X_MIN_COOR), 3);

		double y = (pos.GetY() - Y_MIN_COOR)/
					pow(fabs(pos.GetY() - Y_MIN_COOR), 3) +
 					(pos.GetY() - Y_MIN_COOR)/
					pow(fabs(pos.GetY() - Y_MIN_COOR), 3);
		
		return Vector(x, y);
	}
};

#endif
