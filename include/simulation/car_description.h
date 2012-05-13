#ifndef SIMULATION_CAR_DESCRIPTION_H
#define SIMULATION_CAR_DESCRIPTION_H

#include <iostream>

namespace geometry {
class Polygon;
class Point;
class Line;
}  // namespace geometry

namespace simulation {

class CarPosition;

class CarDescription {
 public:
  CarDescription(double width, double length, double max_steering_angle);
  double GetMaxSteeringAngle() const;
  double GetWidth() const;
  double GetLength() const;
  double GetWheelAxisFraction() const;

  void GetBounds(const CarPosition& position, geometry::Polygon& bounds) const;

  geometry::Line GetRearWheelsAxis(const CarPosition& position) const;

  geometry::Point GetFrontLeftWheelCenter(const CarPosition& position) const;
  geometry::Point GetFrontRightWheelCenter(const CarPosition& position) const;
  geometry::Point GetRearLeftWheelCenter(const CarPosition& position) const;
  geometry::Point GetRearRightWheelCenter(const CarPosition& position) const;

  bool CanBeRotationCenter(const CarPosition& car_position,
                           const geometry::Point& center) const;

  friend std::ostream& operator<<(std::ostream& out,
                                  const CarDescription& car_description);
 private:
  void CalculateMinDistFromMainAxis();

 private:
  double width_, length_;
  double maxSteeringAngle_;
  double wheelAxisFraction_;
  double minDistFromMainAxis_;
};
}
#endif // SIMULATION_CAR_DESCRIPTION_H
