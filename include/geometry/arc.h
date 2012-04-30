#ifndef INCLUDE_GEOMETRY_ARC_H_
#define INCLUDE_GEOMETRY_ARC_H_

#include "geometry/circle.h"

namespace geometry {

class BoundingBox;
class Line;
class Point;
class Segment;

class Arc { 
 public:
  Arc(const Circle& circle, double start_angle, double end_angle);
  Arc(const geometry::Point& center, const geometry::Point& from,
      const geometry::Point& to);
  Arc(const geometry::Point& center, const geometry::Point& from,
      double angle);

  std::vector<Point> Intersect(const Line& line) const;
  bool Intersect(const Segment& segment) const;
  bool IntersectFast(const Segment& segment) const;

  double GetStartAngle() const;
  double GetEndAngle() const;

  const Point& GetStartPoint() const;
  const Point& GetEndPoint() const;

  double GetRadius() const;
  Point GetCenter() const;
  const Circle& GetCircle() const;

  double GetLength() const;
  BoundingBox GetBoundingBox() const;

 private:
  bool Contains(const Point& point) const;
  bool ContainsAngle(double angle) const;

 private:
  Circle circle_;
  Point startPoint_, endPoint_;
  double startAngle_, endAngle_;
};

}  // namespace geometry
#endif  // INCLUDE_GEOMETRY_ARC_H_
