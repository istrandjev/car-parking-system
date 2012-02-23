#include "geometry/boundary_line.h"

namespace geometry {

BoundaryLine::BoundaryLine() : crossable_(false) {}

bool BoundaryLine::IsCrossable() const {
  return crossable_;
}

void BoundaryLine::SetIsCrossable(bool crossable) {
  crossable = crossable_;
}

}  // namespace geometry