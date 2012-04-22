#ifndef SIMULATION_CAR_MANUEVER_H
#define SIMULATION_CAR_MANUEVER_H

#include "geometry/point.h"
#include "simulation/car.h"

#include <iostream>

namespace simulation {

class CarManuever {
 public:
  CarManuever(const simulation::Car& begin_position);
  CarManuever();

  void SetInitialStraightSectionDistance(double distance);
  double GetInitialStraightSectionDistance() const;

  void SetFinalStraightSectionDistance(double distance);
  double GetFinalStraightSectionDistance() const;

  void SetTurnAngle(double angle);
  double GetTurnAngle() const;

  void SetRotationCenter(const geometry::Point& center);
  const geometry::Point& GetRotationCenter() const;

  void SetBeginPosition(const simulation::Car& begin_position);
  simulation::Car GetBeginPosition() const;

  simulation::Car GetPosition(double distance) const;

  double GetTotalDistance() const;

  friend std::ostream& operator<<(
      std::ostream& out, const CarManuever& manuever);

 private:
  double initialStraightSectionLength_;
  double finalStraightSectionLength_;
  double turnAngle_;
  double turnRadius_;
  geometry::Point rotationCenter_;
  
  simulation::Car beginPosition_;
};

}  // namespace simulation
#endif // SIMULATION_CAR_MANUEVER_H
