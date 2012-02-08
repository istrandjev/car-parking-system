#include "object_holder.h"

#include "point.h"
#include "rectangle_object.h"

#include <vector>

namespace utils {

static const double DEFAULT_WIDTH = 1.0;

ObjectHolder::ObjectHolder() : selectedIsFianlized_(false) {}

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
  AddRectangleObjectToContainer(from, to, &roadSegments_);
}

void ObjectHolder::AddParkingLot(const geometry::Point& from, const geometry::Point& to) {
  AddRectangleObjectToContainer(from, to, &parkingLots_);
}

void ObjectHolder::AddObstacle(const geometry::Point& from, const geometry::Point& to) {
  AddRectangleObjectToContainer(from, to, &obstacles_);
  
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
 
void ObjectHolder::AddRectangleObjectToContainer(const geometry::Point& from, 
    const geometry::Point& to, RectangleObjectContainer* container) {
  geometry::RectangleObject* object = 
      new geometry::RectangleObject(from, to, DEFAULT_WIDTH);
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

}  // namespace utils
