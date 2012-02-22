#include "geometry/arc.h"

#include "geometry/circle.h"
#include "geometry/point.h"
#include "geometry/geometry_utils.h"
#include "utils/double_utils.h"

#include <vector>

namespace geometry {

Arc::Arc(const Circle& circle, double start_angle, double end_angle) 
    : circle_(circle), startAngle_(start_angle), endAngle_(end_angle) {}

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
  double angle = GetAngle(point);
  if (DoubleIsGreaterOrEqual(endAngle_, startAngle_)) {
    return DoubleIsGreaterOrEqual(angle, startAngle_) 
        && DoubleIsGreaterOrEqual(endAngle_, angle);
  } else {
    return DoubleIsGreaterOrEqual(angle, endAngle_) 
        && DoubleIsGreaterOrEqual(startAngle_, angle);
  }
}

double Arc::GetAngle(const Point& point) const {
  const geometry::Point& center = circle_.GetCenter();
  double angle = atan2(point.x - center.x, point.y - center.y);
  if (DoubleIsGreater(0.0, angle)) {
    angle += 2.0 * geometry::GeometryUtils::PI;
  }
  return angle;
}

}  // namespace geometry