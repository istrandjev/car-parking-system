#include "event_handlers.h"

#include "scene.h"
#include "user_input_handler.h"

#include <AntTweakBar.h>
#include <cstdlib>
#include <glut.h>

namespace utils {


void HandleMouseClick(double x, double y);
void HandleMouseDrag(double fx, double fy, double tx, double ty);

void InitializeHandlers() {
	UserInputHandler::SetLeftMouseClickHandler(HandleMouseClick);
	UserInputHandler::SetLeftMouseDragHandler(HandleMouseDrag);
}

void HandleMouseClick(double x, double y) {
  visualize::Scene::DoubleLineWidth();
}

void HandleMouseDrag(double fx, double fy, double tx, double ty) {
  visualize::Scene::ResetSegment(fx, fy, tx, ty);
}

void KeyPressed(unsigned char c, int x, int y) {
  if (!TwEventKeyboardGLUT(c, x , y)) {
    utils::UserInputHandler::PressRegularKey(c, x, y);
  }
}

void KeyReleased(unsigned char c, int x, int y) {
  if (!TwEventKeyboardGLUT(c, x , y)) {
    utils::UserInputHandler::ReleaseRegularKey(c);
  }
}


void SpecialKeyPressed(int c, int x, int y) {
  if (!TwEventSpecialGLUT(c, x, y)) {
    utils::UserInputHandler::PressSpecialKey(c, x, y);
  }
}

void SpecialKeyReleased(int c, int x, int y) {
  if (!TwEventSpecialGLUT(c, x, y)) {
    utils::UserInputHandler::ReleaseSpecialKey(c);
  }
}

void MousePressFunc(int button, int state, int x, int y) {
  if (!TwEventMouseButtonGLUT(button, state, x, y) ) {
	  if (state == GLUT_DOWN) {
		  if (button == GLUT_LEFT_BUTTON) {
        utils::UserInputHandler::PressLeftMouse(x, y);
      } else {
        utils::UserInputHandler::PressRightMouse(x, y);
      }
	  } else if (state = GLUT_UP) {
      if (button == GLUT_LEFT_BUTTON) {
        utils::UserInputHandler::ReleaseLeftMouse(x, y);
      } else {
        utils::UserInputHandler::ReleaseRightMouse(x, y);
      }
    }
  }
}

void MouseMoveFunc(int x, int y) {
  if (!TwEventMouseMotionGLUT(x, y) ) {
    utils::UserInputHandler::MoveMouse(x, y);
  }
}

void HandleKeyboardEvents() {
  if (utils::UserInputHandler::IsSpecialKeyPressed(GLUT_KEY_LEFT)) {
    visualize::Scene::TurnCarLeft();
  }
  if (utils::UserInputHandler::IsSpecialKeyPressed(GLUT_KEY_RIGHT)) {
    visualize::Scene::TurnCarRight();
  }
  if (utils::UserInputHandler::IsSpecialKeyPressed(GLUT_KEY_UP)) {
    visualize::Scene::MoveForward();
  }
  if (utils::UserInputHandler::IsSpecialKeyPressed(GLUT_KEY_DOWN)) {
    visualize::Scene::MoveBackward();
  }
  if (utils::UserInputHandler::IsRegularKeyPressed('a')) {
    visualize::Scene::TranslateLeft();
  }
  if (utils::UserInputHandler::IsRegularKeyPressed('d')) {
    visualize::Scene::TranslateRight();
  }
  if (utils::UserInputHandler::IsRegularKeyPressed('w')) {
    visualize::Scene::TranslateUp();
  }
  if (utils::UserInputHandler::IsRegularKeyPressed('s')) {
    visualize::Scene::TranslateDown();
  }
  if (utils::UserInputHandler::IsRegularKeyPressed(' ')) {
    visualize::Scene::ResetCarPosition();
  }
}

}  // namespace utils