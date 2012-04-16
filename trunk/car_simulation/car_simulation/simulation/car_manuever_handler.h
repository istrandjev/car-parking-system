#ifndef SIMULATION_CAR_MANUEVER_HANDLER_H
#define SIMULATION_CAR_MANUEVER_HANDLER_H

#include <vector>

namespace simulation {

class CarManuever;
class Car;

class CarManueverHandler {
 public:
  CarManueverHandler(const std::vector<CarManuever>& manuevers);

  simulation::Car GetCurrentPosition() const;
  void MoveForward(double distance);
  void MoveBackward(double distance);

  void MoveTo(double distance);

 private:
  std::vector<CarManuever> manuevers_;
  unsigned currentManueverIndex_;
  double currentDistance_;
};
}  // namespace simulation

#endif // SIMULATION_CAR_MANUEVER_HANDLER_H
