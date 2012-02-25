#include "utils/double_utils.h"

bool DoubleIsGreater(double lhs, double rhs) {
  return lhs > rhs + epsylon;
}

bool DoubleIsGreaterOrEqual(double lhs, double rhs) {
  return lhs > rhs - epsylon;
}


bool DoubleIsStrictlyBetween(double x, double min_val, double max_val) {
  return x > min_val + epsylon && x < max_val - epsylon;
}

bool DoubleIsBetween(double x, double min_val, double max_val) {
  return x > min_val - epsylon && x < max_val + epsylon;
}

bool DoubleEquals(double lhs, double rhs) {
  return lhs > rhs - epsylon && lhs < rhs + epsylon;
}

bool DoubleIsZero(double x) {
  return DoubleEquals(x, 0.0);
}

int DoubleSign(double x) {
  if (DoubleIsGreater(0.0, x)) {
    return -1;
  } else if (DoubleIsGreater(x, 0.0)) {
    return 1;
  } else {
    return 0;
  }
}
