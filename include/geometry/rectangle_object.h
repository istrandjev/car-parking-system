#ifndef INCLUDE_GEOMETRY_RECTANGLE_OBJECT_H_
#define INCLUDE_GEOMETRY_RECTANGLE_OBJECT_H_

#include "geometry/point.h"

#include <string>

namespace geometry {

class Polygon;
class BoundingBox;

class RectangleObject {
 public:
  RectangleObject();
  RectangleObject(const Point& from, const Point& to);
  RectangleObject(const Point& from, const Point& to, double width);

  virtual ~RectangleObject();
  bool ContainsPoint(const Point& p) const;

  const Point& GetFrom() const;
  void SetFrom(const Point& point);

  const Point& GetTo() const;
  void SetTo(const Point& point);

  double GetWidth() const;
  void SetWidth(double width);

  bool IsObstacle() const;
  void SetIsObstacle(bool is_obstacle);

  Polygon GetBounds() const;
  Polygon GetExpandedBounds(double expand) const;
  virtual BoundingBox GetBoundingBox() const;

  virtual bool IsDirected() const;
  virtual std::string Serialize();
  virtual void Parse(const std::string& serialized);

  void Translate(double translatex, double translatey);
  void TranslateByNormal(double value);

 protected:
  Point from_, to_;
  double width_;
  bool isObstacle_;
};

}  // namespace geometry
#endif  // INCLUDE_GEOMETRY_RECTANGLE_OBJECT_H_
