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

#include <cmath>

namespace geometry {

Arc::Arc(const Circle& circle, double start_angle, double end_angle) 
    : circle_(circle) {
  startAngle_ = GeometryUtils::NormalizeAngle(start_angle);
  endAngle_ = GeometryUtils::NormalizeAngle(end_angle);
  startPoint_ = circle_.GetPoint(startAngle_);
  endPoint_ = circle_.GetPoint(endAngle_);
}

Arc::Arc(const geometry::Point& center, const geometry::Point& from,
    const geometry::Point& to) {
  double radius = center.GetDistance(from);
  circle_.SetCenter(center);
  circle_.SetRadius(radius);

  startAngle_ = circle_.GetAngle(from);
  endAngle_ = circle_.GetAngle(to);
  startPoint_ = circle_.GetPoint(startAngle_);
  endPoint_ = circle_.GetPoint(endAngle_);
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
  startPoint_ = circle_.GetPoint(startAngle_);
  endPoint_ = circle_.GetPoint(endAngle_);
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

bool Arc::IntersectFast(const Segment& segment) const {
  const Point& A = segment.A();
  const Point& B = segment.B();
  const Point& C = circle_.GetCenter();
  double R = circle_.GetRadius();
  double a = segment.SquaredLength();
  double b = (A.x - B.x) * (B.x - C.x) + (A.y - B.y)* (B.y - C.y);
  double c = (B.x - C.x) * (B.x - C.x) + (B.y - C.y) * (B.y - C.y) - R * R;
  double D = b * b - a * c;
  if (DoubleIsGreater(0.0, D)) {
    return false;
  } else if (DoubleIsZero(D)) {
    double u = (-b) / a;
    if (DoubleIsBetween(u, 0, 1)) {
      geometry::Point temp(A.x * u + B.x * (u - 1), A.y * u + B.y * (u - 1));
      return Contains(temp);
    } else {
      return false;
    }
  } else {
    double d = sqrt(D);
    double u1 = (-b - d) / a;
    if (DoubleIsBetween(u1, 0, 1) && Contains(segment.GetPoint(1.0 - u1))) {
      return true;
    }
    double u2 = (-b + d) / a;
    return DoubleIsBetween(u2, 0, 1) && Contains(segment.GetPoint(1.0 - u2));
  }
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

const geometry::Point& Arc::GetStartPoint() const {
  return startPoint_;
}
  
const geometry::Point& Arc::GetEndPoint() const {
  return endPoint_;
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

double Arc::GetLength() const {
  return GetRadius() * fabs(GetEndAngle()- GetStartAngle());
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
