#include "visualize/scene.h"

#include "geometry/directed_rectangle_object.h"
#include "geometry/polygon.h"
#include "geometry/rectangle_object.h"
#include "geometry/vector.h"
#include "utils/object_holder.h"

#include <cassert>
#include <glut.h>
#include <vector>

namespace visualize {

static const double METERS_PER_STEP = 0.2;
static const double TRANSLATION_UNIT = 0.2;

static const int DEFAULT_WIDTH = 800;
static const int DEFAULT_HEIGHT = 600;

static const double SCALE_FACTOR = 10;

// static declarations
utils::ObjectHolder Scene::objectHolder_;

int Scene::width_ = DEFAULT_WIDTH;
int Scene::height_ = DEFAULT_HEIGHT;

double Scene::xTranslation = 0.0; 
double Scene::yTranslation = 0.0;
double Scene::zTranslation = 0.0;
double Scene::xRotate = 0.0;

void DrawPolygon(const geometry::Polygon& polygon) {
  glBegin(GL_POLYGON);
  for (unsigned vindex = 0; vindex < polygon.NumberOfVertices(); ++vindex) {
    const geometry::Point& vertex = polygon.GetPoint(vindex);
    glVertex2f(vertex.x, vertex.y);
  }
  glEnd();   // GL_POLYGON
}

// static 
void Scene::TranslateLeft() {
  xTranslation -= TRANSLATION_UNIT;
}

// static 
void Scene::TranslateRight() {
  xTranslation += TRANSLATION_UNIT;
}

// static 
void Scene::TranslateUp() {
  yTranslation += TRANSLATION_UNIT;
}

// static 
void Scene::TranslateDown() {
  yTranslation -= TRANSLATION_UNIT;
}

// static 
void Scene::TranslateIn() {
  zTranslation -= TRANSLATION_UNIT;
}

// static
void Scene::TranslateOut() {
  zTranslation += TRANSLATION_UNIT;
}

// static 
void Scene::Resize(int width, int height) {
  width_ = width;
  height_ = height;
}

// static
int Scene::Width() {
  return width_;
}

// static
int Scene::Height() {
  return height_;
}

// static 
utils::ObjectHolder* Scene::GetObjectHolder() {
  return &objectHolder_;
}

// static 
void Scene::Draw() {
  DrawObjects();
}

// static 
void Scene::TransformDrawingPane() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho((-width_ * 0.5) / SCALE_FACTOR - xTranslation, 
          (width_ * 0.5) / SCALE_FACTOR - xTranslation, 
          (-height_ * 0.5) / SCALE_FACTOR - yTranslation, 
          (height_ * 0.5) / SCALE_FACTOR - yTranslation, 0, 1);
  glMatrixMode(GL_MODELVIEW);
}

// static
void Scene::DrawObjects() {
  glColor4f(1.0, 0.8, 0.6, 0.5);
  DrawObjectsFromContainer(objectHolder_.GetRoadSegments());
  
  glColor4f(0.5, 0.5, 0.5, 0.5);
  DrawObjectsFromContainer(objectHolder_.GetParkingLots());
  
  glColor4f(0.0, 0.0, 0.0, 0.5);
  DrawObjectsFromContainer(objectHolder_.GetObstacles());

  if (objectHolder_.HasSelected()) {
    DrawSelected(objectHolder_.GetSelected()->GetBounds());
  }
}

// static
void Scene::DrawSelected(const geometry::Polygon& polygon) {
  const double SELECTION_LENGTH = 0.4;
  glLineStipple(0.1, 0xf88f);
  glEnable(GL_LINE_STIPPLE);
  glColor3f(0.0, 0.0, 0.8);

  glBegin(GL_LINE_STRIP);
  for (unsigned index = 0; index <= polygon.NumberOfVertices(); ++index) {
    const geometry::Point& prev = polygon.GetPointCyclic(index - 1);
    const geometry::Point& cur = polygon.GetPointCyclic(index);
    const geometry::Point& next = polygon.GetPointCyclic(index + 1);
    geometry::Point point = cur;
    point += (geometry::Vector(prev, cur).Unit() * SELECTION_LENGTH);
    point += (geometry::Vector(next, cur).Unit() * SELECTION_LENGTH);
    glVertex2f(point.x, point.y);
  }
  glEnd();
  glDisable(GL_LINE_STIPPLE);
}

// static
void Scene::DrawObjectsFromContainer(
    const utils::RectangleObjectContainer& container) {
  double color[4], new_color[4];
  glGetDoublev(GL_CURRENT_COLOR, color);
  for (unsigned index = 0; index < container.size(); ++index) {
    DrawPolygon(container[index]->GetBounds());  
  }
  for (int i = 0; i < 3; ++i) {
    new_color[i] = 1.0 - color[i];
  }
  new_color[3] = color[3];
  glColor4dv(new_color);
  for (unsigned index = 0; index < container.size(); ++index) {
    if (!container[index]->IsDirected()) {
      continue;
    }
    geometry::DirectedRectangleObject* directed_object = 
        dynamic_cast<geometry::DirectedRectangleObject*>(container[index]);
    DrawDirectionalTips(*directed_object);
  }
  glColor4dv(color);
}

void DrawArrow(const geometry::Point& from, const geometry::Point& to,
    double width) {
  geometry::Vector vector(from, to);
  geometry::Vector ortho = vector.GetOrthogonal().Unit();
  for (int sign = -1; sign <= 1; sign += 2) {
    geometry::Point vertex = from + ortho * width * 0.5 * sign;
    geometry::Vector side(vertex, to);
    geometry::Point end = vertex + side * 0.33;
    glBegin(GL_LINE_STRIP);
      glVertex2d(end.x, end.y);  
      glVertex2d(to.x, to.y);
    glEnd();  
  }
} 

// static
void Scene::DrawDirectionalTips(
    const geometry::DirectedRectangleObject& directed_object) {
  const geometry::Point& from = directed_object.GetFrom();
  const geometry::Point& to = directed_object.GetTo();
  double width = directed_object.GetWidth();

  geometry::Vector vector(from, to);
  geometry::Vector ortho = vector.GetOrthogonal().Unit();
  geometry::Point middle = from + vector * 0.5;

  glLineStipple(0.1, 0xf0f0);
  glEnable(GL_LINE_STIPPLE);
  
  glBegin(GL_LINE_STRIP);
    glVertex2d(from.x, from.y);
    glVertex2d(to.x, to.y);
  glEnd();
  glDisable(GL_LINE_STIPPLE);


  DrawArrow(middle + vector * 0.165, middle + vector * 0.33, width);
  if (!directed_object.IsOneWay()) {
    DrawArrow(middle - vector * 0.165, middle - vector * 0.33, width);
  }
}

}  // namespace visualize
