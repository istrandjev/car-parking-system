#include "geometry/straight_boundary_line.h"

#include "geometry/segment.h"
#include "geometry/bounding_box.h"

namespace geometry {
StraightBoundaryLine::StraightBoundaryLine(const Segment& segment)
    : segment_(segment) {}

// virtual
double StraightBoundaryLine::GetLength()  const {
  return segment_.Length();
}

// virtual
BoundingBox StraightBoundaryLine::GetBoundingBox() const {
  return segment_.GetBoundingBox();  
}

Segment StraightBoundaryLine::GetSegment() const {
  return segment_;
}

}  // namespace geometry