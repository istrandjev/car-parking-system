#include "handlers/event_handlers.h"

#include "geometry/point.h"
#include "geometry/rectangle_object.h"
#include "geometry/vector.h"
#include "utils/current_state.h"
#include "utils/object_holder.h"
#include "utils/user_input_handler.h"
#include "visualize/scene.h"

#include <glut.h>

#include <cmath>
#include <cstdlib>


namespace utils {

bool Near(const geometry::Point& a, const geometry::Point& b) {
  return fabs(a.x - b.x) + fabs(a.y - b.y) < 1.0;
}

void HandleMouseClick(double x, double y);
void HandleMousePress(double x, double y);
void HandleMouseRelease(double x, double y);
void HandleMouseDrag(double fx, double fy, double tx, double ty);

void InitializeHandlers() {
  UserInputHandler::SetLeftMouseClickHandler(HandleMouseClick);
  UserInputHandler::SetLeftMouseDragHandler(HandleMouseDrag);
  UserInputHandler::SetLeftMouseReleaseHandler(HandleMouseRelease);
  UserInputHandler::SetLeftMousePressHandler(HandleMousePress);
}

void HandleMouseClick(double x, double y) {
}

void HandleMousePress(double x, double y) {
}

void HandleMouseRelease(double x, double y) {
}

void HandleMouseDrag(double fx, double fy, double tx, double ty) {
}

void KeyPressed(unsigned char c, int x, int y) {
  UserInputHandler::PressRegularKey(c, x, y);
  if (c == 't') {
    visualize::Scene::ShowHideTurnTip();
  }
}

void KeyReleased(unsigned char c, int x, int y) {
  UserInputHandler::ReleaseRegularKey(c);
}

void SpecialKeyPressed(int c, int x, int y) {
  UserInputHandler::PressSpecialKey(c, x, y);
}

void SpecialKeyReleased(int c, int x, int y) {
  UserInputHandler::ReleaseSpecialKey(c);
}

void MousePressFunc(int button, int state, int x, int y) {
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

void MouseMoveFunc(int x, int y) {
  UserInputHandler::MoveMouse(x, y);
}

void HandleKeyboardEvents() {
  if (UserInputHandler::IsSpecialKeyPressed(GLUT_KEY_LEFT)) {
    visualize::Scene::TurnCarLeft(0);
  }
  if (UserInputHandler::IsSpecialKeyPressed(GLUT_KEY_RIGHT)) {
    visualize::Scene::TurnCarRight(0);
  }
  if (UserInputHandler::IsSpecialKeyPressed(GLUT_KEY_UP)) {
    visualize::Scene::Move(true);
  }
  if (UserInputHandler::IsSpecialKeyPressed(GLUT_KEY_DOWN)) {
    visualize::Scene::Move(false);
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
  if (UserInputHandler::IsRegularKeyPressed('e')) {
    CurrentState::debugFlag = !CurrentState::debugFlag;
  }
  if (UserInputHandler::IsRegularKeyPressed(' ')) {
    visualize::Scene::RestartAnimation();
  }

  if (UserInputHandler::IsRegularKeyPressed('v')) {
    visualize::Scene::MoveCurrentPositionForward();
  }
  if (UserInputHandler::IsRegularKeyPressed('b')) {
    visualize::Scene::MoveCurrentPositionBackward();
  }
}

}  // namespace utils
