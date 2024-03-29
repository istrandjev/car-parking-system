#include "car_positions_container.h"

#include "geometry/bounding_box.h"
#include "simulation/car.h"

namespace simulation {

static const int VERTICAL_CELL_NUM = 200;
static const int HORIZONTAL_CELL_NUM = 300;

void CarPositionEntry::AddCarPosition(int position_index) {
  carPositions_.push_back(position_index);
}

const std::vector<int>& CarPositionEntry::GetPositions() const {
  return carPositions_;
}

CarPositionsContainer::CarPositionsContainer(
    double minx, double maxx, double miny, double maxy)
  : minx_(minx), maxx_(maxx), miny_(miny), maxy_(maxy) {
  positionsGrid_.resize(VERTICAL_CELL_NUM);
  for (int i = 0; i < VERTICAL_CELL_NUM; ++i) {
    positionsGrid_[i].resize(HORIZONTAL_CELL_NUM);
  }   
}

CarPositionsContainer::~CarPositionsContainer() {
  for (unsigned i = 0; i < positions_.size(); ++i) {
    delete positions_[i];
  }
}

void CarPositionsContainer::AddCarPosition(
    const CarPosition& position, const geometry::RectangleObject* object) {
  int i, j;
  GetCellCoordinates(position.GetCenter(), i, j);
  positionsGrid_[i][j].AddCarPosition(positions_.size());

  unsigned object_index = 0;
  std::map<const geometry::RectangleObject*, unsigned>::iterator it =
      objectsMap_.find(object);
  if (it == objectsMap_.end()) {
    object_index = objects_.size();
    objectsMap_.insert(std::make_pair(object, object_index));
    objects_.push_back(object);
    positionsForObjects_.push_back(std::vector<int>());
  } else {
    object_index = it->second;
  }
 
  positionsForObjects_[object_index].push_back(
      static_cast<int>(positions_.size()));
  positionObjectMap_.push_back(object_index);
  positions_.push_back(new CarPosition(position));
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
        const CarPosition* car_position = positions_[positions[k]];
        if (bounding_box.Contains(car_position->GetCenter())) {
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

const CarPosition* CarPositionsContainer::GetPosition(
    int position_index) const {
  return positions_[position_index];
}

int CarPositionsContainer::GetNumberOfPositions() const {
  return static_cast<int>(positions_.size());
}

unsigned CarPositionsContainer::GetNumberOfObjects() const {
  return objects_.size();
}

const geometry::RectangleObject *CarPositionsContainer::GetObject(
    int index) const {
  return objects_[index];
}

unsigned CarPositionsContainer::GetObjectIndexForPosition(int position_index) {
  return positionObjectMap_[position_index];
}

const std::vector<int>& CarPositionsContainer::GetCarPositionsForObject(
    int object_index) const {
  return positionsForObjects_[object_index];
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
