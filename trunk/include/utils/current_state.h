#ifndef INCLUDE_UTILS_CURRENT_STATE_H_
#define INCLUDE_UTILS_CURRENT_STATE_H_

#include "geometry/point.h"

namespace utils {

class CurrentState {
 public:
  static bool movingFrom;
  static bool movingTo;
  static bool addingNewLine;
  static geometry::Point lastMousePosition;
  static bool debugFlag;
};

}  // namespace utils
#endif  // INCLUDE_UTILS_CURRENT_STATE_H_
