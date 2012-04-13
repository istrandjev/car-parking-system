#ifndef CAR_SIMULATION_CAR_SIMULATION_HANDLERS_EVENT_HANDLERS_H_
#define CAR_SIMULATION_CAR_SIMULATION_HANDLERS_EVENT_HANDLERS_H_

namespace utils {

void InitializeHandlers();

void KeyPressed(unsigned char c, int x, int y);
void KeyReleased(unsigned char c, int x, int y);
void SpecialKeyPressed(int c, int x, int y);
void SpecialKeyReleased(int c, int x, int y);
void MousePressFunc(int button, int state, int x, int y);
void MouseMoveFunc(int x, int y);

void HandleKeyboardEvents();

}  // namespace utils
#endif  // CAR_SIMULATION_CAR_SIMULATION_HANDLERS_EVENT_HANDLERS_H_