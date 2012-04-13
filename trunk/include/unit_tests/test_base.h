#ifndef INCLUDE_UNIT_TESTS_TEST_BASE_H
#define INCLUDE_UNIT_TESTS_TEST_BASE_H

#include "utils/double_utils.h"

#define ASSERT(STATEMENT) if (!(STATEMENT)) {\
  fprintf(stderr, "%s(%d): Assertion failed: "#STATEMENT"\n" ,\
      __FILE__, __LINE__);\
  }

#define ASSERT_EQUALS(X, Y) ASSERT(X == Y)
#define ASSERT_NOT_EQUALS(X, Y) ASSERT(X != Y)
#define ASSERT_DOUBLE_EQUALS(X, Y) ASSERT(DoubleEquals(X, Y))
#define ASSERT_DOUBLE_NOT_EQUALS(X, Y) ASSERT(!DoubleEquals(X, Y))

#endif  // INCLUDE_UNIT_TESTS_TEST_BASE_H