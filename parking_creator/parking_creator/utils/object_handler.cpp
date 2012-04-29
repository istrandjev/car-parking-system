#include "utils/object_handler.h"

#include "geometry/directed_rectangle_object.h"
#include "geometry/point.h"
#include "geometry/rectangle_object.h"
#include "utils/double_utils.h"
#include "utils/object_holder.h"

#include <vector>

namespace utils {
static const double TRANSLATION_STEP = 0.2;
static const double WIDTH_STEP = 0.1;
static const double MIN_WIDTH = 0.5;

ObjectHandler::ObjectHandler(ObjectHolder* object_holder)
  : selectedIsFinalized_(false), objectHolder_(object_holder),
    currentType_(ROAD_SEGMENT) {}


void ObjectHandler::AddObjectOfCurrentType(
    const geometry::Point& from, const geometry::Point& to) {
  geometry::RectangleObject* object = NULL;
  switch (currentType_) {
    case ROAD_SEGMENT:
        object = objectHolder_->AddTwoWayRoadSegment(from, to);
        break;
    case PARKING_LOT:
        object = objectHolder_->AddParkingLot(from, to);
        break;
    case OBSTACLE:
        object = objectHolder_->AddObstacle(from, to);
        break;
    default: return;
  }
  selected_.clear();
  selected_.push_back(object);
  selectedIndex_ = 0;
}

bool ObjectHandler::HasSelected() const {
  return !selected_.empty();
}

bool ObjectHandler::Select(const geometry::Point& location) {
  selected_.clear();
  objectHolder_->GetObectsForLocation(location, &selected_);
  selectedIndex_ = 0;
  return !selected_.empty();
}

void ObjectHandler::SelectNext() {
  if (selected_.empty()) {
    return;
  }
  selectedIndex_ = (selectedIndex_ +  1) % selected_.size();
}

void ObjectHandler::SelectPrevious() {
  if (selected_.empty()) {
    return;
  }
  selectedIndex_ = (selectedIndex_ + selected_.size() - 1) % selected_.size();
}

void ObjectHandler::FinalizeSelected() {
  selectedIsFinalized_ = true;
}

bool ObjectHandler::IsSelectedFinalized() const {
  return selectedIsFinalized_;
}

geometry::RectangleObject* ObjectHandler::GetSelected() {
  return selected_[selectedIndex_];
}

ObjectHolder* ObjectHandler::GetObjectHolder() {
  return objectHolder_;
}

ObjectHandler::ObjectType ObjectHandler::GetCurrentType() const {
  return currentType_;
}

void ObjectHandler::DeleteSelected() {
  if (!HasSelected()) {
    return;
  }

  geometry::RectangleObject* selected = selected_[selectedIndex_];
  if (objectHolder_->FindAndDelete(selected)) {
    selected_.clear();
  }
}

void ObjectHandler::TranslatedSelectedUp() {
  if (!HasSelected()) {
    return;
  }
  GetSelected()->Translate(0, TRANSLATION_STEP);
}

void ObjectHandler::TranslatedSelectedDown() {
  if (!HasSelected()) {
    return;
  }
  GetSelected()->Translate(0, -TRANSLATION_STEP);
}

void ObjectHandler::TranslatedSelectedLeft() {
  if (!HasSelected()) {
    return;
  }
  GetSelected()->Translate(-TRANSLATION_STEP, 0);
}

void ObjectHandler::TranslatedSelectedRight() {
  if (!HasSelected()) {
    return;
  }
  GetSelected()->Translate(TRANSLATION_STEP, 0);
}

void ObjectHandler::IncreaseSelectedWidth() {
  if (!HasSelected()) {
    return;
  }
  geometry::RectangleObject* object = GetSelected();
  object->SetWidth(object->GetWidth() + WIDTH_STEP);
}

void ObjectHandler::DecreaseSelectedWidth() {
  if (!HasSelected()) {
    return;
  }
  geometry::RectangleObject* object = GetSelected();
  if (DoubleIsGreaterOrEqual(object->GetWidth() - WIDTH_STEP, MIN_WIDTH)) {
    object->SetWidth(object->GetWidth() - WIDTH_STEP);
  }
}

void ObjectHandler::AddSibling() {
  geometry::RectangleObject* selected = GetSelected();
  if (!HasSelected() || !selected->IsDirected()) {
    return;
  }

  geometry::RectangleObject* sibling = objectHolder_->
      AddOneWayRoadSegment(selected->GetTo(), selected->GetFrom());
  sibling->TranslateByNormal(sibling->GetWidth());
  sibling->SetWidth(selected->GetWidth());
  
  selected_.clear();
  selected_.push_back(sibling);
  selectedIndex_ = 0;
}

}  // namespace utils
