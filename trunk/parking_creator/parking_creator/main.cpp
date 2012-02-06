#include "geometry_utils.h"
#include "glut_utils.h"
#include "scene.h"

#include <AntTweakBar.h>
#include <cstdlib>
#include <fstream>
#include <glut.h>
#include <iostream>

using namespace std;

void ReadInput() {
  ifstream in("input.in");
  if (!in) {
    std::exception("Could not open the input file");
  }
  double width, length, max_steering_angle;
  in >> width >> length >> max_steering_angle;
  max_steering_angle = geometry::GeometryUtils
      ::DegreesToRadians(max_steering_angle);
  visualize::Scene::ResetCar(width, length, max_steering_angle);
}

int main(int argc, char ** argv)
{
  ReadInput();

  // Initialize GLUT.
  visualize::initGlut(argc, argv);

  // Run GLUT main loop.
  glutMainLoop();

  return 0;
}