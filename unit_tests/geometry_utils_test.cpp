#include "geometry/geometry_utils.h"

#include "unit_tests/test_base.h"
#include "utils/double_utils.h"

#include <iostream>

using namespace std;

class TestGeometryUtils : public geometry::GeometryUtils {
 public:
  static void TestNormalizeAngle();
};

// static
void TestGeometryUtils::TestNormalizeAngle() {
  const double pi = geometry::GeometryUtils::PI;
  ASSERT_DOUBLE_EQUALS(pi * 0.25, NormalizeAngle(pi * 0.25));
  ASSERT_DOUBLE_EQUALS(pi * 1.75, NormalizeAngle(-pi * 0.25));
  ASSERT_DOUBLE_EQUALS(0, NormalizeAngle(pi * 2.0));
  ASSERT_DOUBLE_EQUALS(pi * 0.25, NormalizeAngle(pi * 14.25));
  ASSERT_DOUBLE_EQUALS(pi * 0.5, NormalizeAngle(pi * -13.5));
}
