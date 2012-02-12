#include "directed_rectangle_object.h"

#include "point.h"

namespace geometry {

DirectedRectangleObject::DirectedRectangleObject(
    const Point& from, const Point& to) 
    : RectangleObject(from, to), oneWay_(false) {};
DirectedRectangleObject::DirectedRectangleObject(
    const Point& from, const Point& to, double width)
    : RectangleObject(from, to, width), oneWay_(false) {}

// virtual
bool DirectedRectangleObject::IsDirected() const {
  return true;
}
  
bool DirectedRectangleObject::IsOneWay() const {
  return oneWay_;
}

void DirectedRectangleObject::SwapOneWayFlag() {
  oneWay_ = !oneWay_;
}

}  // namespace geometry
