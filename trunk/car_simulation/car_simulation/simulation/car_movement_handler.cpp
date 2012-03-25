#include "simulation/car_movement_handler.h"

#include "geometry/bounding_box.h"
#include "geometry/boundary_line.h"
#include "geometry/geometry_utils.h"
#include "geometry/polygon.h"
#include "geometry/rectangle_object.h"
#include "geometry/straight_boundary_line.h"
#include "geometry/vector.h"
#include "simulation/car.h"
#include "utils/double_utils.h"
#include "utils/intersection_handler.h"

namespace simulation {

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
  

}

}  // namespace simulation