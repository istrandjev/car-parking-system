#ifndef CAR_SIMULATION_CAR_SIMULATION_BOUNDARY_LINE_H_
#define CAR_SIMULATION_CAR_SIMULATION_BOUNDARY_LINE_H_

#include "geometry/segment.h"

namespace geometry {

class BoundingBox;

class BoundaryLine {
 public:
  BoundaryLine();
  bool IsCrossable() const;
  void SetIsCrossable(bool crossable);

  virtual BoundingBox GetBoundingBox() const = 0;

 private:
  bool crossable_;
};

}  // namespace geometry

#endif  // CAR_SIMULATION_CAR_SIMULATION_BOUNDARY_LINE_H_
