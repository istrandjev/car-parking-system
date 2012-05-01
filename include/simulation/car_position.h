#ifndef SIMULATION_CAR_POSITION_H
#define SIMULATION_CAR_POSITION_H

#include "geometry/point.h"
#include "geometry/vector.h"

namespace simulation {
class CarPosition {
 public:
  CarPosition();
  CarPosition(const geometry::Point& center,
              const geometry::Vector& direction,
              bool is_final,
              bool is_along_baseline);

  void SetCenter(const geometry::Point& center);
  const geometry::Point& GetCenter() const;

  void SetDirection(const geometry::Vector& direction);
  const geometry::Vector& GetDirection() const;

  void SetIsFinal(bool is_final);
  bool IsFinal() const;

  void SetIsAlongBaseLine(bool is_along_baseline);
  bool IsAlongBaseLine() const;

  friend std::ostream& operator<<(
      std::ostream& out, const CarPosition& car_position);

 private:
  bool isFinal_;
  bool isAlongBaseLine_;
  geometry::Point center_;
  geometry::Vector direction_;
};
}  // namespace simulation
#endif // SIMULATION_CAR_POSITION_H
