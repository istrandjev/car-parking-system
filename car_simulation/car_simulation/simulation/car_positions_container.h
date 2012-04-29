#ifndef SIMUALTION_CAR_POSITIONS_CONTAINER_H
#define SIMUALTION_CAR_POSITIONS_CONTAINER_H

#include <map>
#include <vector>

namespace geometry {
class BoundingBox;
class Point;
class RectangleObject;
}  // namespace geometry

namespace simulation {

class Car;

class CarPosition {
 public:
  void AddCarPosition(int position_index);
  const std::vector<int>& GetPositions() const;

 private:
  std::vector<int> carPositions_;
};

class CarPositionsContainer {
 public:
  CarPositionsContainer(double minx, double maxx, double miny, double maxy);
  ~CarPositionsContainer();

  void AddCarPosition(
      const Car& position, const geometry::RectangleObject* object);

  std::vector<int> GetPositions(
      const geometry::BoundingBox& bounding_box) const;

  std::vector<int> GetPositions() const;

  const Car* GetPosition(int position_index) const;

  int GetNumberOfPositions() const;

  unsigned GetNumberOfObjects() const;
  const geometry::RectangleObject* GetObject(int index) const;
  unsigned GetObjectIndexForPosition(int position_index);
  const std::vector<int>& GetCarPositionsForObject(int object_index) const;

 private:
  void GetCellCoordinates(const geometry::Point& point, int& i, int& j) const;

 private:
  std::vector<Car*> positions_;
  std::vector<unsigned> positionObjectMap_;
  std::map<const geometry::RectangleObject*, unsigned> objectsMap_;
  std::vector<const geometry::RectangleObject*> objects_;
  std::vector<std::vector<int> > positionsForObjects_;
  std::vector<std::vector<CarPosition> > positionsGrid_;


  double minx_, maxx_;
  double miny_, maxy_;
};

}  // namespace simulation

#endif // SIMUALTION_CAR_POSITIONS_CONTAINER_H
