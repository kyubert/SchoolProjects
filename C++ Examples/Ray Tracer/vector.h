/*
Ben Golden, 12/7/2015
vector.h
Header for Vector file to be used for ray tracer
*/

#ifndef VECTOR_H
#define VECTOR_H

#include <ostream>

class Vector {
public:
    Vector();
    Vector(double, double, double);

    Vector operator+(const Vector&);
    Vector operator-(const Vector&);
    Vector operator*(double);
    Vector operator/(double);

    double magnitude();
    Vector normalize();
    double dot(const Vector&);

    double getX();
    double getY();
    double getZ();

    friend std::ostream& operator<<(std::ostream&, const Vector&);

//private:
    double x;
    double y;
    double z;
};

#endif
