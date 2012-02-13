#ifndef RECTANGLE_OBJECT_H
#define RECTANGLE_OBJECT_H

#include "point.h"

#include <string>

namespace geometry {

class Polygon;

class RectangleObject {
 public:
  RectangleObject() {};
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
  virtual std::string Serialize();
  virtual void Parse(const std::string& serialized);

  void Translate(double value);  
  void TranslateByNormal(double value);

 protected:
  Point from_, to_;
  double width_;
};

}  // namespace geometry
#endif  // RECTANGLE_OBJECT_H