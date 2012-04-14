#include "simulation/car_positions_graph.h"

#include "simulation/car_movement_handler.h"

namespace simulation {

static const double MIN_X_COORDINATE = -1000.0;
static const double MAX_X_COORDINATE = 1000.0;
static const double MIN_Y_COORDINATE = -1000.0;
static const double MAX_Y_COORDINATE = 1000.0;

CarPositionsGraph::CarPositionsGraph(const CarMovementHandler *movement_handler)
  : movementHandler_(movement_handler),
  positionsContainer_(MIN_X_COORDINATE, MAX_X_COORDINATE, MIN_Y_COORDINATE, MAX_Y_COORDINATE) {}

void CarPositionsGraph::AddPosition(const Car &position) {
  positionsContainer_.AddCarPosition(position);
}

void CarPositionsGraph::ConstructGraph() {
  int number_of_vertices = positionsContainer_.GetNumberOfPositions();
  graph_.resize(number_of_vertices);

  for (int i = 0; i < number_of_vertices; ++i) {
    GetPositionNeighbours(i, graph_[i]);
  }
}

void CarPositionsGraph::GetPositionNeighbours(
    int position_index, std::vector<std::pair<int, CarManuever> >& neighbours) {
  int number_of_vertices = positionsContainer_.GetNumberOfPositions();
  for (int i = 0; i < number_of_vertices; ++i) {
    if (i == position_index) {
      continue;
    }
    const Car* car1 = positionsContainer_.GetPosition(position_index);
    const Car* car2 = positionsContainer_.GetPosition(i);
    CarManuever manuever;
    if (movementHandler_->SingleManueverBetweenStates(*car1, *car2, manuever)) {
      neighbours.push_back(std::make_pair(i, manuever));
    }
  }
}


}  // namespace simulation
