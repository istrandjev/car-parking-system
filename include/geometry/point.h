#ifndef INCLUDE_GEOMETRY_POINT_H_
#define INCLUDE_GEOMETRY_POINT_H_

#include <iostream>

namespace geometry {

class Vector;

// A class that represents a point.
class Point {
 public:
  Point() : x(0), y(0) {}
  Point(double x_coordinate, double y_coordinate)
    : x(x_coordinate), y(y_coordinate) {}

  Point& operator+=(const Vector& vector);
  Point& operator-=(const Vector& vector);
  Point operator+(const Vector& vector) const;
  Point operator-(const Vector& vector) const;
  bool operator==(const Point& other) const;
  bool operator!=(const Point& other) const;

  Point Rotate(const Point& center, double angle) const;

  double GetSquaredDistance(const Point& other) const;
  double GetDistance(const Point& other) const;

 public:
  double x, y;
};

std::istream& operator>>(std::istream& in, Point& point);
std::ostream& operator<<(std::ostream& out, const Point& point);

}  // namespace geometry
#endif  // INCLUDE_GEOMETRY_POINT_H_
