/*
Ben Golden, 12/7/2015
vector.cpp
C++ file for Vector functions to use with ray tracer
*/
#include "vector.h"

#include <math.h>

//Constructors
Vector::Vector() :
  x(0),
  y(0),
  z(0)
{}

Vector::Vector(double newX, double newY, double newZ) :
  x(newX),
  y(newY),
  z(newZ)
{}

//Overloaded operators
Vector Vector::operator+(const Vector& rhs)
{
  Vector ret(x + rhs.x, y + rhs.y, z + rhs.z);
  return ret;
}

Vector Vector::operator-(const Vector& rhs)
{
  Vector ret(x - rhs.x, y - rhs.y, z - rhs.z);
  return ret;
}

Vector Vector::operator*(double rhs)
{
  Vector ret(x * rhs, y * rhs, z * rhs);
  return ret;
}

Vector Vector::operator/(double rhs)
{
  Vector ret(x / rhs, y / rhs, z / rhs);
  return ret;
}

//Magnitude
double Vector::magnitude()
{
  return (sqrt(x * x + y * y + z * z));
}

//Normalize
Vector Vector::normalize()
{
  double mag = magnitude();
  x /= mag;
  y /= mag;
  z /= mag;
  return *this;
}

//Dot product
double Vector::dot(const Vector& rhs)
{
  return (x * rhs.x + y * rhs.y + z * rhs.z);
}

double Vector::getX()
{
  return x;
}

double Vector::getY()
{
  return y;
}

double Vector::getZ()
{
  return z;
}

std::ostream& operator<<(std::ostream& out, const Vector& vec)
{
  out << "<" << vec.x << ", " << vec.y << ", " << vec.z << ">";
  return out;
}