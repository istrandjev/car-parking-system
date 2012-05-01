#include "simulation/car.h"

#include "geometry/geometry_utils.h"
#include "geometry/line.h"
#include "geometry/point.h"
#include "geometry/polygon.h"
#include "geometry/rectangle_object.h"
#include "utils/double_utils.h"

#include <cmath>
#include <stdexcept>

namespace simulation {

static const double TURN_STEP_RADIANS = 0.01;
static const double WHEEL_WIDTH = 0.195;
static const double WHEEL_LENGTH = 0.8128;

Car::Car(const CarDescription& description)  
      : description_(description),
        currentSteeringAngle_(0) {
}

const CarPosition& Car::GetPosition() const {
  return position_;
}

void Car::SetPosition(const CarPosition &position) {
  position_ = position;
}

const CarDescription& Car::GetDescription() const {
  return description_;
}

void Car::SetDescription(const CarDescription& description) {
  description_ = description;
}

double Car::GetCurrentSteeringAngle() const {
  return currentSteeringAngle_;
}

geometry::Point Car::GetRotationCenter() const {
  int angle_sign = DoubleSign(currentSteeringAngle_);
  
  if (angle_sign == 0) {
    return position_.GetCenter();
  }

  geometry::Point fw_center, rw_center;
  if (angle_sign > 0) {
    fw_center = description_.GetFrontLeftWheelCenter(position_);
    rw_center = description_.GetRearLeftWheelCenter(position_);
  } else {
    fw_center = description_.GetFrontRightWheelCenter(position_);  
    rw_center = description_.GetRearRightWheelCenter(position_);
  }

  const geometry::Vector& direction = position_.GetDirection();
  double fw_angle = currentSteeringAngle_ +
      geometry::GeometryUtils::PI * angle_sign * 0.5;

  geometry::Vector fw_axis_vector = direction.Rotate(fw_angle);
  geometry::Vector rw_axis_vector = direction.GetOrthogonal();

  geometry::Line fw_axis(fw_center, fw_axis_vector);
  geometry::Line rw_axis(rw_center, rw_axis_vector);
  geometry::Point rotation_center;
  if (!fw_axis.Intersect(rw_axis, &rotation_center)) {
    throw std::invalid_argument("current_steering_angle in "
        "Car::Move has incorrect value!");
  } 
  return rotation_center;
}

void Car::Move(double meters_step) {
  int angle_sign = DoubleSign(currentSteeringAngle_);
  if (angle_sign == 0) {
    geometry::Point center = position_.GetCenter();
    position_.SetCenter(
        center + position_.GetDirection().Unit() * meters_step);
    return;
  }

  geometry::Point rotation_center = GetRotationCenter();

  const geometry::Point& center = position_.GetCenter();
  const geometry::Vector& direction = position_.GetDirection();
  double rotation_angle = meters_step / rotation_center.GetDistance(center) ;
  position_.SetCenter(
      center.Rotate(rotation_center, rotation_angle * angle_sign));
  position_.SetDirection(direction.Rotate(rotation_angle * angle_sign));
}

void Car::TurnLeft() {
  if (DoubleIsGreaterOrEqual(description_.GetMaxSteeringAngle(),
      currentSteeringAngle_ + TURN_STEP_RADIANS)) {
    currentSteeringAngle_ += TURN_STEP_RADIANS;
  }
}

void Car::TurnRight() {
  if (DoubleIsGreaterOrEqual(description_.GetMaxSteeringAngle(),
       TURN_STEP_RADIANS - currentSteeringAngle_)) {
    currentSteeringAngle_ -= TURN_STEP_RADIANS;
  }
}

geometry::Polygon Car::GetBounds() const {
  geometry::Vector to_front = position_.GetDirection().Unit() *
      description_.GetLength() * 0.5;
  geometry::Vector to_side = position_.GetDirection().GetOrthogonal().Unit() *
      description_.GetWidth() * 0.5;
  geometry::Polygon result;

  result.AddPointDropDuplicates(position_.GetCenter() + to_front + to_side);
  result.AddPointDropDuplicates(position_.GetCenter() + to_front - to_side);
  result.AddPointDropDuplicates(position_.GetCenter() - to_front - to_side);
  result.AddPointDropDuplicates(position_.GetCenter() - to_front + to_side);
  result.Normalize();
  return result;
}

geometry::Polygon Car::GetWheel(const geometry::Point& center, 
    const geometry::Vector& direction) const {
  double length = WHEEL_LENGTH;
  double width = WHEEL_WIDTH;
  geometry::Vector y_vector = direction.Unit() * length * 0.5;
  geometry::Vector x_vector = direction.GetOrthogonal().Unit() * width * 0.5;
  geometry::Polygon result;

  result.AddPointDropDuplicates(center + x_vector + y_vector); 
  result.AddPointDropDuplicates(center - x_vector + y_vector);
  result.AddPointDropDuplicates(center - x_vector - y_vector); 
  result.AddPointDropDuplicates(center + x_vector - y_vector); 
  return result;
}

geometry::Polygon Car::GetRearLeftWheel() const {
  return GetWheel(description_.GetRearLeftWheelCenter(position_),
      position_.GetDirection());
}

geometry::Polygon Car::GetRearRightWheel() const {
  return GetWheel(description_.GetRearRightWheelCenter(position_),
      position_.GetDirection());
}

geometry::Polygon Car::GetFrontLeftWheel() const {
  int sign = DoubleSign(currentSteeringAngle_);
  geometry::Point wheel_center =
      description_.GetFrontLeftWheelCenter(position_);
  if (sign == 0) {
    return GetWheel(wheel_center, position_.GetDirection());
  }
  
  if (sign > 0) {
      geometry::Vector wheel_direction = 
          position_.GetDirection().Rotate(currentSteeringAngle_);
      return GetWheel(wheel_center, wheel_direction);
  } else {
    double x = description_.GetLength() * description_.GetWheelAxisFraction();
    double tn = tan(-currentSteeringAngle_);
    double angle = atan((x * tn) / (x + description_.GetWidth() * tn));
    geometry::Vector wheel_direction = position_.GetDirection().Rotate(-angle);
    return GetWheel(wheel_center, wheel_direction);
  }
}

geometry::Polygon Car::GetFrontRightWheel() const {
  int sign = DoubleSign(currentSteeringAngle_);
  geometry::Point wheel_center =
      description_.GetFrontRightWheelCenter(position_);
  if (sign == 0) {
    return GetWheel(wheel_center, position_.GetDirection());
  }
  
  if (sign < 0) {
      geometry::Vector wheel_direction =
          position_.GetDirection().Rotate(currentSteeringAngle_);
      return GetWheel(wheel_center, wheel_direction);
  } else {
    double x = description_.GetLength() * description_.GetWheelAxisFraction();
    double tn = tan(currentSteeringAngle_);
    double angle = atan((x * tn) / (x + description_.GetWidth() * tn));
    geometry::Vector wheel_direction = position_.GetDirection().Rotate(angle);
    return GetWheel(wheel_center, wheel_direction);
  }
}

std::vector<geometry::Polygon> Car::GetWheels() const {
  std::vector<geometry::Polygon> result;
  result.push_back(GetFrontLeftWheel());
  result.push_back(GetFrontRightWheel());
  result.push_back(GetRearLeftWheel());
  result.push_back(GetRearRightWheel());
  return result;
}

std::vector<geometry::Polygon> Car::GetRotationGraphicsByDistance(
    double distance_limit) const {
  std::vector<geometry::Polygon> res;
  if (DoubleSign(currentSteeringAngle_) == 0) {

    geometry::Point from = position_.GetCenter() +
        position_.GetDirection().Unit() * (description_.GetLength() * -0.5);
    geometry::Point to =  from + position_.GetDirection().Unit() *
        (distance_limit + description_.GetLength());

    geometry::RectangleObject rectangle_object(from, to);
    res.push_back(rectangle_object.GetBounds());
    return res;
  } else {
    geometry::Point rotation_center = GetRotationCenter();
    double radius = rotation_center.GetDistance(position_.GetCenter());
    double angle = distance_limit / radius;
    return GetRotationGraphicsByAngle(angle);
  }
}

std::vector<geometry::Polygon> Car::GetRotationGraphicsByAngle(
    double rotation_limit) const {
  std::vector<geometry::Polygon> res;

  const double step = 3e-2;
  Car temp(*this);
  int angle_sign = DoubleSign(currentSteeringAngle_);
  if (angle_sign != 0) {  
    for (double c = 0.0; c <= 1.0; c += step) {
      res.push_back(temp.GetBounds());
      geometry::Point rotation_center = temp.GetRotationCenter();\
      CarPosition position;
      position.SetCenter(temp.GetPosition().GetCenter().Rotate(
          rotation_center, step * rotation_limit * angle_sign));
      position.SetDirection(temp.GetPosition().GetDirection().Rotate(
          step * rotation_limit * angle_sign));
      temp.SetPosition(position);
    }
  } else {
    const double default_distance = 20.0;
    return GetRotationGraphicsByDistance(default_distance);
  }
  return res;
}

void  Car::Reset() {
  position_.SetCenter(geometry::Point(0, 0));
  position_.SetDirection(geometry::Vector(0, 1));
  currentSteeringAngle_ = 0;
}

std::ostream& operator <<(std::ostream &out, const Car &car) {
  out << car.description_ << car.position_;
  out << "Current steering angle: " << car.currentSteeringAngle_ << "\n";
  return out;
}

}  // namespace simulation
