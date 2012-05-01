#include "utils/car_positions_graph_builder.h"

#include "geometry/bounding_box.h"
#include "geometry/boundary_line.h"
#include "geometry/geometry_utils.h"
#include "geometry/point.h"
#include "geometry/polygon.h"
#include "geometry/rectangle_object.h"
#include "geometry/straight_boundary_line.h"
#include "geometry/vector.h"
#include "simulation/car_description.h"
#include "simulation/car_positions_graph.h"
#include "utils/double_utils.h"
#include "utils/intersection_handler.h"
#include "utils/object_holder.h"

namespace utils {

// Static declaration
const double CarPositionsGraphBuilder::SAMPLING_STEP = 1.0;

CarPositionsGraphBuilder::CarPositionsGraphBuilder(
    const ObjectHolder &object_holder,
    const IntersectionHandler &intersection_handler)
  : intersectionHandler_(intersection_handler),
    objectHolder_(object_holder) {}

void CarPositionsGraphBuilder::CreateCarPositionsGraph(
    simulation::CarPositionsGraph *graph) const {
  const RectangleObjectContainer& roads = objectHolder_.GetRoadSegments();
  for (unsigned i = 0; i < roads.size(); ++i) {
    AddPositionsForObject(roads[i], false, graph);
  }

  const RectangleObjectContainer& parking_lots = objectHolder_.GetParkingLots();
  for (unsigned i = 0; i < parking_lots.size(); ++i) {
    AddPositionsForObject(parking_lots[i], true, graph);
  }

  graph->ConstructGraph();
}

double CarPositionsGraphBuilder::GetSamplingStep() {
  return SAMPLING_STEP;
}

void CarPositionsGraphBuilder::AddPositionsForObject(
    const geometry::RectangleObject* object, bool final,
    simulation::CarPositionsGraph* graph) const {
  const geometry::Point& from = object->GetFrom();
  const geometry::Point& to = object->GetTo();
  double length = from.GetDistance(to);

  double width = object->GetWidth();
  geometry::Vector ox = geometry::Vector(from, to).Unit();
  geometry::Vector oy = ox.GetOrthogonal().Unit();

  geometry::Point origin = from - oy * width * 0.5;
  std::vector<double> y_fractions;
  for (double u = 0; u < width; u += SAMPLING_STEP) {
    y_fractions.push_back(u);
  }
  if (DoubleIsGreater(width, y_fractions.back() + SAMPLING_STEP * 0.5)) {
    y_fractions.push_back(width);
  }
  std::vector<double> x_fractions;
  for (double u = 0; u < length; u += SAMPLING_STEP) {
    x_fractions.push_back(u);
  }
  if (DoubleIsGreater(length, x_fractions.back() + SAMPLING_STEP * 0.5)) {
    x_fractions.push_back(length);
  }
  const simulation::CarDescription& description = graph->GetCarDescription();
  const double pi = geometry::GeometryUtils::PI;
  const double angle_step = pi / 10;
  for (unsigned i = 0; i < y_fractions.size();++i) {
    for (unsigned j = 0; j < x_fractions.size();++j) {
      geometry::Point center = origin + ox * x_fractions[j] +
           oy * y_fractions[i];
      for (double angle = 0; angle < 1.999 * pi; angle += angle_step) {
        simulation::CarPosition car_position;
        car_position.SetCenter(center);
        car_position.SetDirection(ox.Rotate(angle));
        car_position.SetIsFinal(final);
        if (final) {
          geometry::Polygon bounds;
          description.GetBounds(car_position, bounds);
          for (unsigned vert = 0; vert < bounds.NumberOfVertices(); ++vert) {
            if (!object->ContainsPoint(bounds.GetPoint(vert))) {
              car_position.SetIsFinal(false);
              break;
            }
          }
        }

        if (CarPositionIsPossible(description, car_position)) {
          if (DoubleIsZero(angle) || DoubleEquals(angle, pi)) {
            car_position.SetIsAlongBaseLine(true);
          }
          graph->AddPosition(car_position, object);
        }
      }
    }
  }
}

bool CarPositionsGraphBuilder::CarPositionIsPossible(
    const simulation::CarDescription& car_description,
    const simulation::CarPosition& car_position) const {
  geometry::Polygon bounds;
  car_description.GetBounds(car_position, bounds);
  std::vector<const geometry::BoundaryLine*> lines;
  intersectionHandler_.GetBoundaryLines(bounds.GetBoundingBox(), &lines);

  for (unsigned i = 0; i < lines.size(); ++i) {
    const geometry::StraightBoundaryLine* straight_line =
        dynamic_cast<const geometry::StraightBoundaryLine*>(lines[i]);
    if (geometry::Intersect(bounds, straight_line->GetSegment(), NULL)) {
      return false;
    }
  }

  return true;
}

}  // namespace utils
