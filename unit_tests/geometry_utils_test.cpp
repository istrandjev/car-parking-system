#include "geometry/geometry_utils.h"

#include "geometry/vector.h"

#include "unit_tests/test_base.h"
#include "utils/double_utils.h"

#include <iostream>
#include <cmath>

using namespace std;

class TestGeometryUtils : public geometry::GeometryUtils {
 public:
  static void RunTests();
  static void TestNormalizeAngle();
  static void TestGetAngleBetweenVectors();

 private:
  static geometry::Vector GetUnitVectorRotatedByAngle(double angle);
};

// static
void TestGeometryUtils::RunTests() {
  TestNormalizeAngle();
  TestGetAngleBetweenVectors();
}

// static
void TestGeometryUtils::TestNormalizeAngle() {
  const double pi = geometry::GeometryUtils::PI;
  ASSERT_DOUBLE_EQUALS(pi * 0.25, NormalizeAngle(pi * 0.25));
  ASSERT_DOUBLE_EQUALS(pi * 1.75, NormalizeAngle(-pi * 0.25));
  ASSERT_DOUBLE_EQUALS(0, NormalizeAngle(pi * 2.0));
  ASSERT_DOUBLE_EQUALS(pi * 0.25, NormalizeAngle(pi * 14.25));
  ASSERT_DOUBLE_EQUALS(pi * 0.5, NormalizeAngle(pi * -13.5));
}

// static
void TestGeometryUtils::TestGetAngleBetweenVectors() {
  const double pi = geometry::GeometryUtils::PI;
  geometry::Vector a = GetUnitVectorRotatedByAngle(0);
  geometry::Vector b = GetUnitVectorRotatedByAngle(pi * 0.25);
  geometry::Vector c = GetUnitVectorRotatedByAngle(pi * 0.5);
  geometry::Vector d = GetUnitVectorRotatedByAngle(pi * 0.33);
  geometry::Vector e = GetUnitVectorRotatedByAngle(pi * 1.5);

  ASSERT_DOUBLE_EQUALS(pi * 0.25, GetAngleBetweenVectors(a, b));
  ASSERT_DOUBLE_EQUALS(pi * (2.0 - 0.25), GetAngleBetweenVectors(b, a));
  ASSERT_DOUBLE_EQUALS(pi * 0.08, GetAngleBetweenVectors(b, d));
  ASSERT_DOUBLE_EQUALS(pi * 1.17, GetAngleBetweenVectors(d, e));
  ASSERT_DOUBLE_EQUALS(pi, GetAngleBetweenVectors(c, e));
}

// static
geometry::Vector TestGeometryUtils::GetUnitVectorRotatedByAngle(double angle) {
  return geometry::Vector(cos(angle), sin(angle));
}

int main() {
  TestGeometryUtils::RunTests();
  return 0;
}
