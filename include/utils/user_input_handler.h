#ifndef INCLUDE_UTILS_USER_INPUT_HANDLER_H_
#define INCLUDE_UTILS_USER_INPUT_HANDLER_H_

#include "geometry/point.h"

#include <vector>

namespace utils {

typedef void (*MouseClickHandler)(double, double);
typedef void (*MousePressHandler)(double, double);
typedef void (*MouseReleaseHandler)(double, double);
typedef void (*MouseDragHandler)(double, double, double, double);
typedef void (*KeyPressHandler)(unsigned char, double, double);
typedef void (*SpecialKeyPressHandler)(int, double, double);

class UserInputHandler {
 public:
  static void PressRegularKey(unsigned char key, int x, int y);
  static void ReleaseRegularKey(unsigned char key);
  static void PressSpecialKey(int key, int x, int y);
  static void ReleaseSpecialKey(int key);
  static void PressLeftMouse(int x, int y);
  static void ReleaseLeftMouse(int x, int y);
  static void PressRightMouse(int x, int y);
  static void ReleaseRightMouse(int x, int y);
  static void MoveMouse(int x, int y);

  static bool IsRegularKeyPressed(unsigned char key);
  static bool IsSpecialKeyPressed(unsigned char key);

  static void SetLeftMouseClickHandler(MouseClickHandler handler);
  static void SetLeftMousePressHandler(MousePressHandler handler);
  static void SetLeftMouseReleaseHandler(MouseReleaseHandler handler);
  static void SetLeftMouseDragHandler(MouseDragHandler handler);

  static void SetRightMouseClickHandler(MouseClickHandler handler);
  static void SetRightMousePressHandler(MousePressHandler handler);
  static void SetRightMouseReleaseHandler(MouseReleaseHandler handler);
  static void SetRightMouseDragHandler(MouseDragHandler handler);

  static void SetKeyPressHandler(KeyPressHandler handler);
  static void SetSpecialKeyPressHandler(SpecialKeyPressHandler handler);

 private:
  static void UnprojectCoordinates(int x, int y, geometry::Point* point);
  static bool IsClick(const geometry::Point& from, const geometry::Point& to);

 private:
  static std::vector<bool> regularKeysPressed;
  static std::vector<bool> specialKeysPressed;
  static bool leftMousePressed;
  static geometry::Point leftMousePress;
  static bool rightMousePressed;
  static geometry::Point rightMousePress;

  // Event handlers
  static MouseClickHandler leftMouseClickHandler;
  static MousePressHandler leftMousePressHandler;
  static MouseReleaseHandler leftMouseReleaseHandler;
  static MouseDragHandler leftMouseDragHandler;
  static MouseClickHandler rightMouseClickHandler;
  static MousePressHandler rightMousePressHandler;
  static MouseReleaseHandler rightMouseReleaseHandler;
  static MouseDragHandler rightMouseDragHandler;
  static KeyPressHandler keyPressHandler;
  static SpecialKeyPressHandler specialKeyPressHandler;
};

}  // namespace utils

#endif  // INCLUDE_UTILS_USER_INPUT_HANDLER_H_
