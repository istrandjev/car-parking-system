#include "object_holder.h"

#include "directed_rectangle_object.h"
#include "point.h"
#include "rectangle_object.h"

#include <vector>

namespace utils {

static const double DEFAULT_ROAD_WIDTH = 2.5;
static const double DEFAULT_PARKING_WIDTH = 2.5;
static const double DEFAULT_OBSTACLE_WIDTH = 2.0;

ObjectHolder::ObjectHolder() 
    : selectedIsFianlized_(false), currentType(ROAD_SEGMENT) {}

ObjectHolder::~ObjectHolder() {
  for (unsigned index = 0; index < roadSegments_.size(); ++index) {
    delete roadSegments_[index];
  }
  for (unsigned index = 0; index < parkingLots_.size(); ++index) {
    delete parkingLots_[index];
  }
  for (unsigned index = 0; index < obstacles_.size(); ++index) {
    delete obstacles_[index];
  }
}

bool ObjectHolder::HasSelected() {
  return !selected_.empty();
}

void ObjectHolder::AddRoadSegment(const geometry::Point& from, const geometry::Point& to) {
  geometry::DirectedRectangleObject* object = 
      new geometry::DirectedRectangleObject(from, to, DEFAULT_ROAD_WIDTH);
  AddRectangleObjectToContainer(object, &roadSegments_);
}

void ObjectHolder::AddParkingLot(const geometry::Point& from, const geometry::Point& to) {
  geometry::RectangleObject* object = 
      new geometry::RectangleObject(from, to, DEFAULT_PARKING_WIDTH);
  AddRectangleObjectToContainer(object, &parkingLots_);
}

void ObjectHolder::AddObstacle(const geometry::Point& from, const geometry::Point& to) {
  geometry::RectangleObject* object = 
      new geometry::RectangleObject(from, to, DEFAULT_OBSTACLE_WIDTH);
  AddRectangleObjectToContainer(object, &obstacles_);
}

void ObjectHolder::AddObjectOfCurrentType(const geometry::Point& from, 
    const geometry::Point& to) {
  switch(currentType) {
    case ROAD_SEGMENT: AddRoadSegment(from, to); break;
    case PARKING_LOT: AddParkingLot(from, to); break;
    case OBSTACLE: AddObstacle(from, to); break;
  }
}

bool ObjectHolder::Select(const geometry::Point& location) {
  selected_.clear();
  AddSelectedFromContainer(location, &roadSegments_);
  AddSelectedFromContainer(location, &parkingLots_);
  AddSelectedFromContainer(location, &obstacles_);
  selectedIndex_ = 0;
  return !selected_.empty();
}

const RectangleObjectContainer& ObjectHolder::GetRoadSegments() const {
  return roadSegments_;
}

const RectangleObjectContainer& ObjectHolder::GetParkingLots() const {
  return parkingLots_;
}

const RectangleObjectContainer& ObjectHolder::GetObstacles() const {
  return obstacles_;
}

void ObjectHolder::SelectNext() {
  if (selected_.empty()) {
    return;
  }
  selectedIndex_ = (selectedIndex_ +  1) % selected_.size();
}

void ObjectHolder::SelectPrevious() {
  if (selected_.empty()) {
    return;
  }
  selectedIndex_ = (selectedIndex_ + selected_.size() - 1) % selected_.size();
}

void ObjectHolder::FinalizeSelected() {
  selectedIsFianlized_ = true;
}
 
bool ObjectHolder::IsSelectedFinalized() const {
  return selectedIsFianlized_;
}

geometry::RectangleObject* ObjectHolder::GetSelected() {
  return selected_[selectedIndex_];
}

void ObjectHolder::DeleteSelected() {
  if (selected_.empty()) {
    return;
  }

  geometry::RectangleObject* selected = selected_[selectedIndex_];
  if (FindAndDeleteFromContainer(selected, &roadSegments_)) {
    selected_.clear();
    return;
  }

  if (FindAndDeleteFromContainer(selected, &parkingLots_)) {
    selected_.clear();
    return;
  }

  if (FindAndDeleteFromContainer(selected, &obstacles_)) {
    selected_.clear();
    return;
  }
}

void ObjectHolder::AddRectangleObjectToContainer(
    geometry::RectangleObject* object,
    RectangleObjectContainer* container) {
  container->push_back(object);
  selected_.clear();
  selected_.push_back(object);
  selectedIndex_ = 0;
}

void ObjectHolder::AddSelectedFromContainer(const geometry::Point& location,
    RectangleObjectContainer* container) {
  for (unsigned index = 0; index < container->size(); ++index) {
    if (container->at(index)->ContainsPoint(location)) {
      selected_.push_back(container->at(index));
    }
  }
}

bool ObjectHolder::FindAndDeleteFromContainer(
    geometry::RectangleObject* object,
    RectangleObjectContainer* container) {
  for (unsigned index = 0; index < container->size(); ++index) {
    if (container->at(index) == object) {
      container->erase(container->begin() + index);
      delete object;
      return true;
    }
  }

  return false;
}

}  // namespace utils
