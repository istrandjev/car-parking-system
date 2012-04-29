#ifndef CAR_SIMULATION_CAR_SIMULATION_INTERSECTION_HANDLER_H_
#define CAR_SIMULATION_CAR_SIMULATION_INTERSECTION_HANDLER_H_

#include "geometry/regular_grid.h"

namespace geometry {
class BoundingBox;
class BoundaryLine;
class Segment;
}  // namespace geometry

namespace utils {

class ObjectHolder;
class BoundaryLinesHolder;

class IntersectionHandler {
 public:
  IntersectionHandler(double minx, double maxx, double miny, double maxy,
      BoundaryLinesHolder* boundary_lines_holder);

  void Init(const ObjectHolder& object_holder);

   void GetBoundaryLines(const geometry::BoundingBox& bounding_box, 
      std::vector<const geometry::BoundaryLine*>& result) const;
  
   void GetBoundaryLines(std::vector<const geometry::BoundaryLine*>& result) const;

 private: 
  void AddBoundaryLinesForObject(const geometry::RectangleObject* object);
  void RemoveSmallBoundaryLines();

 private:
  geometry::RegularGrid grid_;
  BoundaryLinesHolder* boundaryLinesHolder_;
};
}  // namespace utils

#endif  // CAR_SIMULATION_CAR_SIMULATION_INTERSECTION_HANDLER_H_
