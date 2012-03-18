#ifndef INCLUDE_GEOMETRY_CIRCLE_H_
#define INCLUDE_GEOMETRY_CIRCLE_H_

#include "geometry/point.h"

#include <vector>

namespace geometry {

class Line;

class Circle {
 public:
  Circle(const Point& center, double radius);

  const Point& GetCenter() const;
  double GetRadius() const;

  std::vector<Point> Intersect(const Line& line) const;

 private:
  double radius_;
  Point center_;
};

};
#endif  // INCLUDE_GEOMETRY_CIRCLE_H_