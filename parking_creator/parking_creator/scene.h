#ifndef SCENE_H
#define SCENE_H

#include "object_holder.h"

#include <vector>

namespace geometry {
class Point;
class Polygon;
class RectangleObject;
class DirectedRectangleObject;
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
  // Draws all the polygons in the scene.
  static void Draw();

  // Functions to translate the scene.
  static void TranslateLeft();
  static void TranslateRight();
  static void TranslateUp();
  static void TranslateDown();
  static void TranslateIn();
  static void TranslateOut();

  static void Resize(int width, int height);
  static int Width();
  static int Height();
  static utils::ObjectHolder* GetObjectHolder();

  static void TransformDrawingPane();

 private:
  static void DrawCar();
  static void DrawObjects();
  static void DrawSelected(const geometry::Polygon& polygon);
  static void DrawObjectsFromContainer(
      const utils::RectangleObjectContainer& container);
  static void DrawDirectionalTips(
      const geometry::DirectedRectangleObject& directed_object);

 private:
  static utils::ObjectHolder objectHolder_;

  static double xTranslation, yTranslation, zTranslation;
  static int width_, height_;
  static double xRotate;
};

}  // namespace visualize

#endif  // SCENE_H