#ifndef TW_HANDLER_H
#define TW_HANDLER_H

#include <AntTweakBar.h>

namespace visualize {

class TwHandler {
 public:
  static void Init();
  static void SetSize(int width, int height);
  static void Draw();

 private:
  static TwBar *bar;  
};

}  // namespace visualize
#endif TW_HANDLER_H