#include "simulation/car_manuever.h"

namespace simulation {

CarManuever::CarManuever(const geometry::Point& rotation_center,
    double steering_angle, double distance)
        : rotationCenter_(rotation_center),
        steeringAngle_(steering_angle),
        distance_(distance) {}

CarManuever::CarManuever(double distance)
  : steeringAngle_(0.0), distance_(distance) {}

CarManuever::CarManuever() : steeringAngle_(0.0), distance_(0.0) {}

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

}  // namespace simulation
