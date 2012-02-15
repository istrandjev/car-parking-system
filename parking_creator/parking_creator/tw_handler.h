#ifndef PARKING_CREATOR_PARKING_CREATOR_TW_HANDLER_H_
#define PARKING_CREATOR_PARKING_CREATOR_TW_HANDLER_H_

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
#endif  // PARKING_CREATOR_PARKING_CREATOR_TW_HANDLER_H_
