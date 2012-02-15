#include "geometry/line.h"

#include "geometry/point.h"
#include "geometry/vector.h"
#include "utils/double_utils.h"

namespace geometry {

Line::Line(const Point& A, const Point& B) {
  Init(A, B);
}

Line::Line(const Point& A, const Vector& v) {
  Init(A, A + v);
}

void Line::Init(const Point& A, const Point& B) {
  A_ = A.y - B.y;
  B_ = B.x - A.x;
  C_ = A.x * B.y - B.x * A.y;
}

Vector Line::Normal() const {
  return Vector(A_, B_);
}

bool Line::Intersect(const Line& other, Point* intersection) const {
  double cross_product = other.Normal().CrossProduct(Normal());
  if (DoubleIsZero(cross_product)) {
    return false;
  }
  intersection->y = (A_ * other.C_ - C_ * other.A_) / cross_product;
  if (DoubleIsZero(A_)) {
    intersection->x = (-other.B_ * intersection->y - other.C_)/other.A_;
  } else {
    intersection->x = (-B_ * intersection->y - C_) / A_;
  }
  return true;
}

}  // namespace geometry
