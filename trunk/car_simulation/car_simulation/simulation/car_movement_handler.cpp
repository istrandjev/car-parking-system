#include "simulation/car_movement_handler.h"

#include "simulation/car.h"
#include "utils/double_utils.h"
#include "utils/intersection_handler.h"

namespace simulation {

// static
bool CarMovementHandler::CarMovementPossibleByDistance(const Car& car, double distance,
      const utils::IntersectionHandler& intersection_handler) {
  return false;      
}

bool CarMovementHandler::CarMovementPossibleByAngle(const Car& car, double distance,
      const utils::IntersectionHandler& intersection_handler) {
  if (DoubleIsZero(car.GetCurrentSteeringAngle())) {
    return true;
  }  
}

}  // namespace simulation