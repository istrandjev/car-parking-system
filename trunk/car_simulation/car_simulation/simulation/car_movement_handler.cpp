#include "simulation/car_movement_handler.h"

#include "geometry/arc.h"
#include "geometry/bounding_box.h"
#include "geometry/boundary_line.h"
#include "geometry/geometry_utils.h"
#include "geometry/polygon.h"
#include "geometry/rectangle_object.h"
#include "geometry/straight_boundary_line.h"
#include "geometry/vector.h"
#include "simulation/car.h"
#include "utils/current_state.h"
#include "utils/double_utils.h"
#include "utils/intersection_handler.h"

namespace simulation {

bool IntersectsSectionBetweenConcentricArcs(
    const geometry::Arc& arc1, const geometry::Arc& arc2,
    const std::vector<geometry::Segment>& segments);

bool IntersectsSectionBetweenConcentricArcs(
    const geometry::Arc& arc1, const geometry::Arc& arc2,
    const geometry::Segment& segment);

bool SectionBetweenConcentricArcsContains(
    const geometry::Arc& arc1, const geometry::Arc& arc2,
    const geometry::Point& point);

// static
bool CarMovementHandler::CarMovementPossibleByDistance(const Car& car, double distance,
      const utils::IntersectionHandler& intersection_handler) {
  if (!DoubleIsZero(car.GetCurrentSteeringAngle())) {
    geometry::Point rotation_center = car.GetRotationCenter();
    geometry::Point center = car.GetCenter();
    double radius = rotation_center.GetDistance(center);

    double angle = distance / radius;
    return CarMovementPossibleByAngle(car, angle, intersection_handler);
  }

  geometry::Vector direction = car.GetDirection().Unit();
  geometry::Point from = car.GetCenter() - direction * car.GetLength() * 0.5;
  geometry::Point to = from + direction * (distance + car.GetLength() * 0.5);

  geometry::RectangleObject ro(from, to, car.GetWidth());

  geometry::BoundingBox bounding_box = ro.GetBoundingBox();
  std::vector<const geometry::BoundaryLine*> lines = 
      intersection_handler.GetBoundaryLines(bounding_box);

  geometry::Polygon bounds = ro.GetBounds();
  for (unsigned index = 0; index < lines.size(); ++index) {
    const geometry::StraightBoundaryLine* line =
        dynamic_cast<const geometry::StraightBoundaryLine*>(lines[index]);
    if (geometry::Intersect(bounds, line->GetSegment(), NULL)) {
      return false;
    }
  }
  return true;
}

bool CarMovementHandler::CarMovementPossibleByAngle(const Car& car, double angle,
      const utils::IntersectionHandler& intersection_handler) {
  if (DoubleIsZero(car.GetCurrentSteeringAngle())) {
    return true;
  } 
  if (DoubleIsGreater(angle, geometry::GeometryUtils::PI * 2.0)) {
    angle = geometry::GeometryUtils::PI * 2.0;
  }
  
  geometry::BoundingBox bounding_box;

  geometry::Point center = car.GetRotationCenter();
  geometry::Polygon bounds = car.GetBounds();
  geometry::Vector direction = car.GetDirection();
  std::vector<geometry::Arc> arcs;
  for (unsigned index = 0; index < bounds.NumberOfVertices(); ++index) {
    const geometry::Point& point = bounds.GetPoint(index); 
    geometry::Vector temp(center, point);
    double actual_angle = angle;
    if (DoubleIsGreater(0, temp.CrossProduct(direction))) {
      actual_angle = -actual_angle;
    }
    arcs.push_back(geometry::Arc(center, point, actual_angle));
    bounding_box.UnionWith(arcs[index].GetBoundingBox());
  }

  std::vector<const geometry::BoundaryLine*> lines =
      intersection_handler.GetBoundaryLines(bounding_box);

  std::vector<geometry::Segment> segments;
  for (unsigned index = 0 ; index < lines.size(); ++index) {
    const geometry::StraightBoundaryLine* line =
        dynamic_cast<const geometry::StraightBoundaryLine*>(lines[index]);
    segments.push_back(line->GetSegment());
  }

  for (unsigned index1 = 0; index1 < bounds.NumberOfVertices(); ++index1) {
    for (unsigned index2 = index1 + 1; index2 < bounds.NumberOfVertices(); 
        ++index2) {
      if (IntersectsSectionBetweenConcentricArcs(arcs[index1], arcs[index2], 
          segments)) {
        return false;
      }
    }
  }

  return true;
}


bool IntersectsSectionBetweenConcentricArcs(
      const geometry::Arc& arc1, const geometry::Arc& arc2,
      const std::vector<geometry::Segment>& segments) {
  for (unsigned index = 0; index < segments.size(); ++index) {
    if (IntersectsSectionBetweenConcentricArcs(arc1, arc2, segments[index])) {
      return true; 
    }
  }
  return false;
}

bool IntersectsSectionBetweenConcentricArcs(
      const geometry::Arc& arc1, const geometry::Arc& arc2,
      const geometry::Segment& segment) {
  geometry::Point from1 = arc1.GetStartPoint();
  geometry::Point to1 = arc1.GetEndPoint();

  geometry::Point from2 = arc2.GetStartPoint();
  geometry::Point to2 = arc2.GetEndPoint();

  geometry::Segment from_segment(from1, from2);
  geometry::Segment to_segment(to1, to2);

  if (from_segment.Intersect(segment)) {
    return true;
  }

  if (to_segment.Intersect(segment)) {
    return true;
  }

  if (arc1.Intersect(segment)) {
    return true;
  }

  if (arc2.Intersect(segment)) {
    return true;
  }

  if (SectionBetweenConcentricArcsContains(arc1, arc2, segment.A()) && 
      SectionBetweenConcentricArcsContains(arc1, arc2, segment.B())) {
    return true;
  }

  return false;
}

bool SectionBetweenConcentricArcsContains(
    const geometry::Arc& arc1, const geometry::Arc& arc2,
    const geometry::Point& point) {
  geometry::Point center = arc1.GetCenter();
  if (center != arc2.GetCenter()) {
    throw new std::invalid_argument("The arcs are not concentric!");
  }

  double r1 = arc1.GetRadius();
  double r2 = arc2.GetRadius();

  double distance = center.GetDistance(point);
  if (!DoubleIsBetween(distance, r1, r2)) {
    return false;
  }


  // TODO(istrandjev): fix this to have real logic in it.
  return true;
}

}  // namespace simulation