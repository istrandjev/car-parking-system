#ifndef SCENE_H
#define SCENE_H

#include "object_holder.h"
#include "scoped_ptr.h"

#include <vector>

namespace geometry {
class Point;
class Polygon;
class RectangleObject;
}  // namespace geometry

namespace simulation {
  class Car;
}  // namespace simulation

namespace visualize {

// This class is used to represent the scene. It actually holds the set of
// all polygons to be drawn and also is responsible for the translations and 
// rotations of the view point.
class Scene {
public:
  // Draws a polygon.
  static void DrawCar();
  static void DrawSegment();

  // Draws all the polygons in the scene.
  static void Draw();

  static void ResetCar(double width, double length, double max_steering_angle);
  static void ResetCarPosition();

  static void ResetSegment(int fx, int fy, int tx, int ty);
  static void DoubleLineWidth();

  static void TurnCarLeft();
  static void TurnCarRight();

  static void MoveForward();
  static void MoveBackward();

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
  static utils::ObjectHolder* GetObjectHolder();

  static void TransformDrawingPane();

  static double wi;
  static double hi;

private:

  static scoped_ptr<simulation::Car> car_;
  static scoped_ptr<geometry::RectangleObject> segment_;
  static utils::ObjectHolder objectHolder_;

  static double xTranslation, yTranslation, zTranslation;
  static int width_, height_;
  static double xRotate;
};

}  // namespace visualize

#endif  // SCENE_H