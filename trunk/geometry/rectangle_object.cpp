#include "geometry/rectangle_object.h"

#include "geometry/bounding_box.h"
#include "geometry/geometry_utils.h"
#include "geometry/point.h"
#include "geometry/polygon.h"
#include "geometry/segment.h"
#include "geometry/vector.h"
#include "utils/double_utils.h"

#include <algorithm>
#include <string>
#include <sstream>

#include <cmath>

namespace geometry {

static const double DEFAULT_WIDTH = 2.0;

RectangleObject::RectangleObject() : isObstacle_(false) {}


RectangleObject::RectangleObject(const Point& from, const Point& to)
    : from_(from), to_(to), width_(DEFAULT_WIDTH) {}

RectangleObject::RectangleObject(const Point& from, const Point& to,
    double width) : from_(from), to_(to), width_(width) {}

RectangleObject::~RectangleObject() {}

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

bool RectangleObject::IsObstacle() const {
  return isObstacle_;
}

void RectangleObject::SetIsObstacle(bool is_obstacle) {
  isObstacle_ = is_obstacle;
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

  return DoubleIsGreaterOrEqual(width_ * segment.Length() * 0.5,
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

Polygon RectangleObject::GetExpandedBounds(double expand) const {
  Vector ox = Vector(from_, to_).Unit() * expand;
  Vector oy = ox.GetOrthogonal().Unit() * (width_ * 0.5 + expand);
  Polygon result;
  result.AddPointDropDuplicates(from_ - ox + oy);
  result.AddPointDropDuplicates(from_ - ox - oy);
  result.AddPointDropDuplicates(to_ + ox - oy);
  result.AddPointDropDuplicates(to_ + ox + oy);
  return result;
}
// virtual
BoundingBox RectangleObject::GetBoundingBox() const {
  Polygon bounds = GetBounds();
  double minx, maxx;
  double miny, maxy;
  minx = maxx = bounds.GetPoint(0).x;
  miny = maxy = bounds.GetPoint(0).y;

  for (unsigned index = 0; index < bounds.NumberOfVertices();
      ++index) {
    const Point& point = bounds.GetPoint(index);
    minx = std::min(minx, point.x);
    maxx = std::max(maxx, point.x);
    miny = std::min(miny, point.y);
    maxy = std::max(maxy, point.y);
  }
  return BoundingBox(minx, maxx, miny, maxy);
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

bool AreTouching(const RectangleObject& a, const RectangleObject& b) {
  geometry::Polygon a_bounds = a.GetExpandedBounds(0.3);
  geometry::Polygon b_bounds = b.GetExpandedBounds(0.3);
  // geometry::Polygon a_bounds = a.GetBounds();
  // geometry::Polygon b_bounds = b.GetBounds();
  for (unsigned i = 0; i < a_bounds.NumberOfSides(); ++i) {
    if (geometry::Intersect(b_bounds, a_bounds.GetSide(i), NULL)) {
      return true;
    }
  }

  return false;
}

}  // namespace geometry
