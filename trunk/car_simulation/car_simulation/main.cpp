#include "geometry/geometry_utils.h"
#include "geometry/point.h"
#include "geometry/vector.h"
#include "handlers/event_handlers.h"
#include "utils/boundary_line_holder.h"
#include "utils/intersection_handler.h"
#include "utils/object_holder.h"
#include "visualize/glut_utils.h"
#include "visualize/scene.h"

#include <cstdlib>
#include <fstream>
#include <glut.h>
#include <iostream>

using namespace std;

static const char* DEFAULT_SAVE_LOCATION = "../../resources/parking_serialized.txt";
static const char* DEFAULT_INPUT_LOCATION = "../../resources/input.in";

static const double MIN_X_COORDINATE = -250.0;
static const double MAX_X_COORDINATE = 250.0;
static const double MIN_Y_COORDINATE = -150.0;
static const double MAX_Y_COORDINATE = 150.0;

void ReadInput(utils::ObjectHolder* object_holder) {
  ifstream in(DEFAULT_INPUT_LOCATION);
  if (!in) {
    std::exception("Could not open the input file");
  }
  double width, length, max_steering_angle;
  in >> width >> length >> max_steering_angle;
  geometry::Point car_center, second_point;
  in >> car_center >> second_point;
  
  max_steering_angle = geometry::GeometryUtils
      ::DegreesToRadians(max_steering_angle);
  visualize::Scene::AddCar(width, length, max_steering_angle, car_center,
      second_point);

  object_holder->ParseFromFile(DEFAULT_SAVE_LOCATION);
}

int main(int argc, char ** argv)
{
  utils::ObjectHolder object_holder;
  visualize::Scene::SetObjectHolder(&object_holder);
  ReadInput(&object_holder);
  utils::BoundaryLinesHolder boundary_lines_holder;
  utils::IntersectionHandler intersection_handler(
      MIN_X_COORDINATE, MAX_X_COORDINATE,
      MIN_Y_COORDINATE, MAX_Y_COORDINATE,
      &boundary_lines_holder);
  intersection_handler.Init(object_holder);
  visualize::Scene::SetIntersectionHandler(&intersection_handler);

  utils::InitializeHandlers();
  visualize::initGlut(argc, argv);
  
  // Run GLUT main loop.
  glutMainLoop();

  return 0;
}