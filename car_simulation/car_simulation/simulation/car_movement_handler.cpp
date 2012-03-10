#include "simulation/car_movement_handler.h"

#include "simulation/car.h"
#include "utils/intersection_handler.h"

namespace simulation {

// static
bool CarMovementHandler::CarMovementPossible(const Car& car, double angle,
      const utils::IntersectionHandler& intersection_handler) {
  return false;      
}

}  // namespace simulation