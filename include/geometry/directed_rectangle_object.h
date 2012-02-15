#ifndef INCLUDE_GEOMETRY_DIRECTED_RECTANGLE_OBJECT_H_
#define INCLUDE_GEOMETRY_DIRECTED_RECTANGLE_OBJECT_H_

#include "geometry/rectangle_object.h"

#include <string>

namespace geometry {

class Point;

class DirectedRectangleObject : public RectangleObject {
 public:
  DirectedRectangleObject();
  DirectedRectangleObject(const Point& from, const Point& to);
  DirectedRectangleObject(const Point& from, const Point& to, double width);

  virtual bool IsDirected() const;

  void SetIsOneWay(bool is_one_way);
  bool IsOneWay() const;

  void SwapOneWayFlag();
  void ReverseDirection();

  virtual std::string Serialize();
  virtual void Parse(const std::string& serialized);
  static bool IsSerializedDirectedRectangleObject(const std::string& str);

 private:
  bool oneWay_;
};

}  // namespace geometry

#endif  // INCLUDE_GEOMETRY_DIRECTED_RECTANGLE_OBJECT_H_
