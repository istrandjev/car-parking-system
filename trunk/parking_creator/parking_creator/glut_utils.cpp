#include "glut_utils.h"

#include "delay.h"
#include "scene.h"

#include <AntTweakBar.h>
#include <cstdlib>
#include <glut.h>
#include <iostream>
#include <vector>

namespace visualize {

TwBar *myBar;
double myVar;

bool regular_keys[256];
bool special_keys[256];

void handleResize(int w, int h);
void display();
void keyPressed(unsigned char c, int x, int y);
void keyReleased(unsigned char c, int x, int y);
void specialKeyPressed(int c, int x, int y);
void specialKeyReleased(int c, int x, int y);
void mouseFunc(int button, int state, int x, int y); 
void idle();

void HandleKeyboardEvents();

void TW_CALL ResetSize(void * /*clientData*/)
{ 
  Scene::wi = Scene::hi =  5.0;
}

void initGlut(int argc, char** argv) {
  memset(regular_keys, 0, sizeof(regular_keys));
  memset(special_keys, 0, sizeof(special_keys));

	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(Scene::Width(), Scene::Height());
	glutCreateWindow("Car simulation");
	
  Scene::TransformDrawingPane();

	// Background color.
	glClearColor(1.0, 1.0, 1.0, 0);
	glDisable(GL_DEPTH_TEST);

	// Initialize GLUT callbacks.
	glutDisplayFunc(display);
	glutReshapeFunc(handleResize);
	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyReleased);
	glutSpecialUpFunc(specialKeyReleased);
	glutSpecialFunc(specialKeyPressed);
	glutMouseFunc(mouseFunc);
  glutIdleFunc(idle);

  TwGLUTModifiersFunc(glutGetModifiers);

  TwInit(TW_OPENGL, NULL);
  TwWindowSize(Scene::Width(), Scene::Height());
  myBar = TwNewBar("Bira_ili_boza");
	TwAddVarRW(myBar, "width", TW_TYPE_DOUBLE, &Scene::wi, "");
	TwAddVarRW(myBar, "height", TW_TYPE_DOUBLE, &Scene::hi, "");
  TwAddButton(myBar, "Run", ResetSize, NULL, " label='Reset Size' ");
  TwDefine(" Bira_ili_boza size='160 60' ");
}

//Called when the window is resized
void handleResize(int w, int h) 
{
	//Tell OpenGL how to convert from coordinates to pixel values
	glViewport(0, 0, w, h);

  Scene::Resize(w, h);
  Scene::TransformDrawingPane();
  TwWindowSize(Scene::Width(), Scene::Height());

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
  HandleKeyboardEvents();

	//Clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
  Scene::TransformDrawingPane();
  Scene::Draw();

  TwDraw();
  glutSwapBuffers();
}

void keyPressed(unsigned char c, int x, int y) {
  if (!TwEventKeyboardGLUT(c, x , y)) {
    regular_keys[c] = true;
	  /*switch(c) {
    case 'a': Scene::TranslateLeft(); break;
    case 'd': Scene::TranslateRight(); break;
    case 'w': Scene::TranslateUp(); break;
    case 's': Scene::TranslateDown(); break;
    case ' ': Scene::ResetCar(0); break;
	  }*/
  }
}

void keyReleased(unsigned char c, int x, int y) {
  if (!TwEventKeyboardGLUT(c, x , y)) {
    regular_keys[c] = false;
	  /*switch(c) {
    case 'a': Scene::TranslateLeft(); break;
    case 'd': Scene::TranslateRight(); break;
    case 'w': Scene::TranslateUp(); break;
    case 's': Scene::TranslateDown(); break;
    case ' ': Scene::ResetCar(0); break;
	  }*/
  }
}


void specialKeyPressed(int c, int x, int y) {
  if (!TwEventSpecialGLUT(c, x, y)) {
    special_keys[c] = true;
	  /*switch(c) {
    case GLUT_KEY_LEFT: Scene::TurnCarLeft(0); break;
    case GLUT_KEY_RIGHT: Scene::TurnCarRight(0); break;
    case GLUT_KEY_UP: Scene::Move(true); break;
    case GLUT_KEY_DOWN: Scene::Move(false); break;
	  }*/
  }
}

void specialKeyReleased(int c, int x, int y) {
  if (!TwEventSpecialGLUT(c, x, y)) {
    special_keys[c] = false;
	  /*switch(c) {
    case GLUT_KEY_LEFT: Scene::TurnCarLeft(0); break;
    case GLUT_KEY_RIGHT: Scene::TurnCarRight(0); break;
    case GLUT_KEY_UP: Scene::Move(true); break;
    case GLUT_KEY_DOWN: Scene::Move(false); break;
	  }*/
  }
}

void mouseFunc(int button, int state, int x, int y) {
  if (!TwEventMouseButtonGLUT(button, state, x, y) ) {
	  if (state == GLUT_DOWN) {
		  if (button == GLUT_LEFT_BUTTON) {
			  Scene::RotateXPositive();
		  } else if (button == GLUT_RIGHT_BUTTON) {
			  Scene::RotateXNegative();
		  }
	  }
  }
}

void HandleKeyboardEvents() {
  if (special_keys[GLUT_KEY_LEFT]) {
    Scene::TurnCarLeft(0);
  }
  if (special_keys[GLUT_KEY_RIGHT]) {
    Scene::TurnCarRight(0);
  }
  if (special_keys[GLUT_KEY_UP]) {
    Scene::Move(true);
  }
  if (special_keys[GLUT_KEY_DOWN]) {
    Scene::Move(false);
  }
  if (regular_keys['a']) {
    Scene::TranslateLeft();
  }
  if (regular_keys['d']) {
    Scene::TranslateRight();
  }
  if (regular_keys['w']) {
    Scene::TranslateUp();
  }
  if (regular_keys['s']) {
    Scene::TranslateDown();
  }
  if (regular_keys[' ']) {
    Scene::ResetCar(0);
  }
}
void idle() {
  glutPostRedisplay();   
  delay(0.002);
}

}  // namespace visualize