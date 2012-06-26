#include "geometry/geometry_utils.h"
#include "geometry/point.h"
#include "geometry/vector.h"
#include "handlers/event_handlers.h"
#include "simulation/car.h"
#include "simulation/car_description.h"
#include "simulation/car_manuever.h"
#include "simulation/car_manuever_handler.h"
#include "simulation/car_movement_handler.h"
#include "simulation/car_position.h"
#include "simulation/car_positions_graph.h"
#include "simulation/car_positions_graph_router.h"
#include "utils/benchmark.h"
#include "utils/boundary_line_holder.h"
#include "utils/car_positions_graph_builder.h"
#include "utils/intersection_handler.h"
#include "utils/object_holder.h"
#include "visualize/glut_utils.h"
#include "visualize/scene.h"

#include <cstdlib>
#include <fstream>
#include <glut.h>
#include <iostream>
#include <stdexcept>

#define USE_AI

using namespace std;

static const char* DEFAULT_SAVE_LOCATION = "../../resources/parking_serialized.txt";
static const char* DEFAULT_INPUT_LOCATION = "../../resources/input.in";

static const double MIN_X_COORDINATE = -250.0;
static const double MAX_X_COORDINATE = 250.0;
static const double MIN_Y_COORDINATE = -150.0;
static const double MAX_Y_COORDINATE = 150.0;

const bool USE_AI_TO_PARK = true;

simulation::Car* car;

void ReadInput(utils::ObjectHolder* object_holder) {
  ifstream in(DEFAULT_INPUT_LOCATION);
  if (!in) {
     throw std::runtime_error("Could not open the input file");
  }
  double width, length, max_steering_angle;
  in >> width >> length >> max_steering_angle;
  geometry::Point car_center, second_point;
  in >> car_center >> second_point;
  max_steering_angle = geometry::GeometryUtils::DegreesToRadians(max_steering_angle);

  geometry::Vector vector(car_center, second_point);
  car = new simulation::Car(simulation::CarDescription(
      width, length, max_steering_angle));
  simulation::CarPosition position; 
  position.SetCenter(car_center);
  position.SetDirection(vector);
  car->SetPosition(position);
  
  visualize::Scene::AddCar(*car);

  object_holder->ParseFromFile(DEFAULT_SAVE_LOCATION);
}

int main(int argc, char ** argv)
{
  utils::ObjectHolder object_holder;
  ReadInput(&object_holder);
  visualize::Scene::SetObjectHolder(&object_holder);
  
  utils::BoundaryLinesHolder boundary_lines_holder;
  utils::IntersectionHandler intersection_handler(
      MIN_X_COORDINATE, MAX_X_COORDINATE,
      MIN_Y_COORDINATE, MAX_Y_COORDINATE,
      &boundary_lines_holder);
  intersection_handler.Init(object_holder);
  simulation::CarMovementHandler movement_handler(
      &intersection_handler, car->GetDescription());
  visualize::Scene::SetCarMovementHandler(&movement_handler);
  
#ifdef USE_AI
  simulation::CarPositionsGraph graph(&movement_handler);
  cout << *car << endl;
  utils::RectangleObjectContainer car_objects;
  object_holder.GetObectsForLocation(
      car->GetPosition().GetCenter(), &car_objects);

  if (car_objects.empty()) {
    cerr << "The car should be located within a passable area.\n";
    return 0;
  }
  graph.AddPosition(car->GetPosition(), car_objects.front());

  utils::CarPositionsGraphBuilder builder(object_holder, intersection_handler);
  builder.CreateCarPositionsGraph(&graph);
  cout << "The graph is constructed now\n";
  simulation::CarPositionsGraphRouter router(&graph);
  vector<simulation::CarManuever> route = router.GetRoute(0);
  cerr << "Number of manuevers found" << route.size() << endl;
  for (unsigned i = 0; i < route.size(); ++i) {
    cerr << "Manuever [" << i << "]:\n";
    cerr << route[i] << endl;
  }

  simulation::CarManueverHandler manuever_handler(car->GetDescription(), route);
  visualize::Scene::SetCarManueverHandler(&manuever_handler);
#endif  
  
  utils::Benchmark::DumpBenchmarkingInfo();
  
  utils::InitializeHandlers();
  visualize::initGlut(argc, argv);

  // Run GLUT main loop.
  glutMainLoop();
  return 0;
}
