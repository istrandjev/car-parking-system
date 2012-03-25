#ifndef INCLUDE_GEOMETRY_BOUNDING_BOX_H
#define INCLUDE_GEOMETRY_BOUNDING_BOX_H

namespace geometry {

class Point;

class BoundingBox {
 public:
  BoundingBox();
  BoundingBox(const geometry::Point& point);
  BoundingBox(double minx, double maxx, double miny, double maxy);

  bool IsEmpty() const;
  double GetMinX() const;
  double GetMaxX() const;
  double GetMinY() const;
  double GetMaxY() const;

  BoundingBox GetExpanded(double value);

  bool Contains(const geometry::Point& point) const;
  bool Intersect(const BoundingBox& other) const;

  void UnionWith(const BoundingBox& other);
  void AddPoint(const geometry::Point& point);

 private:
  double minx_, maxx_;
  double miny_, maxy_;
  bool empty_;
};

}  // namespace geometry

#endif  // INCLUDE_GEOMETRY_BOUNDING_BOX_H