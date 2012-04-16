#ifndef UTILS_CAR_POSITIONS_GRAPH_BUILDER_H
#define UTILS_CAR_POSITIONS_GRAPH_BUILDER_H

#include "utils/intersection_handler.h"
#include "utils/object_holder.h"

namespace geometry {
class RectangleObject;
}  // namespace geometry

namespace simulation {
class Car;
class CarPositionsGraph;
}  // namespace simulation

namespace utils {
class CarPositionsGraphBuilder {
 public:
  CarPositionsGraphBuilder(const ObjectHolder& object_holder,
                           const IntersectionHandler& intersection_handler,
                           const simulation::Car& car);

  void CreateCarPositionsGraph(simulation::CarPositionsGraph* graph) const;

 private:
  void AddPositionsForObject(
      const geometry::RectangleObject* object, bool final,
      simulation::CarPositionsGraph* graph) const;
  bool CarPositionIsPossible(const simulation::Car& car) const;

 private:
  const ObjectHolder& objectHolder_;
  const IntersectionHandler& intersectionHandler_;
  const simulation::Car& car_;
};

}  // namespace utils
#endif // UTILS_CAR_POSITIONS_GRAPH_BUILDER_H
