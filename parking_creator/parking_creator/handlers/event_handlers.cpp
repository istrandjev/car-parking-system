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
  visualize::Scene::GetObjectHolder()->Select(geometry::Point(x, y));
}

void HandleMousePress(double x, double y) {
  geometry::Point point(x, y);
  ObjectHolder* obj_holder = visualize::Scene::GetObjectHolder();
  if (obj_holder->HasSelected()) {
    if (!obj_holder->IsSelectedFinalized()) {
     return;
    }

    geometry::RectangleObject* object = obj_holder->GetSelected();
    if (Near(object->GetFrom(), point)) {
      CurrentState::movingFrom = true;
    } else if (Near(object->GetTo(), point)) {
      CurrentState::movingTo = true;
    } else if (!obj_holder->GetSelected()->ContainsPoint(point)) {
      CurrentState::addingNewLine = true;
    }
  } else {
    CurrentState::addingNewLine = true;
  }
}

void HandleMouseRelease(double x, double y) {
  ObjectHolder* obj_holder = visualize::Scene::GetObjectHolder();
  if (obj_holder->HasSelected()) {
    obj_holder->FinalizeSelected();
  }
  CurrentState::movingFrom = false;
  CurrentState::movingTo = false;
  CurrentState::addingNewLine = false;
}

void HandleMouseDrag(double fx, double fy, double tx, double ty) {
  ObjectHolder* obj_holder = visualize::Scene::GetObjectHolder();
  geometry::Point from(fx, fy);
  geometry::Point to(tx, ty);

  if (obj_holder->HasSelected()) {
    geometry::RectangleObject* object = obj_holder->GetSelected();
    if (CurrentState::movingFrom) {
      object->SetFrom(to);
    } else if (CurrentState::movingTo || CurrentState::addingNewLine) {
      if (CurrentState::addingNewLine && !Near(from, object->GetFrom())) {
        obj_holder->AddObjectOfCurrentType(from, to);
      } else {
        object->SetTo(to);
      }
    } else {
      geometry::Vector move(CurrentState::lastMousePosition, to);
      object->SetFrom(object->GetFrom() + move);
      object->SetTo(object->GetTo() + move);
    }

  } else {
    obj_holder->AddObjectOfCurrentType(from, to);
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
    visualize::Scene::GetObjectHolder()->TranslatedSelectedLeft();
  }
  if (UserInputHandler::IsSpecialKeyPressed(GLUT_KEY_RIGHT)) {
    visualize::Scene::GetObjectHolder()->TranslatedSelectedRight();
  }
  if (UserInputHandler::IsSpecialKeyPressed(GLUT_KEY_UP)) {
    visualize::Scene::GetObjectHolder()->TranslatedSelectedUp();
  }
  if (UserInputHandler::IsSpecialKeyPressed(GLUT_KEY_DOWN)) {
    visualize::Scene::GetObjectHolder()->TranslatedSelectedDown();
  }
  if (UserInputHandler::IsSpecialKeyPressed(GLUT_KEY_PAGE_DOWN)) {
    visualize::Scene::GetObjectHolder()->SelectPrevious();
  }
  if (UserInputHandler::IsSpecialKeyPressed(GLUT_KEY_PAGE_UP)) {
    visualize::Scene::GetObjectHolder()->SelectNext();
  }

  if (UserInputHandler::IsRegularKeyPressed(127)) {
    visualize::Scene::GetObjectHolder()->DeleteSelected();
  }
  if (UserInputHandler::IsRegularKeyPressed(',')) {
    visualize::Scene::GetObjectHolder()->DecreaseSelectedWidth();
  }
  if (UserInputHandler::IsRegularKeyPressed('.')) {
    visualize::Scene::GetObjectHolder()->IncreaseSelectedWidth();
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