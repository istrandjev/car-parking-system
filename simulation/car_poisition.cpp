#include "simulation/car_position.h"

#include "geometry/point.h"
#include "geometry/vector.h"

namespace simulation {

CarPosition::CarPosition() : center_(0, 0), direction_(1, 0),
    isFinal_(false), isAlongBaseLine_(false){}

CarPosition::CarPosition(const geometry::Point& center,
            const geometry::Vector& direction,
            bool is_final,
            bool is_along_baseline)
  : center_(center), isFinal_(is_final),
    isAlongBaseLine_(is_along_baseline) {
  direction_ = direction.Unit();
}

void CarPosition::SetCenter(const geometry::Point& center) {
  center_ = center;
}

const geometry::Point& CarPosition::GetCenter() const {
  return center_;
}

void CarPosition::SetDirection(const geometry::Vector& direction) {
  direction_ = direction.Unit();
}

const geometry::Vector& CarPosition::GetDirection() const {
  return direction_;
}

void CarPosition::SetIsFinal(bool is_final) {
  isFinal_ = is_final;
}

bool CarPosition::IsFinal() const {
  return isFinal_;
}

void CarPosition::SetIsAlongBaseLine(bool is_along_baseline) {
  isAlongBaseLine_ = is_along_baseline;
}

bool CarPosition::IsAlongBaseLine() const {
  return isAlongBaseLine_;
}

std::ostream& operator <<(std::ostream& out, const CarPosition& car_position) {
  out << "Center: " << car_position.center_
      << " Direction: " << car_position.direction_
      << " Final?: " << car_position.isFinal_
      << " Along baseline?: " << car_position.isAlongBaseLine_ << "\n";
  return out;
}

}  // namespace simulation
