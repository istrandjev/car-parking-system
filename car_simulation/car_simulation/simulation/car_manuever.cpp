#include "simulation/car_manuever.h"

#include "simulation/car.h"
#include "utils/double_utils.h"

#include <cmath>

namespace simulation {

CarManuever::CarManuever(
    const CarPosition& begin_position)
        : beginPosition_(begin_position),
          turnAngle_(0.0),
          turnRadius_(0.0),
          initialStraightSectionLength_(0.0),
          finalStraightSectionLength_(0.0),
          reversed_(false) {}

CarManuever::CarManuever()
  : turnAngle_(0.0),
    turnRadius_(0.0),
    initialStraightSectionLength_(0.0),
    finalStraightSectionLength_(0.0),
    reversed_(false) {}

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

void CarManuever::SetBeginPosition(
    const simulation::CarPosition& begin_position) {
  beginPosition_ = begin_position;
}

simulation::CarPosition CarManuever::GetBeginPosition() const {
  return beginPosition_;
}

simulation::CarPosition CarManuever::GetPosition(double distance) const {
  double turn_distance = GetTurnDistance();
  double total_distance = initialStraightSectionLength_ + turn_distance +
      finalStraightSectionLength_;

  if (reversed_) {
    distance = total_distance - distance;
  }

  if (DoubleIsGreater(distance, total_distance)) {
    distance = total_distance;
  }

  if (DoubleIsGreaterOrEqual(0.0, distance)) {
    return beginPosition_;
  }

  simulation::CarPosition result = beginPosition_;

  geometry::Point center = result.GetCenter();
  geometry::Vector direction = result.GetDirection().Unit();

  if (DoubleIsGreaterOrEqual(initialStraightSectionLength_, distance)) {
    result.SetCenter(center + direction * distance);
    return result;
  }

  distance -= initialStraightSectionLength_;

  if (DoubleIsGreaterOrEqual(turn_distance, distance)) {
    double angle = distance / turnRadius_;
    if (DoubleIsGreater(0.0, turnAngle_)) {
      angle *= -1.0;
    }
    result.SetCenter(result.GetCenter().Rotate(rotationCenter_, angle));
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

double CarManuever::GetTurnDistance() const {
  return fabs(turnAngle_ * turnRadius_);
}

double CarManuever::GetTotalDistance() const {
  return initialStraightSectionLength_ + GetTurnDistance() +
      finalStraightSectionLength_;
}

void CarManuever::SetReversed(bool reversed) {
  reversed_ = reversed;
}

bool CarManuever::IsReversed() const {
  return reversed_;
}

std::ostream& operator<<(std::ostream& out, const CarManuever& manuever) {
  out << "Initial position: " << manuever.beginPosition_ << "\n";
  out << "Initial straight segment length: "
      << manuever.initialStraightSectionLength_ << "\n";

  out << "Turn angle: " << manuever.turnAngle_
      << "Turn radius:" << manuever.turnRadius_ << "\n";

  out << "Rotation center: " << manuever.rotationCenter_ << "\n";
  out << "Final straight segment length: "
      << manuever.finalStraightSectionLength_;
  out << "Manuever is reversed: " << manuever.reversed_;
  return out;
}

}  // namespace simulation
