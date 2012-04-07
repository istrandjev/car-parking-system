#include "simulation/car.h"

#include "geometry/geometry_utils.h"
#include "geometry/line.h"
#include "geometry/point.h"
#include "geometry/polygon.h"
#include "geometry/rectangle_object.h"
#include "utils/double_utils.h"

#include <cmath>

namespace simulation {

static const double TURN_STEP_RADIANS = 0.01;
static const double WHEEL_AXIS_FRACTION = 0.62;
static const double WHEEL_WIDTH = 0.195;
static const double WHEEL_LENGTH = 0.8128;

Car::Car(double width, double length, double max_steering_angle)  
      : width_(width), length_(length), current_steering_angle_(0),
      max_steering_angle_(max_steering_angle) {
  center_ = geometry::Point(0, 0);
  direction_ = geometry::Vector(0, 1);      
}

void Car::SetCenter(const geometry::Point& center) {
  center_ = center;
}

geometry::Point Car::GetCenter() const {
  return center_;
}

void Car::SetDirection(const geometry::Vector& direction) {
  direction_ = direction.Unit();
}

geometry::Vector Car::GetDirection() const {
  return direction_;
}

double Car::GetCurrentSteeringAngle() const {
  return current_steering_angle_;
}

double Car::GetWidth() const {
  return width_;
}
double Car::GetLength() const {
  return length_;
}

geometry::Point Car::GetRotationCenter() const {
  int angle_sign = DoubleSign(current_steering_angle_);
  
  if (angle_sign == 0) {
    return center_;
  }

  geometry::Point fw_center, rw_center;
  if (angle_sign > 0) {
    fw_center = GetFrontLeftWheelCenter();
    rw_center = GetRearLeftWheelCenter();
  } else {
    fw_center = GetFrontRightWheelCenter();  
    rw_center = GetRearRightWheelCenter();
  }

  double fw_angle = current_steering_angle_ + 
      geometry::GeometryUtils::PI * angle_sign * 0.5;

  geometry::Vector fw_axis_vector = direction_.Rotate(fw_angle);
  geometry::Vector rw_axis_vector = direction_.GetOrthogonal();

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
  int angle_sign = DoubleSign(current_steering_angle_);
  if (angle_sign == 0) {
    center_ = center_ + direction_.Unit() * meters_step;
    return;
  }

  geometry::Point rotation_center = GetRotationCenter();

  double rotation_angle = meters_step / rotation_center.GetDistance(center_) ;
  center_ = center_.Rotate(rotation_center, rotation_angle * angle_sign);
  direction_ = direction_.Rotate(rotation_angle * angle_sign);
}

void Car::TurnLeft() {
  if (DoubleIsGreaterOrEqual(max_steering_angle_, 
      current_steering_angle_ + TURN_STEP_RADIANS)) {
    current_steering_angle_ += TURN_STEP_RADIANS;    
  }
}

void Car::TurnRight() {
  if (DoubleIsGreaterOrEqual(max_steering_angle_, 
       TURN_STEP_RADIANS - current_steering_angle_)) {
    current_steering_angle_ -= TURN_STEP_RADIANS;    
  }
}

geometry::Point Car::GetFrontLeftWheelCenter() const {
  return center_ + direction_.Unit() * length_ * 0.5 * WHEEL_AXIS_FRACTION -
      direction_.GetOrthogonal() * width_ * 0.5;
}

geometry::Point Car::GetFrontRightWheelCenter() const {
  return center_ + direction_.Unit() * length_ * 0.5 * WHEEL_AXIS_FRACTION +
      direction_.GetOrthogonal() * width_ * 0.5;
}

geometry::Point Car::GetRearLeftWheelCenter() const {
  return center_ - direction_.Unit() * length_ * 0.5 * WHEEL_AXIS_FRACTION -
      direction_.GetOrthogonal() * width_ * 0.5;
}

geometry::Point Car::GetRearRightWheelCenter() const {
  return center_ - direction_.Unit() * length_ * 0.5 * WHEEL_AXIS_FRACTION +
      direction_.GetOrthogonal() * width_ * 0.5;
}

geometry::Polygon Car::GetBounds() const {
  geometry::Vector to_front = direction_.Unit() * length_ * 0.5;
  geometry::Vector to_side = direction_.GetOrthogonal().Unit() * width_ * 0.5;
  geometry::Polygon result;

  result.AddPointDropDuplicates(center_ + to_front + to_side);
  result.AddPointDropDuplicates(center_ + to_front - to_side);
  result.AddPointDropDuplicates(center_ - to_front - to_side);
  result.AddPointDropDuplicates(center_ - to_front + to_side);
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
  return GetWheel(GetRearLeftWheelCenter(), direction_);
}

geometry::Polygon Car::GetRearRightWheel() const {
  return GetWheel(GetRearRightWheelCenter(), direction_);
}

geometry::Polygon Car::GetFrontLeftWheel() const {
  int sign = DoubleSign(current_steering_angle_);
  geometry::Point wheel_center = GetFrontLeftWheelCenter();
  if (sign == 0) {
    return GetWheel(wheel_center, direction_);
  }
  
  if (sign > 0) {
      geometry::Vector wheel_direction = direction_.Rotate(current_steering_angle_);
      return GetWheel(wheel_center, wheel_direction);
  } else {
    double x = length_ * WHEEL_AXIS_FRACTION;
    double tn = tan(-current_steering_angle_);
    double angle = atan((x * tn) / (x + width_ * tn));
    geometry::Vector wheel_direction = direction_.Rotate(-angle);
    return GetWheel(wheel_center, wheel_direction);
  }
}

geometry::Polygon Car::GetFrontRightWheel() const {
  int sign = DoubleSign(current_steering_angle_);
  geometry::Point wheel_center = GetFrontRightWheelCenter();
  if (sign == 0) {
    return GetWheel(wheel_center, direction_);
  }
  
  if (sign < 0) {
      geometry::Vector wheel_direction = direction_.Rotate(current_steering_angle_);
      return GetWheel(wheel_center, wheel_direction);
  } else {
    double x = length_ * WHEEL_AXIS_FRACTION;
    double tn = tan(current_steering_angle_);
    double angle = atan((x * tn) / (x + width_ * tn));
    geometry::Vector wheel_direction = direction_.Rotate(angle);
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
    double distance_limit) {
  std::vector<geometry::Polygon> res;
  if (DoubleSign(current_steering_angle_) == 0) {

    geometry::Point from = center_ + direction_.Unit() * (length_ * -0.5);
    geometry::Point to =  from + direction_.Unit() * (distance_limit + length_);

    geometry::RectangleObject rectangle_object(from, to);
    res.push_back(rectangle_object.GetBounds());
    return res;
  } else {
    geometry::Point rotation_center = GetRotationCenter();
    double radius = rotation_center.GetDistance(center_);
    double angle = distance_limit / radius;
    return GetRotationGraphicsByAngle(angle);
  }
}

std::vector<geometry::Polygon> Car::GetRotationGraphicsByAngle(
    double rotation_limit) {
  std::vector<geometry::Polygon> res;

  const double step = 3e-2;

  int angle_sign = DoubleSign(current_steering_angle_);
  if (angle_sign != 0) {  
    geometry::Point saved_center = center_;
    geometry::Vector saved_direction = direction_;
    for (double c = 0.0; c <= 1.0; c += step) {
      res.push_back(GetBounds());

      geometry::Point rotation_center = GetRotationCenter();

      center_ = center_.Rotate(rotation_center, step * rotation_limit * angle_sign);
      direction_ = direction_.Rotate(step * rotation_limit * angle_sign);
    }
    center_ = saved_center;
    direction_ = saved_direction;
  } else {
    const double default_distance = 20.0;
    return GetRotationGraphicsByDistance(default_distance);
  }
  return res;
}

void  Car::Reset() {
  center_ = geometry::Point(0, 0);
  direction_ = geometry::Vector(0, 1);
  current_steering_angle_ = 0;
}

}  // namespace simulation