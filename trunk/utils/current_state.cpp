#include "utils/current_state.h"

#include "geometry/point.h"

namespace utils {

// static declarations
bool CurrentState::movingFrom = false;
bool CurrentState::movingTo = false;
bool CurrentState::addingNewLine = false;
geometry::Point CurrentState::lastMousePosition;
bool CurrentState::debugFlag = false;

}  // namespace utils
