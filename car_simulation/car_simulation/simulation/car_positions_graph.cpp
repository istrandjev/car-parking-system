#include "simulation/car_positions_graph.h"

#include "geometry/rectangle_object.h"
#include "simulation/car.h"
#include "simulation/car_movement_handler.h"
#include "utils/benchmark.h"
#include "utils/delay.h"
#include "utils/double_utils.h"

#include <iomanip>

namespace simulation {

static const double MIN_X_COORDINATE = -1000.0;
static const double MAX_X_COORDINATE = 1000.0;
static const double MIN_Y_COORDINATE = -1000.0;
static const double MAX_Y_COORDINATE = 1000.0;

CarPositionsGraph::CarPositionsGraph(const CarMovementHandler *movement_handler)
  : movementHandler_(movement_handler),
  positionsContainer_(MIN_X_COORDINATE, MAX_X_COORDINATE,
                      MIN_Y_COORDINATE, MAX_Y_COORDINATE) {}

void CarPositionsGraph::AddPosition(const CarPosition &position,
                                    const geometry::RectangleObject* object) {
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
    neighbour_list[i].push_back(i);
    for (unsigned j = i + 1; j < number_of_objects; ++j) {
      if (geometry::AreTouching(*positionsContainer_.GetObject(i),
                                *positionsContainer_.GetObject(j))) {
        neighbour_list[i].push_back(j);
        neighbour_list[j].push_back(i);
      }
    }
  }
}

void CarPositionsGraph::FinalizeGraph() {
  std::cerr << "Number of positions to build graph from: "
            << positionsContainer_.GetNumberOfPositions() << std::endl;

//  double start_time = get_time();
  numberOfVertices_ = positionsContainer_.GetNumberOfPositions();
  GetNeighbourhoodList(neighbourhoodList_);
  graph_.resize(numberOfVertices_);
  neighboursComputed_.resize(numberOfVertices_, false);
//  int number_of_vertices = positionsContainer_.GetNumberOfPositions();
//  graph_.resize(number_of_vertices);

//  std::vector<std::vector<int> > neighbourhood_list;
//  GetNeighbourhoodList(neighbourhood_list);
//  int broi = 0;
//  for (int i = 0; i < number_of_vertices; ++i) {
//    if ((i+1) % 100 == 0) {
//      std::cerr << "Now working on vertex: " << i << "\n";
//      std::cerr << "Total edges: " << num << "\n";
//      std::cerr << "Time consumed so far:" << std::setprecision(8)
//                << get_time() - start_time << std::endl;
//      utils::Benchmark::DumpBenchmarkingInfo();
//    }
//    GetPositionNeighbours(i, neighbourhood_list, graph_[i]);
//  }
}

const CarDescription& CarPositionsGraph::GetCarDescription() const {
  return movementHandler_->GetCarDescription();
}

//const std::vector<std::vector<GraphEdge> >&
//    CarPositionsGraph::GetGraph() const {
//  return graph_;
//}

bool CarPositionsGraph::IsPositionFinal(int position_index) const {
  return positionsContainer_.GetPosition(position_index)->IsFinal();
}

int CarPositionsGraph::GetNumberOfVertices() const {
  return graph_.size();
}

const CarPosition* CarPositionsGraph::GetPosition(int position_index) const {
  return positionsContainer_.GetPosition(position_index);
}

const std::vector<GraphEdge>&
    CarPositionsGraph::GetNeighbours(int position_index) {
  if (!neighboursComputed_[position_index]) {
    GetPositionNeighbours(position_index);
    neighboursComputed_[position_index] = true;
  }
  return graph_[position_index];
}

void CarPositionsGraph::GetPositionNeighbours(int position_index) {
  int object_index = positionsContainer_.
      GetObjectIndexForPosition(position_index);
  const CarPosition* car = positionsContainer_.GetPosition(position_index);
  positionsContainer_.GetObject(object_index);
  for (unsigned ne_idx = 0; ne_idx < neighbourhoodList_[object_index].size();
       ++ne_idx) {
    const std::vector<int>& positions = positionsContainer_.
        GetCarPositionsForObject(neighbourhoodList_[object_index][ne_idx]);
    for (unsigned pos_index = 0; pos_index < positions.size(); ++pos_index) {
      if (positions[pos_index] == position_index) {
        continue;
      }
      CarManuever manuever;
      const CarPosition* car2 =
          positionsContainer_.GetPosition(positions[pos_index]);

      if (neighboursComputed_[positions[pos_index]]) {
        continue;
      }

      if (movementHandler_->SingleManueverBetweenStates(
          *car, *car2, manuever)) {
        graph_[position_index].push_back(
              std::make_pair(positions[pos_index], manuever));

        // Add the reversed manuever.
        manuever.SetReversed(true);
        graph_[positions[pos_index]].push_back(
              std::make_pair(position_index, manuever));
      } else if (movementHandler_->SingleManueverBetweenStates(
          *car2, *car, manuever)) {
        graph_[positions[pos_index]].push_back(
              std::make_pair(position_index, manuever));

        // Add the reversed manuever.
        manuever.SetReversed(true);
        graph_[position_index].push_back(
              std::make_pair(positions[pos_index], manuever));
      }
    }
  }
}


}  // namespace simulation
