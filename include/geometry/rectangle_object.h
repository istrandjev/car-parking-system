#ifndef INCLUDE_GEOMETRY_RECTANGLE_OBJECT_H_
#define INCLUDE_GEOMETRY_RECTANGLE_OBJECT_H_

#include "geometry/point.h"

#include <string>

namespace geometry {

class Polygon;
class BoundingBox;

class RectangleObject {
 public:
  RectangleObject() {}
  virtual ~RectangleObject() {}
  RectangleObject(const Point& from, const Point& to);
  RectangleObject(const Point& from, const Point& to, double width);
  bool ContainsPoint(const Point& p) const;

  const Point& GetFrom() const;
  void SetFrom(const Point& point);

  const Point& GetTo() const;
  void SetTo(const Point& point);

  double GetWidth() const;
  void SetWidth(double width);

  Polygon GetBounds() const;
  virtual BoundingBox GetBoundingBox() const;

  virtual bool IsDirected() const;
  virtual std::string Serialize();
  virtual void Parse(const std::string& serialized);

  void Translate(double translatex, double translatey);
  void TranslateByNormal(double value);

 protected:
  Point from_, to_;
  double width_;
};

}  // namespace geometry
#endif  // INCLUDE_GEOMETRY_RECTANGLE_OBJECT_H_
