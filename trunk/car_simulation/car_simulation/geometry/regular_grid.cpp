#include "regular_grid.h"

#include "geometry/bounding_box.h"
#include "geometry/boundary_line.h"
#include "geometry/rectangle_object.h"
#include "utils/double_utils.h"

#include <vector>

namespace geometry {

static const int VERTICAL_CELL_NUM = 20;
static const int HORIZONTAL_CELL_NUM = 30;

void GridElement::AddRectangleObject(const RectangleObject* rectangle_object) {
  rectangleObjects_.push_back(rectangle_object);
}

void GridElement::AddBoundaryLine(const BoundaryLine* boundary_line) {
  boundaryLines_.push_back(boundary_line);
}


const std::vector<const RectangleObject*>& 
    GridElement::GetRectangleObjects() const {
  return rectangleObjects_;    
}

const std::vector<const BoundaryLine*>& GridElement::GetBoudnaryLines() const {
  return boundaryLines_;
}


RegularGrid::RegularGrid(double minx, double maxx, double miny, double maxy)
    : minx_(minx), maxx_(maxx), miny_(miny), maxy_(maxy) {
  grid_.resize(VERTICAL_CELL_NUM, std::vector<GridElement>(
      HORIZONTAL_CELL_NUM));
}

void RegularGrid::AddRectangleObject(const RectangleObject* object) {
  BoundingBox bounding_box = object->GetBoundingBox();

  int mini, maxi;
  int minj, maxj;

  GetCellCoordinates(bounding_box.GetMinX(), bounding_box.GetMinY(),
      &mini, &minj);
  GetCellCoordinates(bounding_box.GetMinX(), bounding_box.GetMinY(),
      &maxi, &maxj);
  for (int i = mini; i <= maxi; ++i) {
    for (int j = minj; j <= maxj; ++j) {
      grid_[i][j].AddRectangleObject(object);
    }
  }
}

void RegularGrid::AddBoundaryLine(const BoundaryLine* border) {
  BoundingBox bounding_box = border->GetBoundingBox();

  int mini, maxi;
  int minj, maxj;

  GetCellCoordinates(bounding_box.GetMinX(), bounding_box.GetMinY(),
      &mini, &minj);
  GetCellCoordinates(bounding_box.GetMinX(), bounding_box.GetMinY(),
      &maxi, &maxj);
  for (int i = mini; i <= maxi; ++i) {
    for (int j = minj; j <= maxj; ++j) {
      grid_[i][j].AddBoundaryLine(border);
    }
  }
}

std::vector<const RectangleObject*> RegularGrid::GetRectangleObjects(
    const BoundingBox& bounding_box) const {
  std::vector<const RectangleObject*> result;

  int mini, maxi;
  int minj, maxj;
  GetCellCoordinates(bounding_box.GetMinX(), bounding_box.GetMinY(),
      &mini, &minj);
  GetCellCoordinates(bounding_box.GetMinX(), bounding_box.GetMinY(),
      &maxi, &maxj);
  for (int i = mini; i < maxi; ++i) {
    for (int j = minj; j < maxj; ++j) {
      const std::vector<const RectangleObject*>& objects =
          grid_[i][j].GetRectangleObjects();
      result.insert(result.end(), objects.begin(), objects.end());
    }
  }

  return result;
}

std::vector<const BoundaryLine*> RegularGrid::GetBoundaryLines(
    const BoundingBox& bounding_box) const {
  std::vector<const BoundaryLine*> result;

  int mini, maxi;
  int minj, maxj;
  GetCellCoordinates(bounding_box.GetMinX(), bounding_box.GetMinY(),
      &mini, &minj);
  GetCellCoordinates(bounding_box.GetMinX(), bounding_box.GetMinY(),
      &maxi, &maxj);
  for (int i = mini; i < maxi; ++i) {
    for (int j = minj; j < maxj; ++j) {
      const std::vector<const BoundaryLine*>& lines =
          grid_[i][j].GetBoudnaryLines();
      result.insert(result.end(), lines.begin(), lines.end());
    }
  }

  return result;
}

void RegularGrid::GetCellCoordinates(double x, double y,
    int* i, int* j) const {
  *i = static_cast<int>((x * VERTICAL_CELL_NUM) / (maxx_ - minx_));
  if (*i == VERTICAL_CELL_NUM) {
    *i--;
  }

  *j = static_cast<int>((y * HORIZONTAL_CELL_NUM) / (maxy_ - miny_));
  if (*j == HORIZONTAL_CELL_NUM) {
    *j--;
  }
}

}  // namespace geometry