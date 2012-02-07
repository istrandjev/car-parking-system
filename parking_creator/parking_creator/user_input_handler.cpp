 #include "user_input_handler.h"

 #include "scene.h"

 #include <glut.h>
 #include <vector>

namespace utils {

static const int CLICK_TOLERANCE = 2;

// static declarations
std::vector<bool> UserInputHandler::regularKeysPressed(256, false);
std::vector<bool> UserInputHandler::specialKeysPressed(256, false);
bool UserInputHandler::leftMousePressed = false;
double UserInputHandler::leftMousePressX = 0; 
double UserInputHandler::leftMousePressY = 0;
bool UserInputHandler::rightMousePressed = false;
double UserInputHandler::rightMousePressX;
double UserInputHandler::rightMousePressY;
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
  UnprojectCoordinates(x, y, leftMousePressX, leftMousePressY);
}

// static
void UserInputHandler::ReleaseLeftMouse(int x, int y) {
  leftMousePressed = false;
  double leftMouseReleaseX, leftMouseReleaseY;
  UnprojectCoordinates(x, y, leftMouseReleaseX, leftMouseReleaseY);
  bool click = fabs(leftMouseReleaseX - leftMousePressX) + 
      fabs(leftMouseReleaseY - leftMousePressY) <= CLICK_TOLERANCE;
  if (click) {
    if (leftMouseClickHandler != NULL) {
      (*leftMouseClickHandler)(leftMousePressX, leftMousePressY);
    }
  } else {
    if (leftMouseDragHandler != NULL) {
      (*leftMouseDragHandler)(leftMousePressX, leftMousePressY, 
          leftMouseReleaseX, leftMouseReleaseY);
    }
  }
}

// static
void UserInputHandler::PressRightMouse(int x, int y) {
  rightMousePressed = true;
  UnprojectCoordinates(x, y, rightMousePressX, rightMousePressY);
}
  
// static
void UserInputHandler::ReleaseRightMouse(int x, int y) {
  rightMousePressed = false;
  double rightMouseReleaseX, rightMouseReleaseY;
  UnprojectCoordinates(x, y, rightMouseReleaseX, rightMouseReleaseY);
  bool click = fabs(rightMouseReleaseX - rightMousePressX) + 
      fabs(rightMouseReleaseY - rightMousePressY) <= CLICK_TOLERANCE;
  if (click) {
    if (rightMouseClickHandler != NULL) {
      (*rightMouseClickHandler)(rightMousePressX, rightMousePressY);
    }
  } else {
    if (rightMouseDragHandler != NULL) {
      (*rightMouseDragHandler)(rightMousePressX, rightMousePressY, 
          rightMouseReleaseX, rightMouseReleaseY);
    }
  }
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
void UserInputHandler::UnprojectCoordinates(int x, int y, double& xf,
    double& yf) {
  double modelview[16], projection[16];
  int viewport[4];
  float z;
  double objz;

  //get the modelview matrix		
  glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
  //get the projection matrix
  glGetDoublev( GL_PROJECTION_MATRIX, projection );
  //get the viewport		
  glGetIntegerv( GL_VIEWPORT, viewport );
	
  //Read the window z co-ordinate 
  //(the z value on that point in unit cube)		
  //glReadPixels( x, viewport[3]-y, 1, 1,
		//GL_DEPTH_COMPONENT, GL_FLOAT, &z );
 
  
  //Unproject the window co-ordinates to 
  //find the world co-ordinates.
  gluUnProject( x, y, 0, modelview, 
	projection, viewport, &xf, &yf, &objz );
  objz -= 2;
  /*xf = x;
  yf = y;*/
}
}  // namespace utils
