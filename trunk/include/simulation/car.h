#ifndef CAR_H
#define CAR_H

#include "geometry/point.h"
#include "geometry/vector.h"

#include <vector>

namespace geometry {
class Polygon;
}  // namespace geometry

namespace simulation {

class Car {
 public:
  Car(double width, double length, double max_steering_angle);

  void SetCenter(const geometry::Point& center);
  void SetDirection(const geometry::Vector& direction);

  void TurnLeft();
  void TurnRight();

  void Move(double meters_step);
  void Reset();
  
  geometry::Polygon GetBounds() const;

  std::vector<geometry::Polygon> GetWheels() const;
  
  std::vector<geometry::Polygon> GetRotationGraphics();
 
 private:
  geometry::Point GetFrontLeftWheelCenter() const;
  geometry::Point GetFrontRightWheelCenter() const;
  geometry::Point GetRearLeftWheelCenter() const;
  geometry::Point GetRearRightWheelCenter() const;

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

#endif  // CAR_H