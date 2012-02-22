#include "geometry/circle.h"

#include "geometry/line.h"
#include "geometry/point.h"
#include "geometry/vector.h"
#include "utils/double_utils.h"

namespace geometry {

Circle::Circle(const Point& center, double radius)
    : center_(center), radius_(radius) {}

std::vector<Point> Circle::Intersect(const Line& line) const {
  std::vector<Point> result;

  Line perpendiculiar(center_, line.Normal());
  Point heel;
  perpendiculiar.Intersect(line, &heel);

  double distance = heel.GetSquaredDistance(center_);
  int sign = DoubleSign(distance - radius_ * radius_);
  if (sign > 0) {
    return result;
  } else if (sign == 0) {
    result.push_back(heel);
    return result;
  } else {
    Vector parallel = line.Normal().GetOrthogonal().Unit();
    parallel = parallel * sqrt(radius_ * radius_ - distance);
    result.push_back(heel + parallel);
    result.push_back(heel - parallel);
    return result;
  }
}

const Point& Circle::GetCenter() const {
  return center_;
}

double Circle::GetRadius() const {
  return radius_;
}

}  // namespace geometry  