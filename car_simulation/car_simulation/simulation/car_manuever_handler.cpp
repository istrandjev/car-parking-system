#include "simulation/car_manuever_handler.h"

#include "simulation/car.h"
#include "simulation/car_manuever.h"
#include "utils/double_utils.h"

namespace simulation {

CarManueverHandler::CarManueverHandler(
    const std::vector<CarManuever>& manuevers)
    : manuevers_(manuevers), currentManueverIndex_(0), currentDistance_(0) {}

simulation::Car CarManueverHandler::GetCurrentPosition() const {
  return manuevers_[currentManueverIndex_].GetPosition(currentDistance_);
}

void CarManueverHandler::MoveForward(double distance) {
  const simulation::CarManuever& manuever = manuevers_[currentManueverIndex_];
  if (DoubleIsGreaterOrEqual(manuever.GetDistance(),
                             currentDistance_ + distance)) {
    currentDistance_ += distance;
  } else {
    if (currentManueverIndex_ + 1 == manuevers_.size()) {
      currentDistance_ = manuever.GetDistance();
      return;
    } else {
      ++currentManueverIndex_;
    }
    distance -= manuever.GetDistance() - currentDistance_;
    while (currentManueverIndex_ < manuevers_.size()) {
      double current_distance = manuevers_[currentManueverIndex_].GetDistance();
      if (DoubleIsGreaterOrEqual(current_distance, distance)) {
        currentDistance_ = distance;
        break;
      } else {
        distance -= current_distance;
        currentManueverIndex_++;
      }
    }
    if (currentManueverIndex_ == manuevers_.size()) {
      currentManueverIndex_--;
      currentDistance_ = manuevers_[currentManueverIndex_].GetDistance();
    }
  }
}

void CarManueverHandler::MoveBackward(double distance) {
  if (DoubleIsGreaterOrEqual(currentDistance_, distance)) {
    currentDistance_ -= distance;
  } else {
    if (currentManueverIndex_ == 0) {
      currentDistance_ = 0;
      return;
    } else {
      --currentManueverIndex_;
    }
    distance -= currentDistance_;
    while (currentManueverIndex_ >= 0) {
      double current_distance = manuevers_[currentManueverIndex_].GetDistance();
      if (DoubleIsGreaterOrEqual(current_distance, distance)) {
        currentDistance_ = current_distance - distance;
        break;
      } else {
        distance -= current_distance;
        currentManueverIndex_--;
      }
    }
    if (currentManueverIndex_ < 0) {
      currentManueverIndex_ = 0;
      currentDistance_ = 0;
    }
  }
}

void CarManueverHandler::MoveTo(double distance) {
  currentManueverIndex_ = 0;
  currentDistance_ = 0.0;
  MoveForward(distance);
}

}  // namespace simulation
