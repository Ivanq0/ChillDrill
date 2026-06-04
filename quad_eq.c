#include "quad_eq.h"
#include <math.h>
#include <stdio.h>

int sign(int x) {
  if (x > 0) {
    return 1;
  } else if (x == 0) {
    return 0;
  } else {
    return -1;
  }
}
int get_roots(double a, double b, double c, double roots[2]) {

  const double EPS = 1e-7;

  if (a == 0) {
    return 0;
  }

  if (b == 0 && fabs(c) < EPS) {
    roots[0] = 0;
    return 1;
  }

  if (b == 0 && c < 0) {
    roots[0] = -sqrt(-c);
    roots[1] = sqrt(-c);
    return 2;
  }

  double d = b * b - 4 * a * c;

  if (d < 0)
    return 0;

  double x1 = -(b + sign(b) * sqrt(d)) / 2;
  double x2 = c / x1;

  if (x2 > x1) {
    roots[0] = x1;
    roots[1] = x2;
  } else {
    roots[0] = x2;
    roots[1] = x1;
  }

  return 2;
}