#ifndef INCLUDE_UTILS_OBJECT_HOLDER_H_
#define INCLUDE_UTILS_OBJECT_HOLDER_H_

#include "utils/scoped_ptr.h"

#include <string>
#include <vector>

namespace geometry {
class RectangleObject;
class Point;
}  // namespace geometry

namespace utils {

typedef std::vector<geometry::RectangleObject*> RectangleObjectContainer;

class ObjectHolder {
 public:
  ObjectHolder();
  ~ObjectHolder();
  void DeleteObjects();

  geometry::RectangleObject* AddTwoWayRoadSegment(
      const geometry::Point& from, const geometry::Point& to);

  geometry::RectangleObject* AddOneWayRoadSegment(
      const geometry::Point& from, const geometry::Point& to);

  geometry::RectangleObject* AddParkingLot(
      const geometry::Point& from, const geometry::Point& to);

  geometry::RectangleObject* AddObstacle(
      const geometry::Point& from, const geometry::Point& to);

  bool FindAndDelete(geometry::RectangleObject* object);

  void GetObectsForLocation(const geometry::Point& location,
      RectangleObjectContainer* container);

  const RectangleObjectContainer& GetRoadSegments() const;
  const RectangleObjectContainer& GetParkingLots() const;
  const RectangleObjectContainer& GetObstacles() const;

  void DumpToFile(const std::string& file_path) const;
  void ParseFromFile(const std::string& file_path);

 private:

  static void GetObectsForLocationFromContainer(
      const geometry::Point& location,
      const RectangleObjectContainer* container,
      RectangleObjectContainer* location_objects);

  static bool FindAndDeleteFromContainer(geometry::RectangleObject* object,
      RectangleObjectContainer* container);

 private:
  RectangleObjectContainer roadSegments_;
  RectangleObjectContainer parkingLots_;
  RectangleObjectContainer obstacles_;
};

}  // namespace utils

#endif  // INCLUDE_UTILS_OBJECT_HOLDER_H_
