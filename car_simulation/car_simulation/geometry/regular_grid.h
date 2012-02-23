#ifndef CAR_SIMULATION_CAR_SIMULATION_HANDLERS_REGULAR_GRID_H_
#define CAR_SIMULATION_CAR_SIMULATION_HANDLERS_REGULAR_GRID_H_

#include <vector>

namespace geometry {

class BoundingBox;
class BoundaryLine;
class RectangleObject;

class GridElement {
 public:
  void AddRectangleObject(const RectangleObject* rectangle_object);
  void AddBoundaryLine(const BoundaryLine* boudnary_line);

  const std::vector<const RectangleObject*>& GetRectangleObjects() const;
  const std::vector<const BoundaryLine*>& GetBoudnaryLines() const;

 private:
  std::vector<const RectangleObject*> rectangleObjects_;
  std::vector<const BoundaryLine*> boundaryLines_;
};

class RegularGrid {
 public:
  RegularGrid(double minx, double maxx, double miny, double maxy);
  void AddRectangleObject(const RectangleObject* object);  
  void AddBoundaryLine(const BoundaryLine* border);

  std::vector<const RectangleObject*> GetRectangleObjects(
      const BoundingBox& bounding_box) const;

  std::vector<const BoundaryLine*> GetBoundaryLines(
      const BoundingBox& bounding_box) const;
 
 private:
  void GetCellCoordinates(double x, double y, int* i, int* j) const;

 private:
  std::vector<std::vector<GridElement> > grid_;
  double minx_, maxx_;
  double miny_, maxy_;
};

}  // namespace geometry

#endif  // CAR_SIMULATION_CAR_SIMULATION_HANDLERS_REGULAR_GRID_H_
