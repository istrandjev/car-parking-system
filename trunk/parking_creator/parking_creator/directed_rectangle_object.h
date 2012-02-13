#ifndef DIRECTED_RECTANGLE_OBJECT_H
#define DIRECTED_RECTANGLE_OBJECT_H

#include "rectangle_object.h"

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

#endif  // DIRECTED_RECTANGLE_OBJECT_H