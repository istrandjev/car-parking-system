#ifndef SIMUALTION_CAR_MOVEMENT_HANDLER_H_
#define SIMUALTION_CAR_MOVEMENT_HANDLER_H_

#include "utils/intersection_handler.h"

namespace simulation {

class Car;

class CarMovementHandler {
 public:
  CarMovementHandler(const utils::IntersectionHandler* intersection_handler);

  const utils::IntersectionHandler* GetIntersectionHandler() const;
  
  bool CarMovementPossibleByDistance(const Car& car, double distance);

  bool CarMovementPossibleByAngle(const Car& car, double angle);

  bool SingleManueverBetweenStates(const Car& pos1, const Car& pos2,
      double& steering_angle, double& distance);
 
 private:
  const utils::IntersectionHandler* intersectionHandler_;
};

}  // namespace simulation

#endif  // SIMUALTION_CAR_MOVEMENT_HANDLER_H_
