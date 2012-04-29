#ifndef SIMUALTION_CAR_POSITIONS_GRAPH_H
#define SIMUALTION_CAR_POSITIONS_GRAPH_H

#include "simulation/car_manuever.h"
#include "simulation/car_positions_container.h"

#include <vector>

namespace geometry {
class RectangleObject;
}  // namespace geometry

namespace simulation {

class CarMovementHandler;
class Car;

typedef std::pair<int, CarManuever> GraphEdge;

class CarPositionsGraph {
 public:
  CarPositionsGraph(const CarMovementHandler* movement_handler);

  void AddPosition(bool final, const Car& position,
                   const geometry::RectangleObject* object);
  void ConstructGraph();

  const std::vector<int>& GetFinalPositions() const;
  const std::vector<std::vector<GraphEdge> >& GetGraph() const;

 private:
  void GetPositionNeighbours(int position_index,
      std::vector<std::pair<int, CarManuever> >& neighbours);

 private:
  std::vector<std::vector<std::pair<int, CarManuever> > > graph_;
  std::vector<int> final_;
  CarPositionsContainer positionsContainer_;
  const CarMovementHandler* movementHandler_;
};
}  // namespace simulation
#endif // SIMUALTION_CAR_POSITIONS_GRAPH_H
