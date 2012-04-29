#ifndef INCLUDE_UTILS_OBJECT_HANDLER_H
#define INCLUDE_UTILS_OBJECT_HANDLER_H

#include "utils/object_holder.h"

namespace utils {
class ObjectHandler {
 public:
  enum ObjectType {
    ROAD_SEGMENT,
    PARKING_LOT,
    OBSTACLE
  };
  ObjectType currentType_;

 public:
  ObjectHandler(ObjectHolder* object_holder);

  void AddObjectOfCurrentType(
      const geometry::Point& from, const geometry::Point& to);

  bool HasSelected() const;
  bool Select(const geometry::Point& location);

  void SelectNext();
  void SelectPrevious();

  void FinalizeSelected();
  bool IsSelectedFinalized() const;

  geometry::RectangleObject* GetSelected();

  ObjectHolder* GetObjectHolder();
  ObjectType GetCurrentType() const;

  void DeleteSelected();

  void TranslatedSelectedUp();
  void TranslatedSelectedDown();
  void TranslatedSelectedLeft();
  void TranslatedSelectedRight();

  void IncreaseSelectedWidth();
  void DecreaseSelectedWidth();

  void AddSibling();

 private:
  ObjectHolder* objectHolder_;
  
  RectangleObjectContainer selected_;
  unsigned selectedIndex_;
  bool selectedIsFinalized_;
};
}  // namespace utils
#endif // INCLUDE_UTILS_OBJECT_HANDLER_H
