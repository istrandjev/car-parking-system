#include "simulation/car_movement_handler.h"

#include "geometry/arc.h"
#include "geometry/bounding_box.h"
#include "geometry/boundary_line.h"
#include "geometry/geometry_utils.h"
#include "geometry/line.h"
#include "geometry/polygon.h"
#include "geometry/segment.h"
#include "geometry/rectangle_object.h"
#include "geometry/straight_boundary_line.h"
#include "geometry/vector.h"
#include "simulation/car.h"
#include "simulation/car_manuever.h"
#include "utils/current_state.h"
#include "utils/double_utils.h"
#include "utils/intersection_handler.h"

#include <cmath>
#include <stdexcept>

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

CarMovementHandler::CarMovementHandler(
    const utils::IntersectionHandler* intersection_handler)
    : intersectionHandler_(intersection_handler) {}

const utils::IntersectionHandler* 
    CarMovementHandler::GetIntersectionHandler() const {
  return intersectionHandler_;
}

// static
bool CarMovementHandler::CarMovementPossibleByDistance(
    const Car& car, double distance) const {
  if (!DoubleIsZero(car.GetCurrentSteeringAngle())) {
    geometry::Point rotation_center = car.GetRotationCenter();
    geometry::Point center = car.GetCenter();
    double radius = rotation_center.GetDistance(center);

    double angle = distance / radius;
    return CarMovementPossibleByAngle(car, angle);
  }

  geometry::Vector direction = car.GetDirection().Unit();
  geometry::Point from = car.GetCenter() - direction * car.GetLength() * 0.5;
  geometry::Point to = from + direction * (distance + car.GetLength());

  geometry::RectangleObject ro(from, to, car.GetWidth());

  geometry::BoundingBox bounding_box = ro.GetBoundingBox();
  std::vector<const geometry::BoundaryLine*> lines = 
      intersectionHandler_->GetBoundaryLines(bounding_box);

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

bool CarMovementHandler::CarMovementPossibleByAngle(
    const Car& car, double angle) const {
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
      intersectionHandler_->GetBoundaryLines(bounding_box);

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
    throw std::invalid_argument("The arcs are not concentric!");
  }

  double r1 = arc1.GetRadius();
  double r2 = arc2.GetRadius();

  double distance = center.GetDistance(point);
  if (!DoubleIsBetween(distance, r1, r2)) {
    return false;
  }

  const double pi = geometry::GeometryUtils::PI;

  double start1 = arc1.GetStartAngle();
  double end1 = arc1.GetEndAngle();
  if (DoubleIsGreater(start1, end1)) {
    end1 += pi * 2.0;
  }
  double start2 = arc2.GetStartAngle();
  double end2 = arc2.GetEndAngle();
  if (DoubleIsGreater(start2, end2)) {
    end2 += pi * 2.0;
  }

  double begin_angle = std::max(start1, start2);
  double end_angle = std::min(end1, end2);
  if (DoubleIsGreaterOrEqual(begin_angle, end_angle)) {
    return false;
  }
  
  double angle = atan2(point.y - center.y, point.x - center.x);
  angle = geometry::GeometryUtils::NormalizeAngle(angle);

  // Check if the point is within the common interval of angles for 
  // the two arcs.
  if (DoubleIsBetween(angle, begin_angle, end_angle) ||
      DoubleIsBetween(angle + 2.0 * pi, begin_angle, end_angle)) {
    return true;
  }

  // Check if the triangle in the begining of the two arcs contains the point.
  geometry::Point A1 = arc1.GetStartPoint();
  geometry::Point B1 = arc2.GetStartPoint();
  geometry::Point C1;
  if (DoubleIsGreater(start1, start2)) {
    C1 = arc2.GetCircle().GetPoint(start1);
  } else {
    C1 = arc1.GetCircle().GetPoint(start2);
  }

  if (geometry::GeometryUtils::TriangleContains(A1, B1, C1, point)) {
    return true;
  }

  // Check if the triangle at the end of the two arcs contains the point.
  geometry::Point A2 = arc1.GetEndPoint();
  geometry::Point B2 = arc2.GetEndPoint();
  geometry::Point C2;
  if (DoubleIsGreater(end1, end2)) {
    C2 = arc1.GetCircle().GetPoint(end2);
  } else {
    C2 = arc2.GetCircle().GetPoint(end1);
  }

  if (geometry::GeometryUtils::TriangleContains(A2, B2, C2, point)) {
    return true;
  }
  return false;
}

// static
bool CarMovementHandler::SingleManueverBetweenStates(
        const Car& car1, const Car& car2,
        CarManuever& manuever) const {
  const geometry::Vector& dir1 = car1.GetDirection().Unit();
  const geometry::Vector& dir2 = car2.GetDirection().Unit();

  const geometry::Point& center1 = car1.GetCenter();
  const geometry::Point& center2 = car2.GetCenter();

  if (DoubleIsZero(dir1.CrossProduct(dir2))) {
    geometry::Vector vector(center1, center2);

    // All four points lie on the same line
    if (DoubleIsZero(vector.CrossProduct(dir1))) {
      // Opposite directions - no solution
      if (DoubleIsGreaterOrEqual(0, dir1.DotProduct(dir2))) {
        return false;
      }

      double temp_distance = center1.GetDistance(center2);
      if (!CarMovementPossibleByDistance(car1, temp_distance)) {
          return false;
      } else {
          manuever.SetBeginPosition(car1);
          manuever.SetInitialStraightSectionDistance(temp_distance);
          return true;
      }
    }

    // Same direction - no solution
    if (DoubleIsGreaterOrEqual(dir1.DotProduct(dir2), 0)) {
      return false;
    }

    geometry::Segment central(center1, center2);
    geometry::Line l(central.GetMiddle(), dir1);
    geometry::Point rotation_center;
    l.Intersect(car1.GetRearWheelsAxis(), &rotation_center);
    return ConstructManuever(car1, car2, rotation_center, manuever);
  }

  geometry::Line l1(center1, dir1);
  geometry::Line l2(center2, dir2);

  // Avoid the case when center1 or center2 is the intersection
  if (DoubleIsZero(l1.GetDistanceFromPoint(center2)) ||
      DoubleIsZero(l2.GetDistanceFromPoint(center1))) {
    return false;
  }

  geometry::Point intersection;
  l1.Intersect(l2, &intersection);
  geometry::Line bisectrics = geometry::GeometryUtils::GetBisectrice(
        intersection + dir1, intersection, intersection + dir2);

  geometry::Point rotation_center;
  bisectrics.Intersect(car1.GetRearWheelsAxis(), &rotation_center);
  return ConstructManuever(car1, car2, rotation_center, manuever);
}

bool CarMovementHandler::ConstructManuever(
    const Car &car1, const Car &car2,
    const geometry::Point &rotation_center,
    CarManuever &manuever) const {
  if (!car1.CanBeRotationCenter(rotation_center)) {
    return false;
  }

  const geometry::Point& center1 = car1.GetCenter();
  const geometry::Point& center2 = car2.GetCenter();
  const geometry::Vector& dir2 = car2.GetDirection();
  const geometry::Vector& dir1 = car1.GetDirection();

  double angle = geometry::GeometryUtils::GetAngleBetweenVectors(
        dir1, dir2);

  if (!CarMovementPossibleByAngle(car1, angle)) {
    return false;
  }

  Car after_turn = car1;
  after_turn.SetDirection(dir2);
  geometry::Point center = center1.Rotate(rotation_center, angle);
  geometry::Vector vec(center, center2);
  
  if (DoubleIsGreaterOrEqual(0, vec.DotProduct(dir2))) {
    return false;
  }

  after_turn.SetCenter(center);
  double distance = center.GetDistance(center2);
  if (!CarMovementPossibleByDistance(after_turn, distance)) {
    return false;
  }

  manuever.SetBeginPosition(car1);
  manuever.SetTurnAngle(angle);
  manuever.SetRotationCenter(rotation_center);
  manuever.SetFinalStraightSectionDistance(distance);
  return true;
}

}  // namespace simulation
