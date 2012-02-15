#ifndef INCLUDE_UTILS_TW_HANDLER_H_
#define INCLUDE_UTILS_TW_HANDLER_H_

#include <AntTweakBar.h>

namespace visualize {

class TwHandler {
 public:
  static void Init();
  static void SetSize(int width, int height);
  static void ModifyAccordingToState();
  static void Draw();

 private:
  static void AddObjectTypeEnum();

 private:
  static TwBar *bar;
};

}  // namespace visualize
#endif  // INCLUDE_UTILS_TW_HANDLER_H_
