// This file contains some utility methods for double comparisons.

#ifndef INCLUDE_UTILS_DOUBLE_UTILS_H_
#define INCLUDE_UTILS_DOUBLE_UTILS_H_

static const double epsylon = 1e-9;

// @return - true iff "lhs" is greater then "rhs" with double tolerance.
bool DoubleIsGreater(double lhs, double rhs);

// @return true iff "lhs" is greater then or equal to "rhs" with double
//     tolerance.
bool DoubleIsGreaterOrEqual(double lhs, double rhs);

// @return true iff 'x' belongs to the open interval (min_val, max_val).
bool DoubleIsStrictlyBetween(double x, double min_val, double max_val);

// @return true iff 'x' belongs to the closed interval [min_val, max_val].
bool DoubleIsBetween(double x, double min_val, double max_val);

// @return true iff the intervals [min1, max1] and [min2, max2] overlap.
bool DoubleIntervalsOverlap(double min1, double max1,
                            double min2, double max2);

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