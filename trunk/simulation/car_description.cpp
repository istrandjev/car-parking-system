#include "simulation/car_description.h"

#include "geometry/geometry_utils.h"
#include "geometry/line.h"
#include "geometry/point.h"
#include "geometry/polygon.h"
#include "simulation/car_position.h"
#include "utils/double_utils.h"

namespace simulation {

static const double WHEEL_AXIS_FRACTION = 0.62;

CarDescription::CarDescription(double width, double length,
    double max_steering_angle) : width_(width), length_(length),
    maxSteeringAngle_(max_steering_angle),
    wheelAxisFraction_(WHEEL_AXIS_FRACTION) {
  CalculateMinDistFromMainAxis();
}

double CarDescription::GetMaxSteeringAngle() const {
  return maxSteeringAngle_;
}

double CarDescription::GetWidth() const {
  return width_;
}

double CarDescription::GetLength() const {
    return length_;
}

double CarDescription::GetWheelAxisFraction() const {
  return wheelAxisFraction_;
}

void CarDescription::GetBounds(const CarPosition &position,
                               geometry::Polygon &bounds) const {
  bounds.Reset();
  geometry::Vector to_front = position.GetDirection().Unit() * length_ * 0.5;
  geometry::Vector to_side = position.GetDirection().GetOrthogonal().Unit() *
      width_ * 0.5;

  bounds.AddPointDropDuplicates(position.GetCenter() + to_front + to_side);
  bounds.AddPointDropDuplicates(position.GetCenter() + to_front - to_side);
  bounds.AddPointDropDuplicates(position.GetCenter() - to_front - to_side);
  bounds.AddPointDropDuplicates(position.GetCenter() - to_front + to_side);
  bounds.Normalize();
}

geometry::Line CarDescription::GetRearWheelsAxis(
    const CarPosition& position) const{
  return geometry::Line(GetRearLeftWheelCenter(position),
      GetRearRightWheelCenter(position)); 
}

geometry::Point CarDescription::GetFrontLeftWheelCenter(
    const CarPosition& position) const {
  return position.GetCenter() 
      + position.GetDirection() * length_ * 0.5 * WHEEL_AXIS_FRACTION
      + position.GetDirection().GetOrthogonal() * width_ * 0.5;
}

geometry::Point CarDescription::GetFrontRightWheelCenter(
    const CarPosition& position) const {
  return position.GetCenter() 
      + position.GetDirection() * length_ * 0.5 * WHEEL_AXIS_FRACTION
      - position.GetDirection().GetOrthogonal() * width_ * 0.5;
}

geometry::Point CarDescription::GetRearLeftWheelCenter(
    const CarPosition& position) const {
  return position.GetCenter() 
      - position.GetDirection() * length_ * 0.5 * WHEEL_AXIS_FRACTION
      + position.GetDirection().GetOrthogonal() * width_ * 0.5;
}

geometry::Point CarDescription::GetRearRightWheelCenter(
    const CarPosition& position) const {
  return position.GetCenter() 
      - position.GetDirection() * length_ * 0.5 * WHEEL_AXIS_FRACTION
      - position.GetDirection().GetOrthogonal() * width_ * 0.5;
}

bool CarDescription::CanBeRotationCenter(const CarPosition &car_position,
                                         const geometry::Point &center) const {
  geometry::Line main_axis(car_position.GetCenter(),
                           car_position.GetDirection());

  double dist = main_axis.GetDistanceFromPoint(center);
  double min_dist = minDistFromMainAxis_;
  if (DoubleIsGreater(min_dist, dist)) {
    return false;
  }
  geometry::Line rear_axis = GetRearWheelsAxis(car_position);
  return DoubleIsZero(rear_axis.GetDistanceFromPoint(center));
}

std::ostream& operator<<(
    std::ostream& out, const CarDescription& car_description) {
  out << "Width:" << car_description.width_
      << " Length: " << car_description.length_
      << " Max steering angle: " << car_description.maxSteeringAngle_ << "\n";
  return out;
}

void CarDescription::CalculateMinDistFromMainAxis() {
  CarPosition basic;
  basic.SetCenter(geometry::Point(0,0));
  basic.SetDirection(geometry::Vector(1, 0));

  geometry::Line rw_axis = GetRearWheelsAxis(basic);
  geometry::Line main_axis(basic.GetCenter(), basic.GetDirection());

  const double pi = geometry::GeometryUtils::PI;
  geometry::Point flw = GetFrontLeftWheelCenter(basic);
  geometry::Vector dir = basic.GetDirection().Rotate(maxSteeringAngle_ + pi * 0.5);
  geometry::Line l1(flw, dir);
  geometry::Point intersection;
  l1.Intersect(rw_axis, &intersection);

  minDistFromMainAxis_ = fabs(main_axis.GetDistanceFromPoint(intersection));
}

}  // namespace simulation
