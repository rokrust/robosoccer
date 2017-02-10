#include "vectorfield.h"
#include <math.h>


#define exponent 4
#define X_MIN_COOR -1.15
#define Y_MIN_COOR -0.9
#define X_MAX_COOR 1.15
#define Y_MAX_COOR 0.9


//This will be directly piped in to the reference_angle of the controller
/**
 * @brief Returns the angle of vector *this
 *
 * @return Angle
 */
Angle ateam::Vector::vector_angle(){
    return Position(0, 0).AngleOfLineToPos(Position(x, y));;
}

/**
 * @brief Rotates the vector *this.
 *
 * @param angle The angle of the rotation.
 */
void ateam::Vector::rotate(Angle angle) {
	double old_x = x, old_y = y;
	double cosine = cos(angle.Get());
	double sine = sin(angle.Get());

	set_x(old_x*cosine + old_y*sine);
	set_y(-x*sine + old_y*cosine);
}

//non-member operators
/**
 * @brief
 *
 * @param os
 * @param vec
 * @return ostream & ateam::operator
 */
ostream& ateam::operator<<(ostream& os, const Vector& vec)
{
    os << "[" << vec.get_x() << ", " << vec.get_y() << "]";
    return os;
}

/**
 * @brief
 *
 * @param vec1
 * @param vec2
 * @return ateam::Vector ateam::operator
 */
ateam::Vector ateam::operator+(const ateam::Vector& vec1, const ateam::Vector& vec2){
    return ateam::Vector(vec1.get_x()+vec2.get_x(), vec1.get_y()+vec2.get_y());

}

/**
 * @brief
 *
 * @param vec1
 * @param vec2
 * @return ateam::Vector ateam::operator
 */
ateam::Vector ateam::operator-(const ateam::Vector& vec1, const ateam::Vector& vec2){
    return ateam::Vector(vec1.get_x()-vec2.get_x(), vec1.get_y()-vec2.get_y());

}

/**
 * @brief
 *
 * @param scale
 * @param vec
 * @return ateam::Vector ateam::operator
 */
ateam::Vector ateam::operator*(const double& scale, const ateam::Vector& vec){
    return ateam::Vector(scale*vec.get_x(), scale*vec.get_y());

}

/**
 * @brief
 *
 * @param vec1
 * @param vec2
 * @return double ateam::operator
 */
double ateam::operator*(const ateam::Vector& vec1, const ateam::Vector& vec2){
    return vec1.get_x()*vec2.get_x()+vec1.get_y()*vec2.get_y();

}


//member operators
/**
 * @brief
 *
 * @param scale
 * @return ateam::Vector ateam::Vector::operator
 */
ateam::Vector ateam::Vector::operator*=(const double& scale){

    return *this = scale*(*this);
}


/**
 * @brief
 *
 * @param vec
 * @return ateam::Vector ateam::Vector::operator
 */
ateam::Vector ateam::Vector::operator=(const ateam::Vector& vec){
    x = vec.get_x();
    y = vec.get_y();

    return *this;
}

/**
 * @brief
 *
 * @param vec
 * @return ateam::Vector ateam::Vector::operator
 */
ateam::Vector ateam::Vector::operator+=(const ateam::Vector& vec){

    return *this = *this + vec;
}

/**
 * @brief
 *
 * @param vec
 * @return ateam::Vector ateam::Vector::operator
 */
ateam::Vector ateam::Vector::operator-=(const ateam::Vector& vec){

    return *this = *this + vec;
}

/**
 * @brief
 *
 * @return ateam::Vector::operator
 */
ateam::Vector::operator Position() { return Position(x, y); }

//x:        (x*tan(1)-y)/(sqrt((x*tan(1)-y)^2+(y/tan(1)-x)^2)),
//y:        (y/tan(1)-x)/(sqrt((x*tan(1)-y)^2+(y/tan(1)-x)^2)
//scale:    1/(15(sin(0.75)*x-cos(0.75)y)^2 + (cos(0.75)*x+sin(0.75)y)^2 + 0.01)
/**
 * @brief Function of the robot vector fields. Returns the vector at a given position.
 *
 * @param pos Position
 * @return ateam::Vector
 */
ateam::Vector ateam::Robot_vector_field::vector_at_pos(Position pos){
    Angle vector_field_angle = pos.AngleOfLineToPos(target_pos);
    //cout << " vector field angle: " << vector_field_angle << endl;


    //Ease of calculations
    double tangent = tan(vector_field_angle.Get());
    double sine = sin(vector_field_angle.Get());
    double cosine = cos(vector_field_angle.Get());

    //Substitution to move the center of the vector field
    double x_diff = (pos.GetX()-center_point.GetX()) * 5;
    double y_diff = (pos.GetY()-center_point.GetY()) * 5;

    double vector_field_denominator = sqrt(pow(x_diff * tangent - y_diff, 2) +
                                           pow(y_diff / tangent - x_diff, 2)) + 0.01;

    double x = (x_diff * tangent - y_diff) / vector_field_denominator;
    double y = (y_diff / tangent - x_diff) / vector_field_denominator;

    //cout << "x_diff " << x_diff << endl;
    //cout << "y_diff " << y_diff << endl;
    double scalar_field = 1 / (eccentricity * pow(sine * x_diff - cosine * y_diff, 2) +
                                              pow(cosine * x_diff + sine * y_diff, 2) + 0.001);
    //cout << "scalar: " << scalar_field << endl;
    return scalar_field * ateam::Vector(x, y); //82 % sure this works
}


/**
 * @brief Returns the vector from the wall vector field at a given position.
 *
 * @param pos Position
 * @return ateam::Vector
 */
ateam::Vector ateam::Wall_vector_field::vector_at_pos(Position pos){
    //Effect of the field shrinks with the higher the power.
    double x = (pos.GetX() - X_MIN_COOR)/
            pow(fabs(pos.GetX() - X_MIN_COOR), exponent) +
            (pos.GetX() - X_MAX_COOR)/
            pow(fabs(pos.GetX() - X_MAX_COOR), exponent);

    double y = (pos.GetY() - Y_MIN_COOR)/
            pow(fabs(pos.GetY() - Y_MIN_COOR), exponent) +
            (pos.GetY() - Y_MAX_COOR)/
            pow(fabs(pos.GetY() - Y_MAX_COOR), exponent);

    return ateam::Vector(x, y);
}


/**
 * @brief Returns the vector from the target vector field at a given position.
 *
 * @param pos Position
 * @return ateam::Vector
 */
ateam::Vector ateam::Target_vector_field::vector_at_pos(Position pos = Position(0, 0)){
    double x_diff = pos.GetX()-center_point.GetX();
    double y_diff = pos.GetY()-center_point.GetY();
    double denominator = -sqrt(pow(fabs(x_diff), 2) + pow(fabs(y_diff), 2));

    double x = x_diff/denominator;
    double y = y_diff/denominator;

    return ateam::Vector(x, y);
}
