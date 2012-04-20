#ifndef CAR_SIMULATION_CAR_SIMULATION_BOUNDARY_LINES_HOLDER_H
#define CAR_SIMULATION_CAR_SIMULATION_BOUNDARY_LINES_HOLDER_H

#include <vector>

namespace geometry { 
class BoundaryLine;  
class Segment;
}  // namespace geometry

namespace utils {

class BoundaryLinesHolder {
 public:
  ~BoundaryLinesHolder();

  geometry::BoundaryLine* AddStraightBoundaryLine(
      const geometry::Segment& line, bool crossable);

 private:
  std::vector<geometry::BoundaryLine*> boundaryLines_;
};

}  // namespace utils

#endif  // CAR_SIMULATION_CAR_SIMULATION_BOUNDARY_LINES_HOLDER_H
