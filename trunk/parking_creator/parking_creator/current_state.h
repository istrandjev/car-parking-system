#ifndef CURRENT_STATE_H
#define CURRENT_STATE_H

#include "point.h"

namespace utils {

class CurrentState {
 public:
  static bool movingFrom;
  static bool movingTo;
  static bool addingNewLine;
  static geometry::Point lastMousePosition; 
};

}  // namespace utils
#endif  // CURRENT_STATE_H