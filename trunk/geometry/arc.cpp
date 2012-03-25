#include "geometry/arc.h"

#include "geometry/circle.h"
#include "geometry/point.h"
#include "geometry/geometry_utils.h"
#include "utils/double_utils.h"

#include <vector>

namespace geometry {

Arc::Arc(const Circle& circle, double start_angle, double end_angle) 
    : circle_(circle), startAngle_(start_angle), endAngle_(end_angle) {}

Arc::Arc(const geometry::Point& center, const geometry::Point& from,
    const geometry::Point& to) {
  double radius = center.GetDistance(from);
  circle_.SetCenter(center);
  circle_.SetRadius(radius);

  startAngle_ = circle_.GetAngle(from);
  endAngle_ = circle_.GetAngle(to);
}

Arc::Arc(const geometry::Point& center, const geometry::Point& from,
    double angle) {
  double radius = center.GetDistance(from);
  circle_.SetCenter(center);
  circle_.SetRadius(radius);

  startAngle_ = circle_.GetAngle(from);
  if(DoubleIsGreater(angle, geometry::GeometryUtils::PI)) {
    angle = geometry::GeometryUtils::PI;
  }

  endAngle_ = startAngle_ + angle;
}

std::vector<Point> Arc::Intersect(const Line& line) const {
  std::vector<Point> intersections = circle_.Intersect(line);
  std::vector<Point> result;
  for (unsigned index = 0; index < intersections.size(); ++index) {
    if (Contains(intersections[index])) {
      result.push_back(intersections[index]);
    }
  }

  return result;
}

bool Arc::Contains(const Point& point) const {
  double angle = circle_.GetAngle(point);
  if (DoubleIsGreaterOrEqual(endAngle_, startAngle_)) {
    return DoubleIsGreaterOrEqual(angle, startAngle_) 
        && DoubleIsGreaterOrEqual(endAngle_, angle);
  } else {
    return DoubleIsGreaterOrEqual(angle, endAngle_) 
        && DoubleIsGreaterOrEqual(startAngle_, angle);
  }
}

}  // namespace geometry