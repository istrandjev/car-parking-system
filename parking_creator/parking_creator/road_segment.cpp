#include "road_segment.h"

#include "double_utils.h"
#include "geometry_utils.h"
#include "point.h"
#include "vector.h"

namespace geometry {

static const double DEAULT_WIDTH = 2.0;

RoadSegment::RoadSegment(const Point& from, const Point& to) 
    : from_(from), to_(to){}

RoadSegment::RoadSegment(const Point& from, const Point& to, double width)
    : from_(from), to_(to), width_(width) {}

const Point& RoadSegment::GetFrom() const {
  return from_;
}

void RoadSegment::SetFrom(const Point& point) {
  from_ = point;
}
  
const Point& RoadSegment::GetTo() const {
  return to_;
}

void RoadSegment::SetTo(const Point& point) {
  to_ = point;
}

double RoadSegment::GetWidth() const {
  return width_;
}

void RoadSegment::SetWidth(double width) {
  width_ = width;
}

void RoadSegment::ReverseDirection() {
  std::swap(from_, to_);
}

bool RoadSegment::ContainsPoint(const Point& p) const {
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

}  // namespace geometry