#include "simulation/car_positions_graph_router.h"

#include "simulation/car.h"
#include "simulation/car_manuever.h"
#include "simulation/car_positions_graph.h"
#include "utils/double_utils.h"

#include <algorithm>
#include <queue>

using namespace std;

namespace simulation {
CarPositionsGraphRouter::CarPositionsGraphRouter(
    const CarPositionsGraph *graph) : graph_(graph) {}

std::vector<CarManuever> CarPositionsGraphRouter::GetRoute(
    int from_index) const {

  const vector<vector<GraphEdge> >& graph = graph_->GetGraph();
  int n = static_cast<int>(graph.size());

  const vector<int>& end_indices = graph_->GetFinalPositions();
  vector<bool> end_position(n, false);
  for (unsigned i = 0 ; i < end_indices.size(); ++i) {
    end_position[end_indices[i]] = true;
  }

  vector<double> dist(n, -1.0);
  dist[from_index] = 0.0;

  priority_queue<pair<double, int> > q;
  q.push(make_pair(0, from_index));

  vector<pair<int, int> > parent(n);
  parent[from_index] = make_pair(from_index, 0);

  vector<bool> visited(n, false);

  int end_index = -1;
  while (!q.empty()) {
    int index = q.top().second;
    double d = -q.top().first;

    q.pop();
    if (visited[index]) {
      continue;
    }
    visited[index] = true;

    // Found an end position - no need to continue searching.
    if (end_position[index]) {
      end_index = index;
      break;
    }

    for (unsigned i = 0; i < graph[index].size(); ++i) {
      double new_dist = d + graph[index][i].second.GetTotalDistance();
      int neighbour_index = graph[index][i].first;
      if (visited[neighbour_index]) {
        continue;
      }
      if (dist[neighbour_index] < 0 ||
          DoubleIsGreater(dist[neighbour_index], new_dist)) {
        parent[neighbour_index] = make_pair(index, i);
        dist[neighbour_index] = new_dist;
        q.push(make_pair(-dist[neighbour_index], neighbour_index));
      }
    }
  }
  vector<CarManuever> result;

  // No route to end position found
  if (end_index == -1) {
    return result;
  }
  int current = end_index;
  while (parent[current].first != current) {
    const GraphEdge& edge =
        graph[parent[current].first][parent[current].second];
    result.push_back(edge.second);
    current = parent[current].first;
  }
  reverse(result.begin(), result.end());

  return result;
}

}  // namespace simulation
