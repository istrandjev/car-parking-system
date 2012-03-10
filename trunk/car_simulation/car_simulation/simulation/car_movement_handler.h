#ifndef SIMUALTION_CAR_MOVEMENT_HANDLER_H_
#define SIMUALTION_CAR_MOVEMENT_HANDLER_H_

namespace utils {
class IntersectionHandler;
}  // namespace utils

namespace simulation {

class Car;

class CarMovementHandler {
  static bool CarMovementPossible(const Car& car, double angle,
      const utils::IntersectionHandler& intersection_handler);
};

}  // namespace simulation

#endif  // SIMUALTION_CAR_MOVEMENT_HANDLER_H_
