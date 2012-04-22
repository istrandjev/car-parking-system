#ifndef INCLUDE_GEOMETRY_VECTOR_H_
#define INCLUDE_GEOMETRY_VECTOR_H_

#include <iostream>

namespace geometry {

class Point;

class Vector {
 public:
  Vector() : x(0), y(0) {}
  Vector(double x_coordinate, double y_coordinate)
    : x(x_coordinate), y(y_coordinate) {}
  Vector(const Point& a, const Point& b);

  Vector& operator*=(double scalar);
  Vector operator*(double scalar) const;

  Vector Unit() const;
  Vector GetOrthogonal() const;
  Vector Rotate(double angle) const;
  double DotProduct(const Vector& other) const;
  double CrossProduct(const Vector& other) const;
  double SquaredLength() const;
  double Length() const;

  bool Parallel(const Vector& other) const;

 public:
  double x, y;
};

std::istream& operator>>(std::istream& in, Vector& vector);
std::ostream& operator<<(std::ostream& out, const Vector& v);

}  // namespace geometry
#endif  // INCLUDE_GEOMETRY_VECTOR_H_
