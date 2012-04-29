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
  void AddBoundaryLine(const BoundaryLine* boundary_line);
  void AddOriginatingBoundaryLine(const BoundaryLine* boundary_line);
  void RemoveBoundaryLine(const BoundaryLine* boundary_line);

  const std::vector<const RectangleObject*>& GetRectangleObjects() const;
  const std::vector<const BoundaryLine*>& GetAllBoudnaryLines() const;
  const std::vector<const BoundaryLine*>& GetOriginatingBoudnaryLines() const;

 private:
  std::vector<const RectangleObject*> rectangleObjects_;
  std::vector<const BoundaryLine*> allBoundaryLines_;
  std::vector<const BoundaryLine*> originatingBoundaryLines_;
};

class RegularGrid {
 public:
  RegularGrid(double minx, double maxx, double miny, double maxy);
  void AddRectangleObject(const RectangleObject* object);  
  void AddBoundaryLine(const BoundaryLine* border);
  void RemoveBoundaryLine(const BoundaryLine* border);

  std::vector<const RectangleObject*> GetRectangleObjects(
      const BoundingBox& bounding_box) const;
  std::vector<const RectangleObject*> GetRectangleObjects() const;

  void GetBoundaryLines(const BoundingBox& bounding_box,
                        std::vector<const BoundaryLine*>& result) const;
  void GetBoundaryLines(std::vector<const BoundaryLine*>& result) const;
  
 private:
  void GetCellCoordinates(double x, double y, int& i, int& j) const;

 private:
  std::vector<std::vector<GridElement> > grid_;
  double minx_, maxx_;
  double miny_, maxy_;
};

}  // namespace geometry

#endif  // CAR_SIMULATION_CAR_SIMULATION_HANDLERS_REGULAR_GRID_H_
