 #include "utils/user_input_handler.h"

 #include "geometry/point.h"
 #include "utils/current_state.h"

 #include <glut.h>
 #include <vector>

namespace utils {

static const int CLICK_TOLERANCE = 2;

// static declarations
std::vector<bool> UserInputHandler::regularKeysPressed(256, false);
std::vector<bool> UserInputHandler::specialKeysPressed(256, false);
bool UserInputHandler::leftMousePressed = false;
geometry::Point UserInputHandler::leftMousePress; 
bool UserInputHandler::rightMousePressed = false;
geometry::Point UserInputHandler::rightMousePress; 
MouseClickHandler UserInputHandler::leftMouseClickHandler = NULL;
MousePressHandler UserInputHandler::leftMousePressHandler = NULL;
MouseReleaseHandler UserInputHandler::leftMouseReleaseHandler = NULL;
MouseDragHandler UserInputHandler::leftMouseDragHandler = NULL;
MouseClickHandler UserInputHandler::rightMouseClickHandler = NULL;
MousePressHandler UserInputHandler::rightMousePressHandler = NULL;
MouseReleaseHandler UserInputHandler::rightMouseReleaseHandler = NULL;
MouseDragHandler UserInputHandler::rightMouseDragHandler = NULL;
KeyPressHandler UserInputHandler::keyPressHandler = NULL;
SpecialKeyPressHandler UserInputHandler::specialKeyPressHandler = NULL;

// static
void UserInputHandler::PressRegularKey(unsigned char key, int x, int y) {
  if (keyPressHandler != NULL) {
    (*keyPressHandler)(key, x, y);
  }
  regularKeysPressed[key] = true;
}
  
// static
void UserInputHandler::ReleaseRegularKey(unsigned char key) {
  regularKeysPressed[key] = false;
}


// static
void UserInputHandler::PressSpecialKey(int key, int x, int y) {
  if (specialKeyPressHandler != NULL) {
    (*specialKeyPressHandler)(key, x, y);
  }
  specialKeysPressed[key] = true;
}

// static
void UserInputHandler::ReleaseSpecialKey(int key) {
  specialKeysPressed[key] = false;
}

// static
void UserInputHandler::PressLeftMouse(int x, int y) {
  UnprojectCoordinates(x, y, &leftMousePress);
  if (leftMousePressHandler != NULL) {
    (*leftMousePressHandler)(leftMousePress.x, leftMousePress.y);
  }
  leftMousePressed = true;
}

// static
void UserInputHandler::ReleaseLeftMouse(int x, int y) {
  geometry::Point leftMouseRelease;
  UnprojectCoordinates(x, y, &leftMouseRelease);
  bool click = IsClick(leftMousePress, leftMouseRelease);
  if (click) {
    if (leftMouseClickHandler != NULL) {
      (*leftMouseClickHandler)(leftMousePress.x, leftMousePress.y);
    }
  }
  if (leftMouseReleaseHandler != NULL) {
    (*leftMouseReleaseHandler)(leftMouseRelease.x, leftMouseRelease.y);
  }
  leftMousePressed = false;
}

// static
void UserInputHandler::PressRightMouse(int x, int y) {
  UnprojectCoordinates(x, y, &rightMousePress);
  if (rightMousePressHandler != NULL) {
    (*rightMousePressHandler)(rightMousePress.x, rightMousePress.y);
  }
  rightMousePressed = true;
}
  
// static
void UserInputHandler::ReleaseRightMouse(int x, int y) {
  geometry::Point rightMouseRelease;
  UnprojectCoordinates(x, y, &rightMouseRelease);
  bool click = IsClick(rightMousePress, rightMouseRelease);
  if (click) {
    if (rightMouseClickHandler != NULL) {
      (*rightMouseClickHandler)(rightMousePress.x, rightMousePress.y);
    }
  } 
  if (rightMouseReleaseHandler != NULL) {
    (*rightMouseReleaseHandler)(rightMouseRelease.x, rightMouseRelease.y);
  }
  rightMousePressed = false;
}

// static
void UserInputHandler::MoveMouse(int x, int y) {
  geometry::Point mousePosition;
  UnprojectCoordinates(x, y, &mousePosition);
  if (!rightMousePressed && !leftMousePressed) {
    CurrentState::lastMousePosition = mousePosition;
    return;
  }
  
  if (leftMouseDragHandler != NULL) {
      bool leftMouseDragged = leftMousePressed && 
          !IsClick(mousePosition, leftMousePress);
      if (leftMouseDragged) {
        (*leftMouseDragHandler)(leftMousePress.x, leftMousePress.y, 
          mousePosition.x, mousePosition.y);
      }
  }

  if (rightMouseDragHandler != NULL) {
      bool rightMouseDragged = rightMousePressed && 
          !IsClick(mousePosition, rightMousePress);
      if (rightMouseDragged) {
        (*rightMouseDragHandler)(rightMousePress.x, rightMousePress.y, 
          mousePosition.x, mousePosition.y);
      }
  }

  CurrentState::lastMousePosition = mousePosition;
}

// static
bool UserInputHandler::IsRegularKeyPressed(unsigned char key) {
  return regularKeysPressed[key];
}

// static
bool UserInputHandler::IsSpecialKeyPressed(unsigned char key) {
  return specialKeysPressed[key];
}


// static 
void UserInputHandler::SetLeftMouseClickHandler(MouseClickHandler handler) {
  leftMouseClickHandler = handler;
}

// static 
void UserInputHandler::SetLeftMousePressHandler(MousePressHandler handler) {
  leftMousePressHandler = handler;
}

// static 
void UserInputHandler::SetLeftMouseReleaseHandler(MouseReleaseHandler handler) {
  leftMouseReleaseHandler = handler;
}

// static
void UserInputHandler::SetLeftMouseDragHandler(MouseDragHandler handler) {
  leftMouseDragHandler = handler;  
}

// static
void UserInputHandler::SetRightMouseClickHandler(MouseClickHandler handler) {
  rightMouseClickHandler = handler;  
}

// static
void UserInputHandler::SetRightMousePressHandler(MousePressHandler handler) {
  rightMousePressHandler = handler;  
}

// static
void UserInputHandler::SetRightMouseReleaseHandler(MouseReleaseHandler handler) {
  rightMouseReleaseHandler = handler;  
}

// static
void UserInputHandler::SetRightMouseDragHandler(MouseDragHandler handler) {
  rightMouseDragHandler = handler;
}

// static
void UserInputHandler::SetKeyPressHandler(KeyPressHandler handler) {
  keyPressHandler = handler;
}

// static
void UserInputHandler::SetSpecialKeyPressHandler(
    SpecialKeyPressHandler handler) {
  specialKeyPressHandler = handler;
}

// static
void UserInputHandler::UnprojectCoordinates(int x, int y, geometry::Point* point) {
  double modelview[16], projection[16];
  int viewport[4];
  double objz;

  //get the modelview matrix		
  glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
  //get the projection matrix
  glGetDoublev( GL_PROJECTION_MATRIX, projection );
  //get the viewport		
  glGetIntegerv( GL_VIEWPORT, viewport );
  
  //Unproject the window co-ordinates to 
  //find the world co-ordinates.
  double xd, yd;
  gluUnProject( x, y, 0, modelview, projection, viewport, &xd, &yd, &objz);
  point->x = xd;
  point->y = -yd;
}

// static
bool UserInputHandler::IsClick(const geometry::Point& from, 
    const geometry::Point& to) {
  return fabs(from.x - to.x) + fabs(from.y - to.y) <= CLICK_TOLERANCE;
}

}  // namespace utils
