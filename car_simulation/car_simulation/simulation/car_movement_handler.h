#ifndef SIMUALTION_CAR_MOVEMENT_HANDLER_H_
#define SIMUALTION_CAR_MOVEMENT_HANDLER_H_

#include "geometry/segment.h"
#include "utils/intersection_handler.h"

#include <vector>

namespace geometry {
class Point;
}  // namespace geometry

namespace simulation {

class Car;
class CarManuever;

class CarMovementHandler {
 public:
  CarMovementHandler(const utils::IntersectionHandler* intersection_handler);

  const utils::IntersectionHandler* GetIntersectionHandler() const;
  
  bool CarMovementPossibleByDistance(const Car& car, double distance) const;

  bool CarMovementPossibleByAngle(const Car& car, double angle) const;
  bool CarMovementPossibleByAngle(const Car& car, double angle,
                                  const geometry::Point& rotation_center) const;
  bool SingleManueverBetweenStates(const Car& pos1, const Car& pos2,
      CarManuever& manuever) const;
 
 private:
  bool ConstructManuever(const Car& car1, const Car& car2,
                         const geometry::Point& rotation_center,
                         CarManuever& manuever) const;
 private:
  mutable std::vector<geometry::Segment> intersectedCache_;
  const utils::IntersectionHandler* intersectionHandler_;
};

}  // namespace simulation

#endif  // SIMUALTION_CAR_MOVEMENT_HANDLER_H_
