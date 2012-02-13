#ifndef LINE_H
#define LINE_H

namespace geometry {

class Point;
class Vector;

class Line{
 public:
	Line(const Point& A, const Point& B);
  Line(const Point& A, const Vector& v);

  bool Intersect(const Line& other, Point* intersection) const;
  Vector Normal() const;

 private:
  void Init(const Point& A, const Point& B);
 
 private:
  double A_, B_, C_;
};

}  // namespace geometry

#endif  // LINE_H