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

}  // namespace geometry
