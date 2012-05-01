#ifndef UTILS_CAR_POSITIONS_GRAPH_BUILDER_H
#define UTILS_CAR_POSITIONS_GRAPH_BUILDER_H

#include "utils/intersection_handler.h"
#include "utils/object_holder.h"

namespace geometry {
class RectangleObject;
}  // namespace geometry

namespace simulation {
class CarDescription;
class CarPosition;
class CarPositionsGraph;
}  // namespace simulation

namespace utils {
class CarPositionsGraphBuilder {
 public:
  CarPositionsGraphBuilder(const ObjectHolder& object_holder,
                           const IntersectionHandler& intersection_handler);

  void CreateCarPositionsGraph(simulation::CarPositionsGraph* graph) const;

  static double GetSamplingStep();

 private:
  void AddPositionsForObject(
      const geometry::RectangleObject* object, bool final,
      simulation::CarPositionsGraph* graph) const;
  bool CarPositionIsPossible(const simulation::CarDescription& car_description,
                             const simulation::CarPosition& car_position) const;

 private:
  static const double SAMPLING_STEP;
  const ObjectHolder& objectHolder_;
  const IntersectionHandler& intersectionHandler_;
};

}  // namespace utils
#endif // UTILS_CAR_POSITIONS_GRAPH_BUILDER_H
