#include "current_state.h"

#include "point.h"

namespace utils {

// static declarations
bool CurrentState::movingFrom = false; 
bool CurrentState::movingTo = false; 
bool CurrentState::addingNewLine = false; 
geometry::Point CurrentState::lastMousePosition; 

}  // namespace utils