#ifndef SIMUALTION_CAR_MOVEMENT_HANDLER_H_
#define SIMUALTION_CAR_MOVEMENT_HANDLER_H_

namespace utils {
class IntersectionHandler;
}  // namespace utils

namespace simulation {

class Car;

class CarMovementHandler {
  static bool CarMovementPossibleByDistance(const Car& car, double distance,
      const utils::IntersectionHandler& intersection_handler);
  static bool CarMovementPossibleByAngle(const Car& car, double angle,
      const utils::IntersectionHandler& intersection_handler);
};

}  // namespace simulation

#endif  // SIMUALTION_CAR_MOVEMENT_HANDLER_H_
