#include "handlers/event_handlers.h"
#include "geometry/geometry_utils.h"
#include "utils/object_handler.h"
#include "utils/object_holder.h"
#include "visualize/glut_utils.h"
#include "visualize/scene.h"

#include <AntTweakBar.h>
#include <glut.h>

#include <cstdlib>
#include <fstream>
#include <iostream>

int main(int argc, char ** argv) {
  utils::ObjectHolder object_holder;
  utils::ObjectHandler object_handler(&object_holder);
  visualize::Scene::SetObjectHandler(&object_handler);

  // Initialize GLUT.
  utils::InitializeHandlers();
  visualize::initGlut(argc, argv);

  // Run GLUT main loop.
  glutMainLoop();

  TwTerminate();
  return 0;
}
