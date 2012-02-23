#ifndef INCLUDE_GEOMETRY_BOUNDING_BOX_H
#define INCLUDE_GEOMETRY_BOUNDING_BOX_H

namespace geometry {

class BoundingBox {
 public:
  BoundingBox();
  BoundingBox(double minx, double maxx, double miny, double maxy);
  
  bool IsEmpty() const;
  double GetMinX() const;
  double GetMaxX() const;
  double GetMinY() const;
  double GetMaxY() const;

 private:
  double minx_, maxx_;
  double miny_, maxy_;
  bool empty_;
};

}  // namespace geometry

#endif  // INCLUDE_GEOMETRY_BOUNDING_BOX_H