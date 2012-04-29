#include "simulation/car_positions_graph.h"

#include "geometry/rectangle_object.h"
#include "simulation/car_movement_handler.h"
#include "utils/delay.h"

#include <iomanip>

namespace simulation {

static const double MIN_X_COORDINATE = -1000.0;
static const double MAX_X_COORDINATE = 1000.0;
static const double MIN_Y_COORDINATE = -1000.0;
static const double MAX_Y_COORDINATE = 1000.0;

int num = 0;
CarPositionsGraph::CarPositionsGraph(const CarMovementHandler *movement_handler)
  : movementHandler_(movement_handler),
  positionsContainer_(MIN_X_COORDINATE, MAX_X_COORDINATE,
                      MIN_Y_COORDINATE, MAX_Y_COORDINATE) {}

void CarPositionsGraph::AddPosition(bool final, const Car &position,
                                    const geometry::RectangleObject* object) {
  if (final) {
    final_.push_back(positionsContainer_.GetNumberOfPositions());
  }
  positionsContainer_.AddCarPosition(position, object);
  if (positionsContainer_.GetNumberOfPositions() % 1000 == 0) {
    std::cerr << "The size of the graph is now:"
              << positionsContainer_.GetNumberOfPositions() << std::endl;
  }
}

void CarPositionsGraph::GetNeighbourhoodList(
    std::vector<std::vector<int> > &neighbour_list) {
  unsigned number_of_objects = positionsContainer_.GetNumberOfObjects();
  neighbour_list.clear();
  neighbour_list.resize(number_of_objects);
  for (unsigned i = 0; i < number_of_objects; ++i) {
    for (unsigned j = i + 1; j < number_of_objects; ++j) {
      if (geometry::AreTouching(*positionsContainer_.GetObject(i),
                                *positionsContainer_.GetObject(j))) {
        neighbour_list[i].push_back(j);
        neighbour_list[j].push_back(i);
      }
    }
  }
}

void CarPositionsGraph::ConstructGraph() {
  std::cerr << "Number of positions to build graph from: "
            << positionsContainer_.GetNumberOfPositions() << std::endl;

  double start_time = get_time();

  int number_of_vertices = positionsContainer_.GetNumberOfPositions();
  graph_.resize(number_of_vertices);

  std::vector<std::vector<int> > neighbourhood_list;
  GetNeighbourhoodList(neighbourhood_list);

  for (int i = 0; i < number_of_vertices; ++i) {
    if ((i+1) % 100 == 0) {
      std::cerr << "Now working on vertex: " << i << "\n";
      std::cerr << "Total edges: " << num << "\n";
      std::cerr << "Time consumed so far:" << std::setprecision(8)
                << get_time() - start_time << std::endl;
    }
    GetPositionNeighbours(i, neighbourhood_list, graph_[i]);
  }
}

const std::vector<int>& CarPositionsGraph::GetFinalPositions() const {
  return final_;
}

const std::vector<std::vector<GraphEdge> >&
    CarPositionsGraph::GetGraph() const {
  return graph_;
}

void CarPositionsGraph::GetPositionNeighbours(int position_index,
      const std::vector<std::vector<int> >& neighbourhood_list,
      std::vector<std::pair<int, CarManuever> >& neighbours) {
  int object_index = positionsContainer_.
      GetObjectIndexForPosition(position_index);
  const Car* car = positionsContainer_.GetPosition(position_index);
  for (unsigned ne_idx = 0; ne_idx < neighbourhood_list[object_index].size();
       ++ne_idx) {
    const std::vector<int>& positions = positionsContainer_.
        GetCarPositionsForObject(neighbourhood_list[object_index][ne_idx]);
    for (unsigned pos_index = 0; pos_index < positions.size(); ++pos_index) {
      CarManuever manuever;
      const Car* car2 = positionsContainer_.GetPosition(positions[pos_index]);
      if (movementHandler_->SingleManueverBetweenStates(
          *car, *car2, manuever)) {
        neighbours.push_back(std::make_pair(positions[pos_index], manuever));
        num++;
      }
    }
  }
}


}  // namespace simulation
