#include "handlers/event_handlers.h"

#include "geometry/point.h"
#include "geometry/rectangle_object.h"
#include "geometry/vector.h"
#include "utils/current_state.h"
#include "utils/object_holder.h"
#include "utils/user_input_handler.h"
#include "visualize/scene.h"

#include <AntTweakBar.h>
#include <glut.h>

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
  visualize::Scene::GetObjectHandler()->Select(geometry::Point(x, y));
}

void HandleMousePress(double x, double y) {
  geometry::Point point(x, y);
  ObjectHandler* obj_handler = visualize::Scene::GetObjectHandler();
  if (obj_handler->HasSelected()) {
    if (!obj_handler->IsSelectedFinalized()) {
     return;
    }

    geometry::RectangleObject* object = obj_handler->GetSelected();
    if (Near(object->GetFrom(), point)) {
      CurrentState::movingFrom = true;
    } else if (Near(object->GetTo(), point)) {
      CurrentState::movingTo = true;
    } else if (!obj_handler->GetSelected()->ContainsPoint(point)) {
      CurrentState::addingNewLine = true;
    }
  } else {
    CurrentState::addingNewLine = true;
  }
}

void HandleMouseRelease(double x, double y) {
  ObjectHandler* obj_handler = visualize::Scene::GetObjectHandler();
  if (obj_handler->HasSelected()) {
    obj_handler->FinalizeSelected();
  }
  CurrentState::movingFrom = false;
  CurrentState::movingTo = false;
  CurrentState::addingNewLine = false;
}

void HandleMouseDrag(double fx, double fy, double tx, double ty) {
  ObjectHandler* obj_handler = visualize::Scene::GetObjectHandler();
  geometry::Point from(fx, fy);
  geometry::Point to(tx, ty);

  if (obj_handler->HasSelected()) {
    geometry::RectangleObject* object = obj_handler->GetSelected();
    if (CurrentState::movingFrom) {
      object->SetFrom(to);
    } else if (CurrentState::movingTo || CurrentState::addingNewLine) {
      if (CurrentState::addingNewLine && !Near(from, object->GetFrom())) {
        obj_handler->AddObjectOfCurrentType(from, to);
      } else {
        object->SetTo(to);
      }
    } else {
      geometry::Vector move(CurrentState::lastMousePosition, to);
      object->SetFrom(object->GetFrom() + move);
      object->SetTo(object->GetTo() + move);
    }

  } else {
    obj_handler->AddObjectOfCurrentType(from, to);
  }
}

void KeyPressed(unsigned char c, int x, int y) {
  if (!TwEventKeyboardGLUT(c, x , y)) {
    UserInputHandler::PressRegularKey(c, x, y);
  }
}

void KeyReleased(unsigned char c, int x, int y) {
  UserInputHandler::ReleaseRegularKey(c);
}

void SpecialKeyPressed(int c, int x, int y) {
  if (!TwEventSpecialGLUT(c, x, y)) {
    UserInputHandler::PressSpecialKey(c, x, y);
  }
}

void SpecialKeyReleased(int c, int x, int y) {
  UserInputHandler::ReleaseSpecialKey(c);
}

void MousePressFunc(int button, int state, int x, int y) {
  if (!TwEventMouseButtonGLUT(button, state, x, y)) {
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
  if (!TwEventMouseMotionGLUT(x, y)) {
    UserInputHandler::MoveMouse(x, y);
  }
}

void HandleKeyboardEvents() {
  if (UserInputHandler::IsSpecialKeyPressed(GLUT_KEY_LEFT)) {
    visualize::Scene::GetObjectHandler()->TranslatedSelectedLeft();
  }
  if (UserInputHandler::IsSpecialKeyPressed(GLUT_KEY_RIGHT)) {
    visualize::Scene::GetObjectHandler()->TranslatedSelectedRight();
  }
  if (UserInputHandler::IsSpecialKeyPressed(GLUT_KEY_UP)) {
    visualize::Scene::GetObjectHandler()->TranslatedSelectedUp();
  }
  if (UserInputHandler::IsSpecialKeyPressed(GLUT_KEY_DOWN)) {
    visualize::Scene::GetObjectHandler()->TranslatedSelectedDown();
  }
  if (UserInputHandler::IsSpecialKeyPressed(GLUT_KEY_PAGE_DOWN)) {
    visualize::Scene::GetObjectHandler()->SelectPrevious();
  }
  if (UserInputHandler::IsSpecialKeyPressed(GLUT_KEY_PAGE_UP)) {
    visualize::Scene::GetObjectHandler()->SelectNext();
  }

  if (UserInputHandler::IsRegularKeyPressed(127)) {
    visualize::Scene::GetObjectHandler()->DeleteSelected();
  }
  if (UserInputHandler::IsRegularKeyPressed(',')) {
    visualize::Scene::GetObjectHandler()->DecreaseSelectedWidth();
  }
  if (UserInputHandler::IsRegularKeyPressed('.')) {
    visualize::Scene::GetObjectHandler()->IncreaseSelectedWidth();
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
}

}  // namespace utils
