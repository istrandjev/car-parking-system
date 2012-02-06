#include "vector.h"

#include "double_utils.h"
#include "point.h"

#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

namespace geometry {

Vector::Vector(const Point& a, const Point& b) {
  x = b.x - a.x;
  y = b.y - a.y;
}

Vector& Vector::operator*=(double scalar) {
  x *= scalar;
  y *= scalar;
  return *this;
}

Vector Vector::operator*(double scalar) const {
  Vector result = *this;
  result *= scalar;
  return result;
}

Vector Vector::Unit() const {
  if (DoubleIsZero(x) && DoubleIsZero(y)) {
    throw std::invalid_argument("Trying to get unit zero vector!");
    return *this;
  }

  double length = sqrt(SquaredLength());
  return Vector(x/length, y/length);
}

Vector Vector::GetOrthogonal() const {
  return Vector(this->y, -this->x);
}

Vector Vector::Rotate(double angle) const {
  double cos_angle = cos(angle);
  double sin_angle = sin(angle);
  return Vector(x * cos_angle - y * sin_angle, 
      x * sin_angle + y * cos_angle);
}

double Vector::DotProduct(const Vector& other) const {
  return this->x * other.x + this->y * other.y;
}

double Vector::CrossProduct(const Vector& other) const {
  return this->x * other.y - this->y * other.x;
}

double Vector::SquaredLength() const {
  return this->DotProduct(*this);
}

bool Vector::Parallel(const Vector& other) const {
  return DoubleIsZero(CrossProduct(other));
}

std::istream& operator>>(std::istream& in, Vector& vector) {
  std::string vector_string;
  getline(in, vector_string, ')');
  for (unsigned index = 0; index < vector_string.size(); ++index) {
    if (vector_string[index] == ',' || vector_string[index] == '(') {
      vector_string[index] = ' ';
    }
  }

  std::istringstream inss(vector_string);
  inss >> vector.x >> vector.y;
  return in;
}

}  // namespace geometry