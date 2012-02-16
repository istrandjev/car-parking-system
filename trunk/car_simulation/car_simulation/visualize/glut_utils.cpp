#include "visualize/glut_utils.h"

#include "visualize/scene.h"
#include "utils/delay.h"

#include <cstdlib>
#include <glut.h>
#include <iostream>
#include <vector>

namespace visualize {

void handleResize(int w, int h);
void display();
void keyPressed(unsigned char c, int x, int y);
void specialKeyPressed(int c, int x, int y);
void mouseFunc(int button, int state, int x, int y); 
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
	glutKeyboardFunc(keyPressed);
	glutSpecialFunc(specialKeyPressed);
	glutMouseFunc(mouseFunc);
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
	//Clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
  Scene::TransformDrawingPane();
  Scene::Draw();

	glutSwapBuffers();
}

void keyPressed(unsigned char c, int x, int y) {
	switch(c) {
  case 'a': Scene::TranslateLeft(); break;
  case 'd': Scene::TranslateRight(); break;
  case 'w': Scene::TranslateUp(); break;
  case 's': Scene::TranslateDown(); break;
  case ' ': Scene::ResetCar(0); break;
	}

	display();
}

void specialKeyPressed(int c, int x, int y) {
	switch(c) {
  case GLUT_KEY_LEFT: Scene::TurnCarLeft(0); break;
  case GLUT_KEY_RIGHT: Scene::TurnCarRight(0); break;
  case GLUT_KEY_UP: Scene::Move(true); break;
  case GLUT_KEY_DOWN: Scene::Move(false); break;
	}

	display();
}

void mouseFunc(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {
			Scene::RotateXPositive();
		} else if (button == GLUT_RIGHT_BUTTON) {
			Scene::RotateXNegative();
		}
	}

	display();
}

void idle() {
  glutPostRedisplay();   
  delay(0.002);
}

}  // namespace visualize