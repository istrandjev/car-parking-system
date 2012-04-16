#ifndef SIMULATION_CAR_H_
#define SIMULATION_CAR_H_

#include "geometry/point.h"
#include "geometry/vector.h"

#include <vector>

namespace geometry {
class Polygon;
class Line;
}  // namespace geometry

namespace simulation {

class Car {
 public:
  Car(double width, double length, double max_steering_angle);

  void SetCenter(const geometry::Point& center);
  const geometry::Point& GetCenter() const;
  void SetDirection(const geometry::Vector& direction);
  const geometry::Vector& GetDirection() const;

  double GetCurrentSteeringAngle() const;
  double GetWidth() const;
  double GetLength() const;

  geometry::Point GetRotationCenter() const;

  bool CanBeRotationCenter(const geometry::Point& center,
                           double& steering_angle) const;

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
  geometry::Point GetFrontLeftWheelCenter() const;
  geometry::Point GetFrontRightWheelCenter() const;
  geometry::Point GetRearLeftWheelCenter() const;
  geometry::Point GetRearRightWheelCenter() const;

  geometry::Line GetRearWheelsAxis() const;

  geometry::Polygon GetFrontLeftWheel() const;
  geometry::Polygon GetFrontRightWheel() const;
  geometry::Polygon GetRearLeftWheel() const;
  geometry::Polygon GetRearRightWheel() const;
  geometry::Polygon GetWheel(const geometry::Point& center, 
      const geometry::Vector& direction) const;
 
 private:
  geometry::Point center_;
  geometry::Vector direction_;
  double width_, length_;   
  double current_steering_angle_; 
  double max_steering_angle_;
};

}  // namespace simulation

#endif  // SIMULATION_CAR_H_
