#ifndef VECTORFIELD_H
#define VECTORFIELD_H

#include "position.h"
#include "angle.h"

//Are all of these needed here?
#include <vector>
#include <cstdlib>
#include <iostream>


namespace ateam{

class Vector{
private:
    double x, y;

public:
    Vector():x(0), y(0){;}
    Vector(double x_in, double y_in): x(x_in), y(y_in){}
	Vector(Position pos) : x(pos.GetX()), y(pos.GetY()) {}

    double get_x() const {return x;}
    double get_y() const {return y;}
    void set_x(double x){this->x = x;}
    void set_y(double y){this->y = y;}

    //Might use some built-in Angle and Position functions instead
    //This will be directly piped in to the reference_angle
    //of the controller
    //
    Angle vector_angle();
	void rotate(Angle angle);
    double length(){ return sqrt((*this) * (*this)); }

    //Operators
    friend Vector operator+(const Vector& vec1, const Vector& vec2);

    friend Vector operator-(const Vector& vec1, const Vector& vec2);

    friend Vector operator*(const double& scale, const Vector& vec);

    friend double operator*(const Vector& vec1, const Vector& vec2);

    friend ostream& operator<<(ostream& os, const Vector& vec);

    Vector operator*=(const double& scale);

    Vector operator=(const Vector& vec);

    Vector operator+=(const Vector& vec);

    Vector operator-=(const Vector& vec);

	operator Position();

};



//Might be called as an array/std::vector of Vector_fields taking in 
//the current robots position
//Vectors from each field is calculated and added toGether with
//the overloaded operator +=
class Vector_field{
private:


public:
    
    Vector_field(double x, double y): center_point(x, y){}
    Vector_field(Position pos = Position(0,0)): center_point(pos){}

    //Maybe make this virtual and create subclasses for robots, walls
    //and target position
    virtual Vector vector_at_pos(Position pos) = 0;

    void set_target_pos(Position pos){target_pos = pos;}
    void set_center_point(Position pos){center_point = pos;}
    void set_center_point(double x, double y){center_point.SetX(x); center_point.SetY(y);}
    Position get_center_point(){return center_point;}

protected:
    Position target_pos;
    Position center_point;

};


class Robot_vector_field: public Vector_field{
private:

    double eccentricity; //Set as function of vector field weight?

public:
    Robot_vector_field(Position pos = Position(0, 0)): eccentricity(1) {center_point = pos;}
    Vector vector_at_pos(Position pos);

};

class Wall_vector_field: public Vector_field{
private:


public:	
    Wall_vector_field(){}
    Vector vector_at_pos(Position pos);
};


class Target_vector_field: public Vector_field{

private:

public:
    Target_vector_field(Position pos = Position(0, 0)){center_point = pos;}
    Vector vector_at_pos(Position pos);
};

Vector operator-(const Vector& vec1, const Vector& vec2);

Vector operator+(const Vector& vec1, const Vector& vec2);

Vector operator*(const double& scale, const Vector& vec);

double operator*(const Vector& vec1, const Vector& vec2);

ostream& operator<<(ostream& os, const Vector& vec);
}
#endif
