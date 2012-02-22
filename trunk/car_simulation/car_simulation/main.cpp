#include "geometry/geometry_utils.h"
#include "geometry/point.h"
#include "geometry/vector.h"
#include "handlers/event_handlers.h"
#include "visualize/glut_utils.h"
#include "visualize/scene.h"

#include <cstdlib>
#include <fstream>
#include <glut.h>
#include <iostream>

using namespace std;

static const char* DEFAULT_SAVE_LOCATION = "../../resources/parking_serialized.txt";
static const char* DEFAULT_INPUT_LOCATION = "../../resources/input.in";

void ReadInput() {
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

  visualize::Scene::GetObjectHolder()->ParseFromFile(DEFAULT_SAVE_LOCATION);
}

int main(int argc, char ** argv)
{
  ReadInput();

  utils::InitializeHandlers();
  visualize::initGlut(argc, argv);

  // Run GLUT main loop.
  glutMainLoop();

  return 0;
}