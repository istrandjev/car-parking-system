#include "rectangle_object.h"

#include "double_utils.h"
#include "geometry_utils.h"
#include "point.h"
#include "polygon.h"
#include "vector.h"

namespace geometry {

static const double DEFAULT_WIDTH = 2.0;

RectangleObject::RectangleObject(const Point& from, const Point& to) 
    : from_(from), to_(to), width_(DEFAULT_WIDTH){}

RectangleObject::RectangleObject(const Point& from, const Point& to, double width)
    : from_(from), to_(to), width_(width) {}

const Point& RectangleObject::GetFrom() const {
  return from_;
}

void RectangleObject::SetFrom(const Point& point) {
  from_ = point;
}
  
const Point& RectangleObject::GetTo() const {
  return to_;
}

void RectangleObject::SetTo(const Point& point) {
  to_ = point;
}

double RectangleObject::GetWidth() const {
  return width_;
}

void RectangleObject::SetWidth(double width) {
  width_ = width;
}

void RectangleObject::ReverseDirection() {
  std::swap(from_, to_);
}

bool RectangleObject::ContainsPoint(const Point& p) const {
  Vector segment(from_, to_);

  Vector from_p(from_, p);
  if (DoubleIsGreater(0.0, segment.DotProduct(from_p))) {
    return false;
  }

  Vector to_p(to_, p);
  if (DoubleIsGreater(0.0, segment.DotProduct(to_p))) {
    return false;
  }

  return DoubleIsGreaterOrEqual(width_ * segment.Length(),
      segment.CrossProduct(from_p));
}

Polygon RectangleObject::GetBounds() const {
  Vector segment(from_, to_);
  Vector shift = segment.GetOrthogonal().Unit() * width_ * 0.5;
  Polygon result;
  result.AddPointDropDuplicates(from_ + shift);
  result.AddPointDropDuplicates(from_ - shift);
  result.AddPointDropDuplicates(to_ - shift);
  result.AddPointDropDuplicates(to_ + shift);
  return result;
}

}  // namespace geometry