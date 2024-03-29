#ifndef SIMULATION_CAR_MANUEVER_H
#define SIMULATION_CAR_MANUEVER_H

#include "geometry/point.h"
#include "simulation/car_position.h"

#include <iostream>

namespace simulation {

class CarManuever {
 public:
  CarManuever(const simulation::CarPosition& begin_position);
  CarManuever();

  void SetInitialStraightSectionDistance(double distance);
  double GetInitialStraightSectionDistance() const;

  void SetFinalStraightSectionDistance(double distance);
  double GetFinalStraightSectionDistance() const;

  void SetTurnAngle(double angle);
  double GetTurnAngle() const;

  void SetRotationCenter(const geometry::Point& center);
  const geometry::Point& GetRotationCenter() const;

  void SetBeginPosition(const simulation::CarPosition& begin_position);
  simulation::CarPosition GetBeginPosition() const;

  CarPosition GetPosition(double distance) const;

  double GetTurnDistance() const;
  double GetTotalDistance() const;

  friend std::ostream& operator<<(
      std::ostream& out, const CarManuever& manuever);

  void SetReversed(bool reversed);
  bool IsReversed() const;

 private:
  double initialStraightSectionLength_;
  double finalStraightSectionLength_;
  double turnAngle_;
  double turnRadius_;
  geometry::Point rotationCenter_;
  bool reversed_;

  simulation::CarPosition beginPosition_;
};

}  // namespace simulation
#endif // SIMULATION_CAR_MANUEVER_H
