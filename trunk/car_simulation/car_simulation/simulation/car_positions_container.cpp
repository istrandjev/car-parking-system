#include "car_positions_container.h"

#include "geometry/bounding_box.h"
#include "simulation/car.h"

namespace simulation {

static const int VERTICAL_CELL_NUM = 200;
static const int HORIZONTAL_CELL_NUM = 300;

void CarPosition::AddCarPosition(int position_index) {
  carPositions_.push_back(position_index);
}

const std::vector<int> &CarPosition::GetPositions() const {
  return carPositions_;
}

CarPositionsContainer::CarPositionsContainer(
    double minx, double maxx, double miny, double maxy)
  : minx_(minx), maxx_(maxx), miny_(miny), maxy_(maxy) {}

CarPositionsContainer::~CarPositionsContainer() {
  for (unsigned i = 0; i < positions_.size(); ++i) {
    delete positions_[i];
  }
}

void CarPositionsContainer::AddCarPosition(const Car &position) {
  int i, j;
  GetCellCoordinates(position.GetCenter(), i, j);
  positionsGrid_[i][j].AddCarPosition(positions_.size());
  positions_.push_back(new Car(position));
}

std::vector<int> CarPositionsContainer::GetPositions(
    const geometry::BoundingBox &bounding_box) const {
  int mini, maxi, minj, maxj;
  geometry::Point lower_left(bounding_box.GetMinX(), bounding_box.GetMinY());
  geometry::Point upper_right(bounding_box.GetMaxX(), bounding_box.GetMaxY());
  GetCellCoordinates(lower_left, mini, minj);
  GetCellCoordinates(upper_right, maxi, maxj);
  std::vector<int> result;
  for (int i = mini; i <= maxi; ++i) {
    for (int j = minj; j <= maxj; ++j) {
      const std::vector<int>& positions =
          positionsGrid_[i][j].GetPositions();
      for (unsigned k = 0; k < positions.size(); ++k) {
        const Car* car = positions_[positions[k]];
        if (bounding_box.Contains(car->GetCenter())) {
          result.push_back(positions[k]);
        }
      }
    }
  }

  return result;
}

std::vector<int> CarPositionsContainer::GetPositions() const {
  std::vector<int> res;
  for (unsigned i = 0; i < positions_.size(); ++i) {
    res.push_back(i);
  }
  return res;
}

const Car* CarPositionsContainer::GetPosition(int position_index) const {
  return positions_[position_index];
}

int CarPositionsContainer::GetNumberOfPositions() const {
  return static_cast<int>(positions_.size());
}

void CarPositionsContainer::GetCellCoordinates(
    const geometry::Point &point, int &i, int &j) const {
  i = static_cast<int>(((point.x - minx_) * VERTICAL_CELL_NUM) / (maxx_ - minx_));
  if (i >= VERTICAL_CELL_NUM) {
    i = VERTICAL_CELL_NUM - 1;
  }
  if (i < 0) {
    i = 0;
  }
  j = static_cast<int>(((point.y - miny_) * HORIZONTAL_CELL_NUM) / (maxy_ - miny_));
  if (j >= HORIZONTAL_CELL_NUM) {
    j = HORIZONTAL_CELL_NUM - 1;
  }
  if (j < 0) {
    j = 0;
  }
}

}  // namespace simulation
