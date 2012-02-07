#include "glut_utils.h"

#include "delay.h"
#include "scene.h"
#include "user_input_handler.h"

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
void mousePressFunc(int button, int state, int x, int y); 
void mouseMoveFunc(int x, int y); 
void idle();

void HandleKeyboardEvents();

void TW_CALL ResetSize(void * /*clientData*/)
{ 
  Scene::wi = Scene::hi =  5.0;
}

void handleMouseClick(double x, double y) {
  Scene::DoubleLineWidth();
}

void handleMouseDrag(double fx, double fy, double tx, double ty) {
  Scene::ResetSegment(fx, fy, tx, ty);
}

void initGlut(int argc, char** argv) {

	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glEnable(GL_DEPTH);
	glutInitWindowSize(Scene::Width(), Scene::Height());
	glutCreateWindow("Car simulation");
	utils::UserInputHandler::SetLeftMouseClickHandler(handleMouseClick);
	utils::UserInputHandler::SetLeftMouseDragHandler(handleMouseDrag);

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
	glutMouseFunc(mousePressFunc);
  glutMotionFunc(mouseMoveFunc);
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
    utils::UserInputHandler::PressRegularKey(c, x, y);
  }
}

void keyReleased(unsigned char c, int x, int y) {
  if (!TwEventKeyboardGLUT(c, x , y)) {
    utils::UserInputHandler::ReleaseRegularKey(c);
  }
}


void specialKeyPressed(int c, int x, int y) {
  if (!TwEventSpecialGLUT(c, x, y)) {
    utils::UserInputHandler::PressSpecialKey(c, x, y);
  }
}

void specialKeyReleased(int c, int x, int y) {
  if (!TwEventSpecialGLUT(c, x, y)) {
    utils::UserInputHandler::ReleaseSpecialKey(c);
  }
}

void mousePressFunc(int button, int state, int x, int y) {
  if (!TwEventMouseButtonGLUT(button, state, x, y) ) {
	  if (state == GLUT_DOWN) {
		  if (button == GLUT_LEFT_BUTTON) {
        utils::UserInputHandler::PressLeftMouse(x, y);
      } else {
        utils::UserInputHandler::PressRightMouse(x, y);
      }
	  } else if (state = GLUT_UP) {
      if (button == GLUT_LEFT_BUTTON) {
        utils::UserInputHandler::ReleaseLeftMouse(x, y);
      } else {
        utils::UserInputHandler::ReleaseRightMouse(x, y);
      }
    }
  }
}

void mouseMoveFunc(int x, int y) {
  if (!TwEventMouseMotionGLUT(x, y) ) {
    utils::UserInputHandler::MoveMouse(x, y);
  }
}

void HandleKeyboardEvents() {
  if (utils::UserInputHandler::IsSpecialKeyPressed(GLUT_KEY_LEFT)) {
    Scene::TurnCarLeft();
  }
  if (utils::UserInputHandler::IsSpecialKeyPressed(GLUT_KEY_RIGHT)) {
    Scene::TurnCarRight();
  }
  if (utils::UserInputHandler::IsSpecialKeyPressed(GLUT_KEY_UP)) {
    Scene::MoveForward();
  }
  if (utils::UserInputHandler::IsSpecialKeyPressed(GLUT_KEY_DOWN)) {
    Scene::MoveBackward();
  }
  if (utils::UserInputHandler::IsRegularKeyPressed('a')) {
    Scene::TranslateLeft();
  }
  if (utils::UserInputHandler::IsRegularKeyPressed('d')) {
    Scene::TranslateRight();
  }
  if (utils::UserInputHandler::IsRegularKeyPressed('w')) {
    Scene::TranslateUp();
  }
  if (utils::UserInputHandler::IsRegularKeyPressed('s')) {
    Scene::TranslateDown();
  }
  if (utils::UserInputHandler::IsRegularKeyPressed(' ')) {
    Scene::ResetCarPosition();
  }
}
void idle() {
  glutPostRedisplay();   
  delay(0.002);
}

}  // namespace visualize