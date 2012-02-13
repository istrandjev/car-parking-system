#ifndef SCENE_H
#define SCENE_H

#include "car.h"

#include <vector>

namespace geometry {
class Polygon;
}  // namespace geometry

namespace visualize {

// This class is used to represent the scene. It actually holds the set of
// all polygons to be drawn and also is responsible for the translations and 
// rotations of the view point.
class Scene {
public:
  // Draws a polygon.
  static void DrawCar(unsigned index);
  
  // Draws all the polygons in the scene.
  static void Draw();

  static void ResetCar(unsigned index);
  static void AddCar(double width, double length, double max_steering_angle);

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

  static void TransformDrawingPane();

private:
  // Sets the current drawing color according to the polygon index. Tries to 
  // choose as differentiable colors as possible.
  static void SetColorForPolygon(unsigned index);

  static std::vector<simulation::Car> cars_;

  static double xTranslation, yTranslation, zTranslation;
  static int width_, height_;
  static double xRotate;
};

}  // namespace visualize

#endif  // SCENE_H