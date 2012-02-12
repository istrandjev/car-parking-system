#include "event_handlers.h"
#include "geometry_utils.h"
#include "glut_utils.h"
#include "scene.h"

#include <AntTweakBar.h>
#include <cstdlib>
#include <fstream>
#include <glut.h>
#include <iostream>

using namespace std;

int main(int argc, char ** argv)
{
  // Initialize GLUT.
  utils::InitializeHandlers();
  visualize::initGlut(argc, argv);
  
  // Run GLUT main loop.
  glutMainLoop();
  
  TwTerminate();
  return 0;
}