#include "simulation/car_manuever.h"

#include "simulation/car.h"
#include "utils/double_utils.h"

namespace simulation {

CarManuever::CarManuever(
    const simulation::Car& begin_position,
    const geometry::Point& rotation_center,
    double steering_angle,
    double distance)
        : beginPosition_(begin_position),
          rotationCenter_(rotation_center),
          steeringAngle_(steering_angle),
          distance_(distance) {}

CarManuever::CarManuever(
    const simulation::Car& begin_position, double distance)
  : beginPosition_(begin_position), steeringAngle_(0.0), distance_(distance) {}

CarManuever::CarManuever()
  : beginPosition_(0.0, 0.0, 0.0), steeringAngle_(0.0), distance_(0.0) {}

void CarManuever::SetSteeringAngle(double steering_angle) {
  steeringAngle_ = steering_angle;
}

double CarManuever::GetSteeringAngle() const {
  return steeringAngle_;
}

void CarManuever::SetDistance(double distance) {
  distance_ = distance;
}

double CarManuever::GetDistance() const {
  return distance_;
}

void CarManuever::SetRotationCenter(const geometry::Point& center) {
  rotationCenter_ = center;
}

const geometry::Point& CarManuever::GetRotationCenter() const {
  return rotationCenter_;
}

void CarManuever::SetBeginPosition(const simulation::Car& begin_position) {
  beginPosition_ = begin_position;
}

simulation::Car CarManuever::GetBeginPosition() const {
  return beginPosition_;
}

simulation::Car CarManuever::GetPosition(double distance) const {
  if (DoubleIsGreater(distance, distance_)) {
    distance = distance_;
  }

  if (DoubleIsGreaterOrEqual(0.0, distance)) {
    return beginPosition_;
  }

  if (DoubleIsZero(GetSteeringAngle())) {  // moving forward
    simulation::Car result = beginPosition_;
    geometry::Point center = beginPosition_.GetCenter();
    center += beginPosition_.GetDirection().Unit() * distance;
    result.SetCenter(center);
    return result;
  } else {  // doing a turn
    simulation::Car result = beginPosition_;
    geometry::Point center = beginPosition_.GetCenter();
    double radius = rotationCenter_.GetDistance(center);
    double angle = distance / radius;
    result.SetCenter(center.Rotate(rotationCenter_, angle));
    geometry::Vector direction = beginPosition_.GetDirection();
    result.SetDirection(direction.Rotate(angle));
    return result;
  }
}

}  // namespace simulation
