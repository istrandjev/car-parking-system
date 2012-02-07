 #include "user_input_handler.h"

 // #include "scene.h"

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
MouseDragHandler UserInputHandler::leftMouseDragHandler = NULL;
MouseClickHandler UserInputHandler::rightMouseClickHandler = NULL;
MouseDragHandler UserInputHandler::rightMouseDragHandler = NULL;
KeyPressHandler UserInputHandler::keyPressHandler = NULL;
SpecialKeyPressHandler UserInputHandler::specialKeyPressHandler = NULL;

// static
void UserInputHandler::PressRegularKey(unsigned char key, int x, int y) {
  regularKeysPressed[key] = true;
  if (keyPressHandler != NULL) {
    (*keyPressHandler)(key, x, y);
  }
}
  
// static
void UserInputHandler::ReleaseRegularKey(unsigned char key) {
  regularKeysPressed[key] = false;
}


// static
void UserInputHandler::PressSpecialKey(int key, int x, int y) {
  specialKeysPressed[key] = true;
  if (specialKeyPressHandler != NULL) {
    (*specialKeyPressHandler)(key, x, y);
  }
}

// static
void UserInputHandler::ReleaseSpecialKey(int key) {
  specialKeysPressed[key] = false;
}

// static
void UserInputHandler::PressLeftMouse(int x, int y) {
  leftMousePressed = true;
  UnprojectCoordinates(x, y, &leftMousePress);
}

// static
void UserInputHandler::ReleaseLeftMouse(int x, int y) {
  leftMousePressed = false;
  geometry::Point leftMouseRelease;
  UnprojectCoordinates(x, y, &leftMouseRelease);
  bool click = IsClick(leftMousePress, leftMouseRelease);
  if (click) {
    if (leftMouseClickHandler != NULL) {
      (*leftMouseClickHandler)(leftMousePress.x, leftMousePress.y);
    }
  } else {
    if (leftMouseDragHandler != NULL) {
      (*leftMouseDragHandler)(leftMousePress.x, leftMousePress.y, 
          leftMouseRelease.x, leftMouseRelease.y);
    }
  }
}

// static
void UserInputHandler::PressRightMouse(int x, int y) {
  rightMousePressed = true;
  UnprojectCoordinates(x, y, &rightMousePress);
}
  
// static
void UserInputHandler::ReleaseRightMouse(int x, int y) {
  rightMousePressed = false;

  geometry::Point rightMouseRelease;
  UnprojectCoordinates(x, y, &rightMouseRelease);
  bool click = IsClick(rightMousePress, rightMouseRelease);
  if (click) {
    if (rightMouseClickHandler != NULL) {
      (*rightMouseClickHandler)(rightMousePress.x, rightMousePress.y);
    }
  } else {
    if (rightMouseDragHandler != NULL) {
      (*rightMouseDragHandler)(rightMousePress.x, rightMousePress.y, 
          rightMouseRelease.x, rightMouseRelease.y);
    }
  }
}

// static
void UserInputHandler::MoveMouse(int x, int y) {
  if (!rightMousePressed && !leftMousePressed) {
    return;
  }
  geometry::Point mousePosition;
  UnprojectCoordinates(x, y, &mousePosition);
  bool rightMouseClicked = rightMousePressed && false;
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
void UserInputHandler::SetLeftMouseDragHandler(MouseDragHandler handler) {
  leftMouseDragHandler = handler;  
}

// static
void UserInputHandler::SetRightMouseClickHandler(MouseClickHandler handler) {
  rightMouseClickHandler = handler;  
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