#include "regular_grid.h"

#include "geometry/bounding_box.h"
#include "geometry/boundary_line.h"
#include "geometry/rectangle_object.h"
#include "utils/double_utils.h"

#include <set>
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

void GridElement::RemoveBoundaryLine(const BoundaryLine* boundary_line) {
  for (unsigned index = 0; index < boundaryLines_.size(); ++index) {
    if (boundaryLines_[index] == boundary_line) {
      boundaryLines_.erase(boundaryLines_.begin() + index);
      break;
    }
  }
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
      mini, minj);
  GetCellCoordinates(bounding_box.GetMaxX(), bounding_box.GetMaxY(),
      maxi, maxj);
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
      mini, minj);
  GetCellCoordinates(bounding_box.GetMaxX(), bounding_box.GetMaxY(),
      maxi, maxj);
  for (int i = mini; i <= maxi; ++i) {
    for (int j = minj; j <= maxj; ++j) {
      grid_[i][j].AddBoundaryLine(border);
    }
  }
}

void RegularGrid::RemoveBoundaryLine(const BoundaryLine* border) {
  BoundingBox bounding_box = border->GetBoundingBox();

  int mini, maxi;
  int minj, maxj;

  GetCellCoordinates(bounding_box.GetMinX(), bounding_box.GetMinY(),
      mini, minj);
  GetCellCoordinates(bounding_box.GetMinX(), bounding_box.GetMinY(),
      maxi, maxj);
  
  for (int i = mini; i <= maxi; ++i) {
    for (int j = minj; j <= maxj; ++j) {
      grid_[i][j].RemoveBoundaryLine(border);
    }
  }
}

std::vector<const RectangleObject*> RegularGrid::GetRectangleObjects(
    const BoundingBox& bounding_box) const {
  int mini, maxi;
  int minj, maxj;
  GetCellCoordinates(bounding_box.GetMinX(), bounding_box.GetMinY(),
      mini, minj);
  GetCellCoordinates(bounding_box.GetMaxX(), bounding_box.GetMaxY(),
      maxi, maxj);
  std::set<const RectangleObject*> rectangle_objects;
  for (int i = mini; i <= maxi; ++i) {
    for (int j = minj; j <= maxj; ++j) {
      const std::vector<const RectangleObject*>& objects =
          grid_[i][j].GetRectangleObjects();
      rectangle_objects.insert(objects.begin(), objects.end());
    }
  }

  return std::vector<const RectangleObject*>(
      rectangle_objects.begin(), rectangle_objects.end());
}

std::vector<const RectangleObject*> RegularGrid::GetRectangleObjects() const {
  geometry::BoundingBox bounding_box(minx_, maxx_, miny_, maxy_);
  return GetRectangleObjects(bounding_box);
}


std::vector<const BoundaryLine*> RegularGrid::GetBoundaryLines(
    const BoundingBox& bounding_box) const {
  int mini, maxi;
  int minj, maxj;
  GetCellCoordinates(bounding_box.GetMinX(), bounding_box.GetMinY(),
      mini, minj);
  GetCellCoordinates(bounding_box.GetMaxX(), bounding_box.GetMaxY(),
      maxi, maxj);

  std::set<const BoundaryLine*> boundary_lines;
  
  for (int i = mini; i <= maxi; ++i) {
    for (int j = minj; j <= maxj; ++j) {
      std::vector<const BoundaryLine*> lines =
          grid_[i][j].GetBoudnaryLines();
      boundary_lines.insert(lines.begin(), lines.end());
    }
  }

  return std::vector<const BoundaryLine*>(
      boundary_lines.begin(), boundary_lines.end());
}

std::vector<const BoundaryLine*> RegularGrid::GetBoundaryLines() const {
  geometry::BoundingBox bounding_box(minx_, maxx_, miny_, maxy_);
  return GetBoundaryLines(bounding_box);
}

void RegularGrid::GetCellCoordinates(double x, double y,
    int& i, int& j) const {
  i = static_cast<int>(((x - minx_) * VERTICAL_CELL_NUM) / (maxx_ - minx_));
  if (i >= VERTICAL_CELL_NUM) {
    i = VERTICAL_CELL_NUM - 1;
  }
  if (i < 0) {
    i = 0;
  }
  j = static_cast<int>(((y - miny_) * HORIZONTAL_CELL_NUM) / (maxy_ - miny_));
  if (j >= HORIZONTAL_CELL_NUM) {
    j = HORIZONTAL_CELL_NUM - 1;
  }
  if (j < 0) {
    j = 0;
  }
}

}  // namespace geometry
