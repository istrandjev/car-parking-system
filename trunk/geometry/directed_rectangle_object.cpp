#include "geometry/directed_rectangle_object.h"

#include "geometry/point.h"

#include <algorithm>
#include <cstring>
#include <string>

namespace geometry {

static const char* DIRECTED_RECTANGLE_OBJECT_STAMP = "DirectedRectangleObject";
static const int STAMP_SIZE = strlen(DIRECTED_RECTANGLE_OBJECT_STAMP);

DirectedRectangleObject::DirectedRectangleObject() : oneWay_(false) {}

DirectedRectangleObject::DirectedRectangleObject(
    const Point& from, const Point& to)
    : RectangleObject(from, to), oneWay_(false) {}
DirectedRectangleObject::DirectedRectangleObject(
    const Point& from, const Point& to, double width)
    : RectangleObject(from, to, width), oneWay_(false) {}

// virtual
bool DirectedRectangleObject::IsDirected() const {
  return true;
}

void DirectedRectangleObject::SetIsOneWay(bool is_one_way) {
  oneWay_ = is_one_way;
}

bool DirectedRectangleObject::IsOneWay() const {
  return oneWay_;
}

void DirectedRectangleObject::SwapOneWayFlag() {
  oneWay_ = !oneWay_;
}


void DirectedRectangleObject::ReverseDirection() {
  std::swap(from_, to_);
}

// virtual
std::string DirectedRectangleObject::Serialize() {
  std::string serialized(DIRECTED_RECTANGLE_OBJECT_STAMP);
  serialized += " ";
  if (oneWay_) {
    serialized += "1 ";
  } else {
    serialized += "0 ";
  }
  serialized += RectangleObject::Serialize();
  return serialized;
}

// virtual
void DirectedRectangleObject::Parse(const std::string& serialized) {
  if (serialized[STAMP_SIZE + 1] == '1') {
    oneWay_ = true;
  } else {
    oneWay_ = false;
  }
  RectangleObject::Parse(serialized.substr(STAMP_SIZE + 3));
}

// static
bool DirectedRectangleObject::IsSerializedDirectedRectangleObject(
    const std::string& str) {
  return str.substr(0, STAMP_SIZE) == DIRECTED_RECTANGLE_OBJECT_STAMP;
}

}  // namespace geometry
