#include "visualize/glut_utils.h"

#include "handlers/event_handlers.h"
#include "visualize/scene.h"
#include "utils/delay.h"

#include <cstdlib>
#include <glut.h>
#include <iostream>
#include <vector>

namespace visualize {

void handleResize(int w, int h);
void display();
void idle();

void initGlut(int argc, char** argv) {
	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(Scene::Width(), Scene::Height());
	glutCreateWindow("Car simulation");
	
  Scene::TransformDrawingPane();

	// Background color.
	glClearColor(1.0, 1.0, 1.0, 0);
	glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Initialize GLUT callbacks.
	glutDisplayFunc(display);
	glutReshapeFunc(handleResize);
  glutKeyboardFunc(utils::KeyPressed);
  glutKeyboardUpFunc(utils::KeyReleased);
  glutSpecialUpFunc(utils::SpecialKeyReleased);
  glutSpecialFunc(utils::SpecialKeyPressed);
  glutMouseFunc(utils::MousePressFunc);
  glutMotionFunc(utils::MouseMoveFunc);
  glutIdleFunc(idle);
}

//Called when the window is resized
void handleResize(int w, int h) 
{
	//Tell OpenGL how to convert from coordinates to pixel values
	glViewport(0, 0, w, h);

  Scene::Resize(w, h);
  Scene::TransformDrawingPane();

	glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective

	//Set the camera perspective
	glLoadIdentity(); //Reset the camera
	gluPerspective(45.0,				  //The camera angle
		(double)w / (double)h, //The width-to-height ratio
		1.0,				   //The near z clipping coordinate
		200.0);				//The far z clipping coordinate
}		


void display()
{
  utils::HandleKeyboardEvents();
	
  //Clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
  Scene::TransformDrawingPane();
  Scene::Draw();

	glutSwapBuffers();
}

void idle() {
  glutPostRedisplay();
  Scene::Tick();
  delay(0.002);
}

}  // namespace visualize
