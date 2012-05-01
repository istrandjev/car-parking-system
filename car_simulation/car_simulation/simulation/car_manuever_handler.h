#ifndef SIMULATION_CAR_MANUEVER_HANDLER_H
#define SIMULATION_CAR_MANUEVER_HANDLER_H

#include "simulation/car_description.h"

#include <vector>

namespace simulation {

class CarManuever;
class CarDescription;
class Car;

class CarManueverHandler {
 public:
  CarManueverHandler(const CarDescription& car, 
      const std::vector<CarManuever>& manuevers);

  simulation::Car GetCurrentPosition() const;
  void MoveForward(double distance);
  void MoveBackward(double distance);

  void MoveTo(double distance);

 private:
  CarDescription carDescription_;
  std::vector<CarManuever> manuevers_;
  unsigned currentManueverIndex_;
  double currentDistance_;
};
}  // namespace simulation

#endif // SIMULATION_CAR_MANUEVER_HANDLER_H
