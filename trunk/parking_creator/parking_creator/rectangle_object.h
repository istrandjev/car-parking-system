#ifndef RECTANGLE_OBJECT_H
#define RECTANGLE_OBJECT_H

#include "point.h"

namespace geometry {

class Polygon;

class RectangleObject {
 public:
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
  
  virtual bool IsDirected() const;

 protected:
  Point from_, to_;
  double width_;
};

}  // namespace geometry
#endif  // RECTANGLE_OBJECT_H