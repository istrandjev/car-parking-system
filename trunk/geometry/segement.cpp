#include "geometry/segment.h"

#include "geometry/bounding_box.h"
#include "geometry/point.h"

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

}  // namespace geometry