#ifndef INCLUDE_GEOMETRY_LINE_H_
#define INCLUDE_GEOMETRY_LINE_H_

namespace geometry {

class Point;
class Vector;

class Line {
 public:
  Line(const Point& A, const Point& B);
  Line(const Point& A, const Vector& v);

  bool Intersect(const Line& other, Point* intersection) const;
  Vector Normal() const;

  double GetOrientedDistanceFromPoint(const Point& A) const;
  double GetDistanceFromPoint(const Point& A) const;

 private:
  void Init(const Point& A, const Point& B);

 private:
  double A_, B_, C_;
};

}  // namespace geometry

#endif  // INCLUDE_GEOMETRY_LINE_H_
