#include "geometry/geometry_utils.h"

#include "geometry/point.h"
#include "geometry/vector.h"
#include "utils/double_utils.h"

#include <cmath>

namespace geometry {

// static
const double GeometryUtils::PI = 3.141592653589;

// static
double GeometryUtils::GetOrientedArea(const Point& A, const Point& B,
    const Point& C) {
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
bool GeometryUtils::InPositiveSemiPlane(const Point& A, const Point& B,
    const Point& X) {
  return GetSemiPlaneSign(A, B, X) == 1;
}

// static
bool GeometryUtils::InNegativeSemiPlane(const Point& A, const Point& B,
    const Point& X) {
  return GetSemiPlaneSign(A, B, X) == -1;
}

// static
bool GeometryUtils::LiesOnLine(const Point& A, const Point& B,
    const Point& X) {
  return GetSemiPlaneSign(A, B, X) == 0;
}

// static
int GeometryUtils::GetSemiPlaneSign(const Point& A, const Point& B,
    const Point& X) {
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
bool GeometryUtils::TriangleContains(const Point& A, const Point& B, const Point& C,
      const Point& X) {
  int s = GetSemiPlaneSign(A, B, C);
  return GetSemiPlaneSign(A, B, X) == s && GetSemiPlaneSign(B, C, X) == s &&
      GetSemiPlaneSign(C, A, X) == s;
}

// static
double GeometryUtils::DegreesToRadians(double degrees) {
  return (degrees * PI) / 180.0;
}

// static
double GeometryUtils::NormalizeAngle(double angle) {
  if (DoubleIsGreater(0, angle)) {
    return angle + ceil(-angle/ (PI * 2.0)) * PI * 2;
  }
  if (DoubleIsGreaterOrEqual(angle, 2 * PI)) {
    return angle - ceil((angle - 2.0 * PI) / ( 2.0 * PI) + epsylon) * PI * 2;
  }
  return angle;
}

double GeometryUtils::GetAngleBetweenVectors(const Vector &v1,
                                             const Vector &v2) {
  double len1 = v1.Length();
  double len2 = v2.Length();
  if (DoubleIsZero(len1) || DoubleIsZero(len2)) {
    return 0.0;
  }

  double dot_product = v1.DotProduct(v2);
  double cosine = dot_product / (len1 * len2);
  double angle = acos(cosine);

  if (DoubleIsGreater(0, v1.CrossProduct(v2))) {
    angle = 2.0 * PI - angle;
  }

  return angle;
}

}  // namespace geometry
