#ifndef SIMULATION_CAR_POSITIONS_GRAPH_ROUTER_H
#define SIMULATION_CAR_POSITIONS_GRAPH_ROUTER_H

#include <vector>

namespace simulation {

class CarPositionsGraph;
class CarManuever;
class Car;

class CarPositionsGraphRouter {
 public:
  CarPositionsGraphRouter(const CarPositionsGraph* graph);

  std::vector<CarManuever> GetRoute(int from_index) const;

 private:
  const CarPositionsGraph* graph_;
};

}  // namespace simulation
#endif // SIMULATION_CAR_POSITIONS_GRAPH_ROUTER_H
