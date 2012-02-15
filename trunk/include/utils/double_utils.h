// This file contains some utility methods for double comparisons.

#ifndef INCLUDE_UTILS_DOUBLE_UTILS_H_
#define INCLUDE_UTILS_DOUBLE_UTILS_H_

static const double epsylon = 1e-9;

// @return - true iff "lhs" is greater then "rhs" with double tolerance.
bool DoubleIsGreater(double lhs, double rhs);

// @return true iff "lhs" is greater then or equal to "rhs" with double
//     tolerance.
bool DoubleIsGreaterOrEqual(double lhs, double rhs);

// @return true iff "lhs" is equal to "rhs" with double tolerance.
bool DoubleEquals(double lhs, double rhs);

// Returns the sign of the double parameter with some double tolerance.
// @return - (-1) if x is negative
//           (0) if x is zero
//           (1) if x is positive
int DoubleSign(double x);

// Returns true iff x is zero with double error tolerance.
// @param x - double value to check if zero.
// @return - true iff x is zero.
bool DoubleIsZero(double x);

#endif  // INCLUDE_UTILS_DOUBLE_UTILS_H_
