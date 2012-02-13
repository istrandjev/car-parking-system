#include "geometry/point.h"

#include "geometry/vector.h"

#include <cmath>
#include <iostream>
#include <string>
#include <sstream>

namespace geometry {

Point& Point::operator+=(const Vector& vector) {
  x += vector.x;
  y += vector.y;
  return *this;
}

Point& Point::operator-=(const Vector& vector) {
  x -= vector.x;
  y -= vector.y;
  return *this;
}

Point Point::operator+(const Vector& vector) const {
  Point result = *this;
  result += vector;
  return result;
}

Point Point::operator-(const Vector& vector) const {
  Point result = *this;
  result -= vector;
  return result;
}

Point Point::Rotate(const Point& center, double angle) const {
  double translatedx = x - center.x;
  double translatedy = y - center.y;

  double cos_angle = cos(angle);
  double sin_angle = sin(angle);
  Point result;
  result.x = translatedx * cos_angle - translatedy * sin_angle + center.x;
  result.y = translatedx * sin_angle + translatedy * cos_angle + center.y; 
  return result;
}

double Point::GetSquaredDistance(const Point& other) const {
  return (x - other.x) * (x - other.x) + (y - other.y) * (y - other.y);
}

double Point::GetDistance(const Point& other) const {
  return sqrt(GetSquaredDistance(other));
}

std::istream& operator>>(std::istream& in, Point& point) {
  std::string point_string;
  getline(in, point_string, ')');
  for (unsigned index = 0; index < point_string.size(); ++index) {
    if (point_string[index] == ',' || point_string[index] == '(') {
      point_string[index] = ' ';
    }
  }

  std::istringstream inss(point_string);
  inss >> point.x >> point.y;
  return in;
}

std::ostream& operator<<(std::ostream& out, Point& point) {
  out << "(" << point.x << ", " << point.y << ")";
  return out;
}
}  // namespace geometry