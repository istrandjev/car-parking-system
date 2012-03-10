#ifndef INCLUDE_GEOMETRY_SEGMENT_H_
#define INCLUDE_GEOMETRY_SEGMENT_H_

#include "geometry/point.h"

namespace geometry {

class BoundingBox;

class Segment {
 public:
  Segment(const Point& A, const Point& B);
  
  const Point& A() const;
  const Point& B() const;

  BoundingBox GetBoundingBox() const;
  
  bool Intersect(const Segment& other, double* this_fraction, 
      double* other_fraction) const;
  bool Intersect(const Segment& other, double* this_fraction) const;

  Point GetPoint(double fraction) const;
  Segment SubSegment(double start_fraction, double end_fraction) const;

  double Length() const;

 public:
  geometry::Point A_, B_;
};

}  // namespace geometry

#endif  // INCLUDE_GEOMETRY_SEGMENT_H_