#include "tw_handler.h"

#include "rectangle_object.h"
#include "scene.h"

#include <AntTweakBar.h>
#include <glut.h>

namespace visualize {

// static declarations
TwBar* TwHandler::bar = NULL;

void TW_CALL ResetSize(void * /*clientData*/);
void TW_CALL SetLineWidthCallback(const void *value, void *clientData);
void TW_CALL GetLineWidthCallback(void* value, void* clientData);

// static
void TwHandler::Init() {
  TwGLUTModifiersFunc(glutGetModifiers);

  TwInit(TW_OPENGL, NULL);
  TwWindowSize(Scene::Width(), Scene::Height());
  bar = TwNewBar("Line options");
  TwAddVarCB(bar, "width", TW_TYPE_DOUBLE, SetLineWidthCallback, GetLineWidthCallback, NULL, "");
  TwAddButton(bar, "Run", ResetSize, NULL, " label='Reset Size' ");
  TwDefine("'Line options' size='200 160' ");
}


// static
void TwHandler::SetSize(int width, int height) {
  TwWindowSize(Scene::Width(), Scene::Height());
}

// static
void TwHandler::Draw() {
  TwDraw();
}

void TW_CALL ResetSize(void * /*clientData*/)
{ 
  // Scene::wi = Scene::hi =  5.0;
}

void TW_CALL SetLineWidthCallback(const void *value, void *clientData)
{ 
    utils::ObjectHolder* obj_holder = Scene::GetObjectHolder();
    if (obj_holder->HasSelected()) {
      geometry::RectangleObject* object = obj_holder->GetSelected();
      object->SetWidth(*((double*)value));
    }
}

void TW_CALL GetLineWidthCallback(void* value, void* clientData) {
  utils::ObjectHolder* obj_holder = Scene::GetObjectHolder();
  if (obj_holder->HasSelected()) {
    geometry::RectangleObject* object = obj_holder->GetSelected();
    *static_cast<double*>(value) = object->GetWidth();
  } else {
    value = NULL;
  }
}

}  // namespace visualize