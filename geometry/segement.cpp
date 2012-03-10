#include "geometry/segment.h"

#include "geometry/bounding_box.h"
#include "geometry/point.h"
#include "geometry/vector.h"
#include "utils/double_utils.h"

#include <algorithm>

namespace geometry {

Segment::Segment(const Point& A, const Point& B) : A_(A), B_(B) {}
  
const Point& Segment::A() const {
  return A_;
}

const Point& Segment::B() const {
  return B_;
}

BoundingBox Segment::GetBoundingBox() const {
  double minx, maxx;
  double miny, maxy;

  minx = std::min(A_.x, B_.x);
  maxx = std::max(A_.x, B_.x);

  miny = std::min(A_.y, B_.y);
  maxy = std::max(A_.y, B_.y);
  
  return BoundingBox(minx, maxx, miny, maxy);
}

bool Segment::Intersect(const Segment& other, double* this_fraction,
    double* other_fraction) const {
  geometry::Vector vector1(A(), B());
  geometry::Vector vector2(other.A(), other.B());

  double determinant = vector2.CrossProduct(vector1);
  if (DoubleIsZero(determinant)) {
    return false;
  }

  double x_diff = A().x - other.A().x;
  double y_diff = A().y - other.A().y;

  *this_fraction = (vector2.y * x_diff - vector2.x * y_diff) / determinant;
  if (!DoubleIsBetween(*this_fraction, 0.0, 1.0)) {
    return false;
  }

  *other_fraction = (vector1.y * x_diff - vector1.x * y_diff) / determinant;
  if (!DoubleIsBetween(*other_fraction, 0.0, 1.0)) {
    return false;
  }

  return true;
}

bool Segment::Intersect(const Segment& other, double* this_fraction) const {
  double other_fraction;
  return Intersect(other, this_fraction, &other_fraction);
}

Point Segment::GetPoint(double fraction) const {
  return Point(A_.x * (1.0 - fraction) + B_.x * fraction,
               A_.y * (1.0 - fraction) + B_.y * fraction);
}

Segment Segment::SubSegment(double start_fraction, double end_fraction) const {
  return Segment(GetPoint(start_fraction), GetPoint(end_fraction));
}

double Segment::Length() const {
  return A_.GetDistance(B_);
}

}  // namespace geometry