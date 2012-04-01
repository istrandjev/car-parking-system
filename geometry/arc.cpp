#include "geometry/arc.h"

#include "geometry/bounding_box.h"
#include "geometry/circle.h"
#include "geometry/geometry_utils.h"
#include "geometry/line.h"
#include "geometry/point.h"
#include "geometry/segment.h"
#include "utils/double_utils.h"

#include <algorithm>
#include <vector>

namespace geometry {

Arc::Arc(const Circle& circle, double start_angle, double end_angle) 
    : circle_(circle) {
  startAngle_ = GeometryUtils::NormalizeAngle(start_angle);
  endAngle_ = GeometryUtils::NormalizeAngle(end_angle);
}

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

  if (DoubleIsGreater(angle, 0)) {
    startAngle_ = circle_.GetAngle(from);
    endAngle_ = GeometryUtils::NormalizeAngle(startAngle_ + angle);
  } else {
    endAngle_ = circle_.GetAngle(from);
    startAngle_ = GeometryUtils::NormalizeAngle(endAngle_ + angle);
  }
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

bool Arc::Intersect(const Segment& segment) const {
  std::vector<Point> points = Intersect(segment.GetLine());

  geometry::BoundingBox bounding_box = segment.GetBoundingBox();
  for (unsigned index = 0; index < points.size(); ++index) {
    if (bounding_box.Contains(points[index])) {
      return true;
    }
  }
  return false;
}

bool Arc::Contains(const Point& point) const {
  double angle = circle_.GetAngle(point);
  return ContainsAngle(angle);
}

bool Arc::ContainsAngle(double angle) const {
  if (DoubleIsGreaterOrEqual(endAngle_, startAngle_)) {
    return DoubleIsBetween(angle, startAngle_, endAngle_);
  } else {
    return DoubleIsBetween(angle, 0, endAngle_) ||
     DoubleIsGreaterOrEqual(angle, startAngle_);
  }
}

double Arc::GetStartAngle() const {
  return startAngle_;
}

double Arc::GetEndAngle() const {
  return endAngle_;
}

geometry::Point Arc::GetStartPoint() const {
  return circle_.GetPoint(startAngle_);
}
  
geometry::Point Arc::GetEndPoint() const {
  return circle_.GetPoint(endAngle_);
}

double Arc::GetRadius() const {
  return circle_.GetRadius();
}

Point Arc::GetCenter() const {
  return circle_.GetCenter();
}

const Circle& Arc::GetCircle() const {
  return circle_;
}

BoundingBox Arc::GetBoundingBox() const {
  BoundingBox result(circle_.GetCenter());

  result.AddPoint(circle_.GetPoint(startAngle_));
  result.AddPoint(circle_.GetPoint(endAngle_));

  const double& pi = geometry::GeometryUtils::PI;
  for (double angle = -pi * 2.0; angle < pi * 2.0; angle += pi * 0.5) {
    if (ContainsAngle(angle)) {
      result.AddPoint(circle_.GetPoint(angle));    
    }
  }
 
  return result; 
}

}  // namespace geometry