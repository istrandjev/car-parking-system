#include "utils/object_holder.h"

#include "geometry/directed_rectangle_object.h"
#include "geometry/point.h"
#include "geometry/rectangle_object.h"
#include "utils/double_utils.h"

#include <vector>

namespace utils {

static const double DEFAULT_ROAD_WIDTH = 2.5;
static const double DEFAULT_PARKING_WIDTH = 2.5;
static const double DEFAULT_OBSTACLE_WIDTH = 2.0;

ObjectHolder::ObjectHolder() {}

ObjectHolder::~ObjectHolder() {
  DeleteObjects();
}

void ObjectHolder::DeleteObjects() {
  for (unsigned index = 0; index < roadSegments_.size(); ++index) {
    delete roadSegments_[index];
  }
  for (unsigned index = 0; index < parkingLots_.size(); ++index) {
    delete parkingLots_[index];
  }
  for (unsigned index = 0; index < obstacles_.size(); ++index) {
    delete obstacles_[index];
  }
  roadSegments_.clear();
  obstacles_.clear();
  parkingLots_.clear();
}


geometry::RectangleObject* ObjectHolder::AddTwoWayRoadSegment(
    const geometry::Point& from, const geometry::Point& to) {
  geometry::RectangleObject* object =
      new geometry::DirectedRectangleObject(from, to, DEFAULT_ROAD_WIDTH);
  roadSegments_.push_back(object);
  return object;
}

geometry::RectangleObject* ObjectHolder::AddOneWayRoadSegment(
    const geometry::Point& from, const geometry::Point& to) {
  geometry::DirectedRectangleObject* object =
      new geometry::DirectedRectangleObject(from, to, DEFAULT_ROAD_WIDTH);
  roadSegments_.push_back(object);
  object->SetIsOneWay(true);
  return object;
}

geometry::RectangleObject* ObjectHolder::AddParkingLot(
    const geometry::Point& from, const geometry::Point& to) {
  geometry::RectangleObject* object =
      new geometry::RectangleObject(from, to, DEFAULT_PARKING_WIDTH);
  parkingLots_.push_back(object);
  return object;
}

geometry::RectangleObject* ObjectHolder::AddObstacle(
    const geometry::Point& from, const geometry::Point& to) {
  geometry::RectangleObject* object =
      new geometry::RectangleObject(from, to, DEFAULT_OBSTACLE_WIDTH);
  object->SetIsObstacle(true);
  obstacles_.push_back(object);
  return object;
}

bool ObjectHolder::FindAndDelete(geometry::RectangleObject *object) {
  return FindAndDeleteFromContainer(object, &roadSegments_) ||
      FindAndDeleteFromContainer(object, &parkingLots_) ||
      FindAndDeleteFromContainer(object, &obstacles_);
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

void ObjectHolder::GetObectsForLocation(
    const geometry::Point& location, RectangleObjectContainer* container) {
  GetObectsForLocationFromContainer(location, &roadSegments_, container);
  GetObectsForLocationFromContainer(location, &parkingLots_, container);
  GetObectsForLocationFromContainer(location, &obstacles_, container);
}

void ObjectHolder::GetObectsForLocationFromContainer(
    const geometry::Point& location,
    const RectangleObjectContainer* container,
    RectangleObjectContainer* location_objects) {
  for (unsigned index = 0; index < container->size(); ++index) {
    if (container->at(index)->ContainsPoint(location)) {
      location_objects->push_back(container->at(index));
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
