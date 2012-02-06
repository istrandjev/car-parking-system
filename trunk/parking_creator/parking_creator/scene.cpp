#include "scene.h"

#include "polygon.h"

#include <glut.h>

#include <vector>

namespace visualize {

static const double METERS_PER_STEP = 0.2;
static const double TRANSLATION_UNIT = 0.2;
static const double ROTATION_UNIT = 0.2;

static const int DEFAULT_WIDTH = 800;
static const int DEFAULT_HEIGHT = 600;

static const double SCALE_FACTOR = 10;

// static declarations
std::vector<simulation::Car> Scene::cars_;
int Scene::width_ = DEFAULT_WIDTH;
int Scene::height_ = DEFAULT_HEIGHT;
double Scene::wi = 0.0;
double Scene::hi = 0.0;

double Scene::xTranslation = 0.0; 
double Scene::yTranslation = 0.0;
double Scene::zTranslation = 0.0;
double Scene::xRotate = 0.0;

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
void Scene::RotateXPositive() {
  xRotate += ROTATION_UNIT;
}

// static 
void Scene::RotateXNegative() {
  xRotate -= ROTATION_UNIT;
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

void Scene::ResetCar(unsigned index) {
  cars_[index].Reset();
}

// static 
void Scene::AddCar(double width, double length, double max_steering_angle) {
  cars_.push_back(simulation::Car(width, length, max_steering_angle));
}

// static 
void Scene::Draw() {
  for (unsigned index = 0; index < cars_.size(); ++index) {
    DrawCar(index);
  }
  glColor3f(0.5, 0.5, 0.0);
  glBegin(GL_POLYGON); 
    glVertex2f(-wi, -hi );
    glVertex2f(-wi, hi );
    glVertex2f(wi, hi );
    glVertex2f(wi, -hi );
  glEnd();
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
void Scene::DrawCar(unsigned index) {
  const simulation::Car& car = cars_[index];
  geometry::Polygon bounds = car.GetBounds();
  glColor3f(0.5, 0.5, 0.5);
  glBegin(GL_POLYGON);
    for (unsigned vertex_index = 0; 
        vertex_index < bounds.NumberOfVertices(); ++vertex_index) {
      const geometry::Point& vertex = bounds.GetPoint(vertex_index);
      glVertex2f(vertex.x, vertex.y);
    }
  glEnd();   // GL_POLYGON

  std::vector<geometry::Polygon> wheels = car.GetWheels();
  glColor3f(0, 0, 0);

  for (unsigned wheel_index = 0; wheel_index < wheels.size(); 
      ++wheel_index) {
    const geometry::Polygon& wheel = wheels[wheel_index];
    glBegin(GL_POLYGON);
      for (unsigned vertex_index = 0; 
          vertex_index < bounds.NumberOfVertices(); ++vertex_index) {
        const geometry::Point& vertex = wheel.GetPoint(vertex_index);
        glVertex2f(vertex.x, vertex.y);
      }
    glEnd();   // GL_POLYGON
  }
}

// static
void Scene::TurnCarLeft(unsigned index) {
  cars_[index].TurnLeft();
}

// static
void Scene::TurnCarRight(unsigned index) {
  cars_[index].TurnRight();
}

// static 
void Scene::Move(bool forward) {
  for (unsigned index = 0; index < cars_.size(); ++index) {
    if (forward) {
      cars_[index].Move(METERS_PER_STEP);
    } else {
      cars_[index].Move(-METERS_PER_STEP);    
    }
  }
}

}  // namespace visualize
