// This file defines some helper function for geometrical calulations.

#ifndef GEOMETRY_UTILS_H
#define GEOMETRY_UTILS_H

#include "point.h"

namespace geometry {

class GeometryUtils {
 public:
  // @return - the oriented area of the triangle ABC.
  static double GetOrientedArea(const Point& A, const Point& B, const Point& C);

  // Checks if points 'X' and 'Y' are in the same semi-plane defined
  // by the line passing though 'A' and 'B'. If the parameter closed is true then
  // 'X' and 'Y' should belong to the same closed semi-plane (i.e. one of them may 
  // lay on AB), otherwise they should be strictly in the same semi-plane.
  // @param A - first point defining the line separating the semi-planes
  // @param B - second point defining the line separating the semi-planes
  // @param X - point to check if it is in the same semi-plane with Y.
  // @param Y - point to check if it is in the same semi-plane with X.
  // @param closed - determines if the semi-plane is closed.
  // @return - true - iff points 'X' and 'Y' are in the same semi-plane defined
  //    by the line passing though 'A' and 'B'.
  static bool PointsAreInSameSemiPlane(const Point& A, const Point& B, 
      const Point& X, const Point& Y, bool closed);

  // Checks if X is in the positive semi-plane with respect to the line A->B.
  // @param A
  // @param B
  // @param X
  // @return - true iff X is in the positive semi-plane with respect to A->B.
  static bool InPositiveSemiPlane(const Point& A, const Point& B, const Point& X);

  // Checks if X is in the negative semi-plane with respect to the line A->B.
  // @param A
  // @param B
  // @param X
  // @return - true iff X is in the negative semi-plane with respect to A->B.
  static bool InNegativeSemiPlane(const Point& A, const Point& B, const Point& X);

  // Checks if X lies on the line AB.
  // @return - true iff X lies on the line AB.
  static bool LiesOnLine(const Point& A, const Point& B, const Point& X);

  // Converts an angle in degrees to the same angle but in radians.
  static double DegreesToRadians(double degrees);

 public:
  static const double PI;

 private:
  static int GetSemiPlaneSign(const Point& A, const Point& B, const Point& X);
};

}  // namespace geometry

#endif  // GEOMETRY_UTILS_H