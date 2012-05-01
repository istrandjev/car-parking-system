#ifndef SIMULATION_CAR_H_
#define SIMULATION_CAR_H_

#include "geometry/point.h"
#include "geometry/vector.h"

#include "simulation/car_description.h"
#include "simulation/car_position.h"

#include <iostream>
#include <vector>

namespace geometry {
class Polygon;
class Line;
}  // namespace geometry

namespace simulation {

class Car {
 public:
  Car(const simulation::CarDescription& description);

  const CarPosition& GetPosition() const ;
  void SetPosition(const CarPosition& position);

  const CarDescription& GetDescription() const;
  void SetDescription(const CarDescription& description);

  double GetCurrentSteeringAngle() const;


  geometry::Point GetRotationCenter() const;

  bool CanBeRotationCenter(const geometry::Point& center) const;

  void TurnLeft();
  void TurnRight();

  void Move(double meters_step);
  void Reset();
  
  geometry::Polygon GetBounds() const;

  std::vector<geometry::Polygon> GetWheels() const;
  
  std::vector<geometry::Polygon> GetRotationGraphicsByAngle(
      double rotation_limit) const;
  std::vector<geometry::Polygon> GetRotationGraphicsByDistance(
      double distance_limit) const;

 private:
  geometry::Polygon GetFrontLeftWheel() const;
  geometry::Polygon GetFrontRightWheel() const;
  geometry::Polygon GetRearLeftWheel() const;
  geometry::Polygon GetRearRightWheel() const;
  geometry::Polygon GetWheel(const geometry::Point& center, 
      const geometry::Vector& direction) const;

  friend std::ostream& operator<<(std::ostream& out, const Car& car);

 private:
  simulation::CarDescription description_;
  simulation::CarPosition position_;
  double currentSteeringAngle_;
};

}  // namespace simulation

#endif  // SIMULATION_CAR_H_
