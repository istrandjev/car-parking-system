#ifndef SIMULATION_CAR_MANUEVER_H
#define SIMULATION_CAR_MANUEVER_H

#include "geometry/point.h"

namespace simulation {

class CarManuever {
 public:
  CarManuever(const geometry::Point& rotation_center,
              double steering_angle,
              double distance);
  CarManuever(double distance);
  CarManuever();

  void SetSteeringAngle(double steering_angle);
  double GetSteeringAngle() const;

  void SetDistance(double distance);
  double GetDistance() const;

 void SetRotationCenter(const geometry::Point& center);
 const geometry::Point& GetRotationCenter() const;

private:
  double steeringAngle_;
  double distance_;
  geometry::Point rotationCenter_;
};
}  // namespace simulation
#endif // SIMULATION_CAR_MANUEVER_H
