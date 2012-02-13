#include "geometry/rectangle_object.h"

#include "geometry/geometry_utils.h"
#include "geometry/point.h"
#include "geometry/polygon.h"
#include "geometry/vector.h"
#include "utils/double_utils.h"

#include <string>
#include <sstream>

namespace geometry {

static const double DEFAULT_WIDTH = 2.0;

RectangleObject::RectangleObject(const Point& from, const Point& to) 
    : from_(from), to_(to), width_(DEFAULT_WIDTH){}

RectangleObject::RectangleObject(const Point& from, const Point& to, double width)
    : from_(from), to_(to), width_(width) {}

const Point& RectangleObject::GetFrom() const {
  return from_;
}

void RectangleObject::SetFrom(const Point& point) {
  from_ = point;
}
  
const Point& RectangleObject::GetTo() const {
  return to_;
}

void RectangleObject::SetTo(const Point& point) {
  to_ = point;
}

double RectangleObject::GetWidth() const {
  return width_;
}

void RectangleObject::SetWidth(double width) {
  width_ = width;
}

bool RectangleObject::ContainsPoint(const Point& p) const {
  Vector segment(from_, to_);

  Vector from_p(from_, p);
  if (DoubleIsGreater(0.0, segment.DotProduct(from_p))) {
    return false;
  }

  Vector to_p(to_, p);
  if (DoubleIsGreater(segment.DotProduct(to_p), 0.0)) {
    return false;
  }

  return DoubleIsGreaterOrEqual(width_ * segment.Length(),
      fabs(segment.CrossProduct(from_p)));
}

Polygon RectangleObject::GetBounds() const {
  Vector segment(from_, to_);
  Vector shift = segment.GetOrthogonal().Unit() * width_ * 0.5;
  Polygon result;
  result.AddPointDropDuplicates(from_ + shift);
  result.AddPointDropDuplicates(from_ - shift);
  result.AddPointDropDuplicates(to_ - shift);
  result.AddPointDropDuplicates(to_ + shift);
  return result;
}

// virtual
bool RectangleObject::IsDirected() const {
  return false;
}

// virtual 
std::string RectangleObject::Serialize() {
  std::ostringstream out;
  out << from_ << " " << to_ << " "<< width_;
  return out.str();
}

// virtual 
void RectangleObject::Parse(const std::string& serialized) {
  std::istringstream in(serialized);
  in >> from_ >> to_ >> width_;
}

void RectangleObject::Translate(double translatex, double translatey) {
  Vector translation = Vector(translatex, translatey);
  from_ = from_ + translation;
  to_ = to_ + translation;
}

void RectangleObject::TranslateByNormal(double value) {
  Vector translation = Vector(from_, to_).GetOrthogonal().Unit() * value;
  from_ = from_ + translation;
  to_ = to_ + translation;
}

}  // namespace geometry