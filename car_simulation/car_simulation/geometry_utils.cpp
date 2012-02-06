#include "geometry_utils.h"

#include "double_utils.h"
#include "point.h"

namespace geometry {

// static
const double GeometryUtils::PI = 3.141592653589;

// static
double GeometryUtils::GetOrientedArea(const Point& A, const Point& B, const Point& C) {
  return (A.x * (B.y - C.y) + B.x * (C.y - A.y) + C.x * (A.y - B.y)) * 0.5;
}

// static
bool GeometryUtils::PointsAreInSameSemiPlane(const Point& A, const Point& B, 
    const Point& X, const Point& Y, bool closed) {
  int sign1 = GetSemiPlaneSign(A, B, X);
  int sign2 = GetSemiPlaneSign(A, B, Y);

  if (closed) {
    return sign1 == 0 || sign2 == 0 || sign1 == sign2;
  } else {
    return sign1 == sign2;
  }
}

// static
bool GeometryUtils::InPositiveSemiPlane(const Point& A, const Point& B, const Point& X) {
  return GetSemiPlaneSign(A, B, X) == 1;
}

// static
bool GeometryUtils::InNegativeSemiPlane(const Point& A, const Point& B, const Point& X) {
  return GetSemiPlaneSign(A, B, X) == -1;
}

// static
bool GeometryUtils::LiesOnLine(const Point& A, const Point& B, const Point& X) {
  return GetSemiPlaneSign(A, B, X) == 0;
}

// static
int GeometryUtils::GetSemiPlaneSign(const Point& A, const Point& B, const Point& X) {
  double area = GetOrientedArea(A, B, X);

  if (DoubleIsGreater(area, 0.0)) {
    return 1;
  } else if (DoubleIsGreater(0.0, area)) {
    return -1;
  } else {
    return 0;
  }
}

// static 
double GeometryUtils::DegreesToRadians(double degrees) {
  return (degrees * PI) / 180.0;
}

}  // namespace geometry
