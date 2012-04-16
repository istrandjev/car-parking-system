#ifndef SIMULATION_CAR_MANUEVER_H
#define SIMULATION_CAR_MANUEVER_H

#include "geometry/point.h"
#include "simulation/car.h"

namespace simulation {

class CarManuever {
 public:
  CarManuever(const simulation::Car& begin_position,
              const geometry::Point& rotation_center,
              double steering_angle,
              double distance);
  CarManuever(const simulation::Car& begin_position, double distance);
  CarManuever();

  void SetSteeringAngle(double steering_angle);
  double GetSteeringAngle() const;

  void SetDistance(double distance);
  double GetDistance() const;

 void SetRotationCenter(const geometry::Point& center);
 const geometry::Point& GetRotationCenter() const;

 void SetBeginPosition(const simulation::Car& begin_position);
 simulation::Car GetBeginPosition() const;

private:
  double steeringAngle_;
  double distance_;
  simulation::Car beginPosition_;
  geometry::Point rotationCenter_;
};
}  // namespace simulation
#endif // SIMULATION_CAR_MANUEVER_H
