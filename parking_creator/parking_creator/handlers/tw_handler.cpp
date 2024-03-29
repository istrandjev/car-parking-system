#include "handlers/tw_handler.h"

#include "geometry/directed_rectangle_object.h"
#include "geometry/rectangle_object.h"
#include "visualize/scene.h"
#include "utils/object_handler.h"
#include "utils/object_holder.h"

#include <AntTweakBar.h>
#include <glut.h>

namespace visualize {

// static declarations
TwBar* TwHandler::bar = NULL;

static const char* DEFAULT_SAVE_LOCATION = "../../resources/parking_serialized.txt";

void TW_CALL DirectionTypeChange(void * clientData);
void TW_CALL ReverseDirection(void * clientData);
void TW_CALL CreateSibling(void * clientData);
void TW_CALL SaveToFile(void * clientData);
void TW_CALL LoadFromFile(void * clientData);
void TW_CALL SetLineWidthCallback(const void *value, void *clientData);
void TW_CALL GetLineWidthCallback(void* value, void* clientData);

void TW_CALL SetObjectTypeCallback(const void *value, void *clientData);
void TW_CALL SetObjectTypeCallback(void* value, void* clientData);

// static
void TwHandler::Init() {
  TwGLUTModifiersFunc(glutGetModifiers);

  TwInit(TW_OPENGL, NULL);
  TwWindowSize(Scene::Width(), Scene::Height());
  bar = TwNewBar("Line options");
  TwAddVarCB(bar, "width", TW_TYPE_DOUBLE, SetLineWidthCallback,
             GetLineWidthCallback, NULL, "");
  TwAddButton(bar, "Direction", DirectionTypeChange, NULL,
              " label='Make one way' ");
  TwAddButton(bar, "CreateSibling", CreateSibling, NULL,
              " label='Create sibling' ");
  TwAddButton(bar, "ReverseDirection", ReverseDirection, NULL,
              " label='Reverse direction' ");

  TwDefine("'Line options' size='200 160' ");

  AddObjectTypeEnum();

  TwAddButton(bar, "SaveToFile", SaveToFile, NULL, " label='Save to file' ");
  TwAddButton(bar, "LoadFromFile", LoadFromFile, NULL,
              " label='Load from file' ");
}


// static
void TwHandler::SetSize(int width, int height) {
  TwWindowSize(Scene::Width(), Scene::Height());
}

// static
void TwHandler::ModifyAccordingToState() {
  utils::ObjectHandler* obj_handler = Scene::GetObjectHandler();
  if (!obj_handler->HasSelected()) {
    TwDefine(" 'Line options'/Direction  visible=false ");
    TwDefine(" 'Line options'/ReverseDirection  visible=false ");
    TwDefine(" 'Line options'/CreateSibling  visible=false ");
    return;
  }

  geometry::RectangleObject* object = NULL;
  if (obj_handler->HasSelected()) {
    object = obj_handler->GetSelected();
  }
  if (object == NULL || object->IsDirected() == false) {
    TwDefine(" 'Line options'/Direction  visible=false ");
    TwDefine(" 'Line options'/ReverseDirection  visible=false ");
  } else {
    geometry::DirectedRectangleObject* directed =
      dynamic_cast<geometry::DirectedRectangleObject*>(object);
    TwDefine(" 'Line options'/Direction  visible=true ");
    if (directed->IsOneWay()) {
      TwDefine(" 'Line options'/Direction  label='Make two directional' ");
      TwDefine(" 'Line options'/ReverseDirection  visible=true ");
      TwDefine(" 'Line options'/CreateSibling  visible=true ");
    } else  {
      TwDefine(" 'Line options'/Direction  label='Make one way' ");
      TwDefine(" 'Line options'/ReverseDirection  visible=false ");
      TwDefine(" 'Line options'/CreateSibling  visible=false ");
    }
  }
}

// static
void TwHandler::Draw() {
  TwDraw();
}

// static
void TwHandler::AddObjectTypeEnum() {
  TwEnumVal object_type_names[] =
      { {utils::ObjectHandler::ROAD_SEGMENT, "Road segment"},
        {utils::ObjectHandler::PARKING_LOT, "Parking lot"},
        {utils::ObjectHandler::OBSTACLE, "Obstacle"} };
  TwType objectType;
  objectType = TwDefineEnum("ObjectType", object_type_names, 3);

  utils::ObjectHandler* obj_handler = Scene::GetObjectHandler();
  TwAddVarRW(bar, "Object type", objectType, &obj_handler->currentType_, NULL);
}

void TW_CALL DirectionTypeChange(void * /*clientData*/) {
    utils::ObjectHandler* obj_handler = Scene::GetObjectHandler();
    if (obj_handler->HasSelected()) {
      geometry::RectangleObject* object = obj_handler->GetSelected();
      if (object->IsDirected()) {
        geometry::DirectedRectangleObject* directed =
            dynamic_cast<geometry::DirectedRectangleObject*>(object);
        directed->SwapOneWayFlag();
      }
    }
}

void TW_CALL ReverseDirection(void * /*clientData*/) {
    utils::ObjectHandler* obj_handler = Scene::GetObjectHandler();
    if (obj_handler->HasSelected()) {
      geometry::RectangleObject* object = obj_handler->GetSelected();
      if (object->IsDirected()) {
        geometry::DirectedRectangleObject* directed =
            dynamic_cast<geometry::DirectedRectangleObject*>(object);
        if (directed->IsOneWay()) {
          directed->ReverseDirection();
        }
      }
    }
}

void TW_CALL CreateSibling(void * /*clientData*/) {
    utils::ObjectHandler* obj_handler = Scene::GetObjectHandler();
    if (obj_handler->HasSelected()) {
      geometry::RectangleObject* object = obj_handler->GetSelected();
      if (!object->IsDirected()) {
        return;
      }
      geometry::DirectedRectangleObject* directed =
          dynamic_cast<geometry::DirectedRectangleObject*>(object);
      if (!directed->IsOneWay()) {
        return;
      }
      obj_handler->AddSibling();
    }
}

void TW_CALL SaveToFile(void * /*clientData*/) {
  utils::ObjectHolder* obj_holder = 
      Scene::GetObjectHandler()->GetObjectHolder();
  obj_holder->DumpToFile(DEFAULT_SAVE_LOCATION);
}

void TW_CALL LoadFromFile(void * /*clientData*/) {
  utils::ObjectHolder* obj_holder = 
      Scene::GetObjectHandler()->GetObjectHolder();
  obj_holder->ParseFromFile(DEFAULT_SAVE_LOCATION);
}

void TW_CALL SetLineWidthCallback(const void *value, void *clientData) {
    utils::ObjectHandler* obj_handler = Scene::GetObjectHandler();
    if (obj_handler->HasSelected()) {
      geometry::RectangleObject* object = obj_handler->GetSelected();
      object->SetWidth(*static_cast<const double*>(value));
    }
}

void TW_CALL GetLineWidthCallback(void* value, void* clientData) {
  utils::ObjectHandler* obj_handler = Scene::GetObjectHandler();
  if (obj_handler->HasSelected()) {
    geometry::RectangleObject* object = obj_handler->GetSelected();
    *static_cast<double*>(value) = object->GetWidth();
  } else {
    value = NULL;
  }
}

}  // namespace visualize
