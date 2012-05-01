#include "simulation/car_manuever_handler.h"

#include "simulation/car.h"
#include "simulation/car_manuever.h"
#include "utils/double_utils.h"

namespace simulation {

CarManueverHandler::CarManueverHandler(const CarDescription& car_description,
    const std::vector<CarManuever>& manuevers)
    : carDescription_(car_description), manuevers_(manuevers), 
      currentManueverIndex_(0), currentDistance_(0) {}

simulation::Car CarManueverHandler::GetCurrentPosition() const {
  simulation::Car result(carDescription_);
  result.SetPosition(manuevers_[currentManueverIndex_]
      .GetPosition(currentDistance_));
  return result;
}

void CarManueverHandler::MoveForward(double distance) {
  if (currentManueverIndex_ >= manuevers_.size()) {
    return;
  }

  const simulation::CarManuever& manuever = manuevers_[currentManueverIndex_];
  double total_distance = manuever.GetTotalDistance();

  if (DoubleIsGreaterOrEqual(total_distance, currentDistance_ + distance)) {
    currentDistance_ += distance;
  } else {
    if (currentManueverIndex_ + 1 == manuevers_.size()) {
      currentDistance_ = total_distance;
      return;
    } else {
      ++currentManueverIndex_;
    }
    distance -= total_distance - currentDistance_;
    while (currentManueverIndex_ < manuevers_.size()) {
      double current_distance = 
          manuevers_[currentManueverIndex_].GetTotalDistance();
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
      currentDistance_ = manuevers_[currentManueverIndex_].GetTotalDistance();
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
      double current_distance = 
          manuevers_[currentManueverIndex_].GetTotalDistance();
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
