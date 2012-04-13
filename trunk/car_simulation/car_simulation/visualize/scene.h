#ifndef SCENE_H
#define SCENE_H

#include "simulation/car.h"
#include "utils/object_holder.h"

#include <vector>

namespace geometry {
class DirectedRectangleObject;
class Polygon;
}  // namespace geometry

namespace utils {
class IntersectionHandler;
}  // namespace utils

namespace simulation {
class CarMovementHandler;  
}  // namespace simulation

namespace visualize {

// This class is used to represent the scene. It actually holds the set of
// all polygons to be drawn and also is responsible for the translations and 
// rotations of the view point.
class Scene {
public:
  // Draws all the objects in the scene.
  static void Draw();

  static void ResetCar(unsigned index);
  static void AddCar(double width, double length, double max_steering_angle);
  static void AddCar(double width, double length, double max_steering_angle, 
      const geometry::Point& center, const geometry::Point& second_point);

  static void TurnCarLeft(unsigned index);
  static void TurnCarRight(unsigned index);

  static void Move(bool forward);

  // Functions to translate the scene.
  static void TranslateLeft();
  static void TranslateRight();
  static void TranslateUp();
  static void TranslateDown();
  static void TranslateIn();
  static void TranslateOut();

  // Functions to rotate the scene.
  static void RotateXPositive();
  static void RotateXNegative();

  static void Resize(int width, int height);
  static int Width();
  static int Height();

  static void SetObjectHolder(utils::ObjectHolder* object_holder);
  static utils::ObjectHolder* GetObjectHolder();
  
  static void SetCarMovementHandler(
      simulation::CarMovementHandler* car_movement_handler);

  static utils::IntersectionHandler* GetIntersectionHandler();

  static void TransformDrawingPane();

  static void ShowHideTurnTip();

 private:
  static void DrawCar(unsigned index);
  static void DrawObjects();
  static void DrawSelected(const geometry::Polygon& polygon);
  static void DrawObjectsFromContainer(
      const utils::RectangleObjectContainer& container);
  static void DrawDirectionalTips(
      const geometry::DirectedRectangleObject& directed_object);
  static void DrawBorderLines();

  // Sets the current drawing color according to the polygon index. Tries to 
  // choose as differentiable colors as possible.
  static void SetColorForPolygon(unsigned index);
 
 private:
  static simulation::CarMovementHandler* carMovementHandler_;
  static utils::ObjectHolder* objectHolder_;
  static std::vector<simulation::Car> cars_;

  static double xTranslation, yTranslation, zTranslation;
  static int width_, height_;
  static bool showTurnTip_;
  static double xRotate;
};

}  // namespace visualize

#endif  // SCENE_H