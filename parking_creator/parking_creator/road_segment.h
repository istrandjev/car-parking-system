#ifndef ROAD_SEGMENT_H
#define ROAD_SEGMENT_H

#include "point.h"

namespace geometry {

class RoadSegment {
 public:
  RoadSegment(const Point& from, const Point& to);
  RoadSegment(const Point& from, const Point& to, double width);
  bool ContainsPoint(const Point& p) const;
  
  const Point& GetFrom() const;
  void SetFrom(const Point& point);
  
  const Point& GetTo() const;
  void SetTo(const Point& point);

  double GetWidth() const;
  void SetWidth(double width);

  void ReverseDirection();
  
 private:
  Point from_, to_;
  double width_;
};

}  // namespace geometry
#endif  // ROAD_SEGMENT_H