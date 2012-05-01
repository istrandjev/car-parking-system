#ifndef SIMUALTION_CAR_MOVEMENT_HANDLER_H_
#define SIMUALTION_CAR_MOVEMENT_HANDLER_H_

#include "geometry/segment.h"
#include "simulation/car_description.h"
#include "utils/intersection_handler.h"

#include <vector>

namespace geometry {
class Point;
}  // namespace geometry

namespace simulation {

class Car;
class CarManuever;
class CarPosition;

class CarMovementHandler {
 public:
  CarMovementHandler(const utils::IntersectionHandler* intersection_handler,
                     const CarDescription& car_description);

  const utils::IntersectionHandler* GetIntersectionHandler() const;
  const CarDescription& GetCarDescription() const;

  bool CarMovementPossibleByDistance(const Car& car, double distance) const;
  bool CarMovementPossibleByAngle(const Car& car, double angle) const;


  bool CarMovementPossibleByDistance(const CarPosition& car_position,
                                     double distance) const;
  bool CarMovementPossibleByAngle(const CarPosition& car, double angle,
                                  const geometry::Point& rotation_center) const;
  bool SingleManueverBetweenStates(
      const CarPosition& pos1, const CarPosition& pos2,
      CarManuever &manuever) const;

 private:
  bool ConstructManuever(const CarPosition& car1, const CarPosition& car2,
                         const geometry::Point& rotation_center,
                         CarManuever& manuever) const;
 private:
  CarDescription carDescription_;
  mutable std::vector<geometry::Segment> intersectedCache_;
  const utils::IntersectionHandler* intersectionHandler_;
};

}  // namespace simulation

#endif  // SIMUALTION_CAR_MOVEMENT_HANDLER_H_
