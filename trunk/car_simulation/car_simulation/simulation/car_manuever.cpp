#include "simulation/car_manuever.h"

#include "simulation/car.h"
#include "utils/double_utils.h"

namespace simulation {

CarManuever::CarManuever(
    const simulation::Car& begin_position)
        : beginPosition_(begin_position),
          turnAngle_(0.0),
          turnRadius_(0.0),
          initialStraightSectionLength_(0.0),
          finalStraightSectionLength_(0.0) {}

CarManuever::CarManuever()
  : beginPosition_(0.0, 0.0, 0.0),
    turnAngle_(0.0),
    turnRadius_(0.0),
    initialStraightSectionLength_(0.0),
    finalStraightSectionLength_(0.0) {}

void CarManuever::SetInitialStraightSectionDistance(double distance) {
  initialStraightSectionLength_ = distance;
}

double CarManuever::GetInitialStraightSectionDistance() const {
  return initialStraightSectionLength_;
}

void CarManuever::SetFinalStraightSectionDistance(double distance) {
  finalStraightSectionLength_ = distance;
}

double CarManuever::GetFinalStraightSectionDistance() const {
  return finalStraightSectionLength_;
}

void CarManuever::SetTurnAngle(double angle) {
  turnAngle_ = angle;
}

double CarManuever::GetTurnAngle() const {
  return turnAngle_;
}

void CarManuever::SetRotationCenter(const geometry::Point& center) {
  turnRadius_ = center.GetDistance(beginPosition_.GetCenter());
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
  double turn_distance = turnRadius_ * turnAngle_;
  double total_distance = initialStraightSectionLength_ + turn_distance +
      finalStraightSectionLength_;
  if (DoubleIsGreater(distance, total_distance)) {
    distance = total_distance;
  }

  if (DoubleIsGreaterOrEqual(0.0, distance)) {
    return beginPosition_;
  }

  simulation::Car result = beginPosition_;

  geometry::Point center = result.GetCenter();
  geometry::Vector direction = result.GetDirection().Unit();

  if (DoubleIsGreaterOrEqual(initialStraightSectionLength_, distance)) {
    result.SetCenter(center + direction * distance);
    return result;
  }

  distance -= initialStraightSectionLength_;

  if (DoubleIsGreaterOrEqual(turn_distance, distance)) {
    double angle = distance / turnRadius_;
    result.SetCenter(center.Rotate(rotationCenter_, angle));
    result.SetDirection(direction.Rotate(angle));
    return result;
  }

  direction = direction.Rotate(turnAngle_);
  center = center.Rotate(rotationCenter_, turnAngle_);
  distance -= turn_distance;

  result.SetCenter(center + direction * distance);
  result.SetDirection(direction);
  return result;
}

double CarManuever::GetTotalDistance() const {
  return initialStraightSectionLength_ + turnAngle_ * turnRadius_ +
      finalStraightSectionLength_;
}

}  // namespace simulation
