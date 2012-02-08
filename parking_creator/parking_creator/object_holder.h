#ifndef OBJECT_HOLDER_H
#define OBJECT_HOLDER_H

#include "scoped_ptr.h"

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

  bool HasSelected();
  void AddRoadSegment(const geometry::Point& from, const geometry::Point& to);
  void AddParkingLot(const geometry::Point& from, const geometry::Point& to);
  void AddObstacle(const geometry::Point& from, const geometry::Point& to);
  bool Select(const geometry::Point& location);

  const RectangleObjectContainer& GetRoadSegments() const;
  const RectangleObjectContainer& GetParkingLots() const;
  const RectangleObjectContainer& GetObstacles() const;

  void SelectNext();
  void SelectPrevious();

  void FinalizeSelected();
  bool IsSelectedFinalized() const;

  geometry::RectangleObject* GetSelected();
 
 private: 
  void AddRectangleObjectToContainer(const geometry::Point& from, 
      const geometry::Point& to, RectangleObjectContainer* container);
  void AddSelectedFromContainer(const geometry::Point& location,
      RectangleObjectContainer* container);

 private:
  RectangleObjectContainer roadSegments_;
  RectangleObjectContainer parkingLots_;
  RectangleObjectContainer obstacles_;
  std::vector<geometry::RectangleObject*> selected_;
  unsigned selectedIndex_;
  bool selectedIsFianlized_;
};

}  // namespace utils 

#endif  // OBJECT_HOLDER_H