#include "vectorfield.h"


//Vector class funtions

//Might use some built-in Angle and Position functions instead
//This will be directly piped in to the reference_angle
//of the controller
//
Angle ateam::Vector::vector_angle(){
    return Position(0, 0).AngleOfLineToPos(Position(x, y));
}


//non-member operators
ateam::Vector ateam::operator+(const ateam::Vector& vec1, const ateam::Vector& vec2){
    return ateam::Vector(vec1.get_x()+vec2.get_x(), vec1.get_y()+vec2.get_y());

}

ateam::Vector ateam::operator*(const double& scale, const ateam::Vector& vec){
    return ateam::Vector(scale*vec.get_x(), scale*vec.get_y());

}

double ateam::operator*(const ateam::Vector& vec1, const ateam::Vector& vec2){
    return vec1.get_x()*vec2.get_x()+vec1.get_y()*vec2.get_y();

}


//member operators
ateam::Vector ateam::Vector::operator*=(const double& scale){

    return scale*(*this);
}


ateam::Vector ateam::Vector::operator=(const ateam::Vector& vec){
    x = vec.get_x();
    y = vec.get_y();

    return *this;
}

ateam::Vector ateam::Vector::operator+=(const ateam::Vector& vec){

    return *this + vec;
}






//Vector_field class funtions

//Equations
//Might be called as an array/std::vector of Vector_fields taking in
//the current robots position
//Vectors from each field is calculated and added together with
//the overloaded operator +=

ateam::Vector ateam::Robot_vector_field::vector_at_pos(Position pos){

    double x_diff = pos.GetX()-center_point.GetX();
    double y_diff = pos.GetY()-center_point.GetY();
    double numerator = pow(fabs(x_diff), 3)+pow(fabs(y_diff), 3);

    double x = (x_diff) / numerator;
    double y = (y_diff) / numerator;

    return ateam::Vector(x, y);
}


ateam::Vector ateam::Wall_vector_field::vector_at_pos(Position pos){
    //Effect of the field shrinks with the higher the power.
    double x = (pos.GetX() - X_MIN_COOR)/
            pow(fabs(pos.GetX() - X_MIN_COOR), 3) +
            (pos.GetX() - X_MAX_COOR)/
            pow(fabs(pos.GetX() - X_MAX_COOR), 3);

    double y = (pos.GetY() - Y_MIN_COOR)/
            pow(fabs(pos.GetY() - Y_MIN_COOR), 3) +
            (pos.GetY() - Y_MAX_COOR)/
            pow(fabs(pos.GetY() - Y_MAX_COOR), 3);

    return ateam::Vector(x, y);
}


ateam::Vector ateam::Target_vector_field::vector_at_pos(Position pos){
    double x_diff = pos.GetX()-center_point.GetX();
    double y_diff = pos.GetY()-center_point.GetY();
    double numerator = -sqrt(pow(fabs(x_diff), 2) + pow(fabs(y_diff), 2));

    double x = x_diff/numerator;
    double y = y_diff/numerator;

    return ateam::Vector(x, y);
}

