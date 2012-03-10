#ifndef CAR_SIMULATION_CAR_SIMULATION_STRAIGHT_BOUNDARY_LINE_H_
#define CAR_SIMULATION_CAR_SIMULATION_STRAIGHT_BOUNDARY_LINE_H_

#include "geometry/boundary_line.h"
#include "geometry/segment.h"

namespace geometry {

class StraightBoundaryLine : public BoundaryLine {
 public:
  StraightBoundaryLine(const Segment& segment);
  virtual double GetLength()  const;  
  virtual BoundingBox GetBoundingBox() const;
  
  Segment GetSegment() const;

 private:
  Segment segment_;
};

}  // namespace geometry

#endif  // CAR_SIMULATION_CAR_SIMULATION_STRAIGHT_BOUNDARY_LINE_H_
