#ifndef USER_INPUT_HANDLER_H
#define USER_INPUT_HANDLER_H

#include <vector>

namespace utils {

typedef void (*MouseClickHandler)(double, double);
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

  static bool IsRegularKeyPressed(unsigned char key);
  static bool IsSpecialKeyPressed(unsigned char key);

  static void SetLeftMouseClickHandler(MouseClickHandler handler);
  static void SetLeftMouseDragHandler(MouseDragHandler handler);

  static void SetRightMouseClickHandler(MouseClickHandler handler);
  static void SetRightMouseDragHandler(MouseDragHandler handler);

  static void SetKeyPressHandler(KeyPressHandler handler);
  static void SetSpecialKeyPressHandler(SpecialKeyPressHandler handler);

private:
  static void UnprojectCoordinates(int x, int y, double& xf, double& yf);

private:
  static std::vector<bool> regularKeysPressed;
  static std::vector<bool> specialKeysPressed;
  static bool leftMousePressed;
  static double leftMousePressX, leftMousePressY;
  static bool rightMousePressed;
  static double rightMousePressX, rightMousePressY;

  // Event handlers
  static MouseClickHandler leftMouseClickHandler;
  static MouseDragHandler leftMouseDragHandler;
  static MouseClickHandler rightMouseClickHandler;
  static MouseDragHandler rightMouseDragHandler;
  static KeyPressHandler keyPressHandler;
  static SpecialKeyPressHandler specialKeyPressHandler;
};

}  // namespace utils

#endif  // USER_INPUT_HANDLER_H