#ifndef INCLUDE_GEOMETRY_ARC_H_
#define INCLUDE_GEOMETRY_ARC_H_

#include "geometry/circle.h"

namespace geometry {

class Line;
class Point;

class Arc { 
 public:
  Arc(const Circle& circle, double start_angle, double end_angle);

  std::vector<Point> Intersect(const Line& line) const;
 
 private:
  bool Contains(const Point& point) const;
  double GetAngle(const Point& point) const;

 private:
  Circle circle_;
  double startAngle_, endAngle_;
};

}  // namespace geometry
#endif  // INCLUDE_GEOMETRY_ARC_H_