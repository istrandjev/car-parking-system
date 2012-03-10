#ifndef INCLUDE_GEOMETRY_POLYGON_H_
#define INCLUDE_GEOMETRY_POLYGON_H_

#include "geometry/point.h"

#include <iostream>
#include <vector>

namespace geometry {

class Segment;
class Vector;

// This class represents a 2D polygon.
class Polygon {
 public:
  Polygon() {}
  explicit Polygon(const std::vector<Point>& points);

  // Normalizes the polygon in a way that its vertices are listed in
  // counter-clockwise direction.
  void Normalize();

  // Adds a point after the last added to the polygon. If the point is the same
  // as the last added (with a small tolerance) no point will be added.
  // NOTE: even if the point is the same as the one that is first in the polygon
  //     it will still be added.
  void AddPointDropDuplicates(const Point& point);

  friend void Intersect(const Polygon& poly1, const Polygon& poly2,
    std::vector<Polygon>* intersection);

  // @return - the number of the vertices of the polygon.
  unsigned NumberOfVertices() const;
  
  // @return - the number of sides of the polygon.
  unsigned NumberOfSides() const;

  // @return the point in the polygon with the specified index.
  inline const Point& GetPoint(unsigned index) const;

  // @return - the point in the polygon with the specified index. The difference
  //     with the above version is that point number N is considered to be point
  //     0 point N + 1 is point 1 and so on.
  inline const Point& GetPointCyclic(int index) const;

  // @return - a segment that represents the side with the given index.
  Segment GetSide(unsigned side_index) const;

  // @return - a vector perpendicular to the side and point to the inside of
  //    the polygon.
  Vector GetSideNormal(unsigned side_index) const;

  // Checks if a point is contained within the boundary of the polygon. For
  // points lying on the boundary this method still returns true.
  bool ContainsPoint(const Point& point) const;

  // Translates the plygon by the supplied "translation_vector".
  void Translate(const Vector& translation_vector);

  friend std::istream& operator>>(std::istream& in, Polygon& polygon);
  friend std::ostream& operator<<(std::ostream& out, const Polygon& polygon);

 private:
  std::vector<Point> points_;
};

// Intersects the two polygons and stores the set of the polygons representing
// their intersection in "intersections".
// @param poly1 - frist polygon
// @param poly2 - second polygon
// @param intersections - vector where the intersection polygons will be stored.
//     Output parameter. Can not be NULL.
void Intersect(const Polygon& poly1, const Polygon& poly2,
  std::vector<Polygon>* intersection);

std::istream& operator>>(std::istream& in, Polygon& polygon);
std::ostream& operator<<(std::ostream& out, const Polygon& polygon);

}  // namespace geometry

#endif  // INCLUDE_GEOMETRY_POLYGON_H_
