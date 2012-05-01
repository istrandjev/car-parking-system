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
    wheelAxisFraction_(WHEEL_AXIS_FRACTION) {}

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
  geometry::Line rear_axis = GetRearWheelsAxis(car_position);
  if(!DoubleIsZero(rear_axis.GetDistanceFromPoint(center))) {
    return false;
  }

  const geometry::Point& fl = GetFrontLeftWheelCenter(car_position);
  const geometry::Point& fr = GetFrontRightWheelCenter(car_position);
  const geometry::Point& rl = GetRearLeftWheelCenter(car_position);
  const geometry::Point& rr = GetRearRightWheelCenter(car_position);

  const double pi = geometry::GeometryUtils::PI;
  if (!geometry::GeometryUtils::PointsAreInSameSemiPlane(
      rl, fl, rr, center, true)) {
    geometry::Vector turn_vector(center, fl);
    geometry::Vector rear_axis_vector(center, rl);
    double angle = geometry::GeometryUtils::GetAngleBetweenVectors(
        rear_axis_vector, turn_vector);
    if (DoubleIsGreater(angle, pi)) {
      angle = pi * 2.0 -  angle;
    }
    return DoubleIsBetween(angle, 0, maxSteeringAngle_);
  } else if (!geometry::GeometryUtils::PointsAreInSameSemiPlane(
                rr, fr, rl, center, true)) {
    geometry::Vector turn_vector(center, fr);
    geometry::Vector rear_axis_vector(center, rr);
    double angle = geometry::GeometryUtils::GetAngleBetweenVectors(
        turn_vector, rear_axis_vector);
    if (DoubleIsGreater(angle, pi)) {
      angle = pi * 2.0 -  angle;
    }
    return DoubleIsBetween(angle, 0, maxSteeringAngle_);
  } else {
    return false;
  }
}

std::ostream& operator<<(
    std::ostream& out, const CarDescription& car_description) {
  out << "Width:" << car_description.width_
      << " Length: " << car_description.length_
      << " Max steering angle: " << car_description.maxSteeringAngle_ << "\n";
  return out;
}

}  // namespace simulation
