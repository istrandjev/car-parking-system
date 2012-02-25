#include "utils/boundary_line_holder.h"

#include "geometry/boundary_line.h"
#include "geometry/straight_boundary_line.h"

#include <vector>

namespace utils {

BoundaryLinesHolder::~BoundaryLinesHolder() {
  for (unsigned index = 0; index < boundaryLines_.size(); ++index) {
    delete boundaryLines_[index];
  }
}

geometry::BoundaryLine* BoundaryLinesHolder::AddStraightBoundaryLine(
    const geometry::Segment& line, bool crossable) {
  geometry::StraightBoundaryLine* straight_boundary_line =
     new geometry::StraightBoundaryLine(line);
  straight_boundary_line->SetIsCrossable(crossable);
  boundaryLines_.push_back(straight_boundary_line);
  return straight_boundary_line;
}

}  // namespace utils