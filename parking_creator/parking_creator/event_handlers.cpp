#include "event_handlers.h"

#include "object_holder.h"
#include "point.h"
#include "rectangle_object.h"
#include "scene.h"
#include "user_input_handler.h"

#include <AntTweakBar.h>
#include <cstdlib>
#include <glut.h>

namespace utils {

bool Near(const geometry::Point& a, const geometry::Point& b) {
  return fabs(a.x - b.x) + fabs(a.y - b.y) < 10.0; 
}

void HandleMouseClick(double x, double y);
void HandleMouseDrag(double fx, double fy, double tx, double ty);

void InitializeHandlers() {
	UserInputHandler::SetLeftMouseClickHandler(HandleMouseClick);
	UserInputHandler::SetLeftMouseDragHandler(HandleMouseDrag);
}

void HandleMouseClick(double x, double y) {
  visualize::Scene::GetObjectHolder()->Select(geometry::Point(x, y));
}

void HandleMouseDrag(double fx, double fy, double tx, double ty) {
  ObjectHolder* obj_holder = visualize::Scene::GetObjectHolder();
  geometry::Point from(fx, fy);
  geometry::Point to(tx, ty);
  if (obj_holder->HasSelected()) {
    geometry::RectangleObject* object = obj_holder->GetSelected();
    if (Near(from, object->GetFrom())) {
      object->SetTo(to);
    } else if (Near(from, object->GetTo())) {
      object->SetFrom(to);
    } else if (!object->ContainsPoint(from)) {
      obj_holder->AddRoadSegment(from, to);
    }
  } else {
    obj_holder->AddRoadSegment(from, to);
  }
  // visualize::Scene::ResetSegment(fx, fy, tx, ty);
}

void KeyPressed(unsigned char c, int x, int y) {
  if (!TwEventKeyboardGLUT(c, x , y)) {
    UserInputHandler::PressRegularKey(c, x, y);
  }
}

void KeyReleased(unsigned char c, int x, int y) {
  if (!TwEventKeyboardGLUT(c, x , y)) {
    UserInputHandler::ReleaseRegularKey(c);
  }
}

void SpecialKeyPressed(int c, int x, int y) {
  if (!TwEventSpecialGLUT(c, x, y)) {
    UserInputHandler::PressSpecialKey(c, x, y);
  }
}

void SpecialKeyReleased(int c, int x, int y) {
  if (!TwEventSpecialGLUT(c, x, y)) {
    UserInputHandler::ReleaseSpecialKey(c);
  }
}

void MousePressFunc(int button, int state, int x, int y) {
  if (!TwEventMouseButtonGLUT(button, state, x, y) ) {
	  if (state == GLUT_DOWN) {
		  if (button == GLUT_LEFT_BUTTON) {
        UserInputHandler::PressLeftMouse(x, y);
      } else {
        UserInputHandler::PressRightMouse(x, y);
      }
	  } else if (state = GLUT_UP) {
      if (button == GLUT_LEFT_BUTTON) {
        UserInputHandler::ReleaseLeftMouse(x, y);
      } else {
        UserInputHandler::ReleaseRightMouse(x, y);
      }
    }
  }
}

void MouseMoveFunc(int x, int y) {
  if (!TwEventMouseMotionGLUT(x, y) ) {
    UserInputHandler::MoveMouse(x, y);
  }
}

void HandleKeyboardEvents() {
  if (UserInputHandler::IsSpecialKeyPressed(GLUT_KEY_LEFT)) {
    visualize::Scene::TurnCarLeft();
  }
  if (UserInputHandler::IsSpecialKeyPressed(GLUT_KEY_RIGHT)) {
    visualize::Scene::TurnCarRight();
  }
  if (UserInputHandler::IsSpecialKeyPressed(GLUT_KEY_UP)) {
    visualize::Scene::MoveForward();
  }
  if (UserInputHandler::IsSpecialKeyPressed(GLUT_KEY_DOWN)) {
    visualize::Scene::MoveBackward();
  }
  if (UserInputHandler::IsRegularKeyPressed('a')) {
    visualize::Scene::TranslateLeft();
  }
  if (UserInputHandler::IsRegularKeyPressed('d')) {
    visualize::Scene::TranslateRight();
  }
  if (UserInputHandler::IsRegularKeyPressed('w')) {
    visualize::Scene::TranslateUp();
  }
  if (UserInputHandler::IsRegularKeyPressed('s')) {
    visualize::Scene::TranslateDown();
  }
  if (UserInputHandler::IsRegularKeyPressed(' ')) {
    visualize::Scene::ResetCarPosition();
  }
}

}  // namespace utils