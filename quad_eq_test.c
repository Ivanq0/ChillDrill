#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "quad_eq.h"

void test_quad_a_is_zero() {
  double a = 0;
  double b = 0;
  double c = 0;
  double roots[2];

  int res = get_roots(a, b, c, roots);
  assert(res == 0);
}

void test_quad_d_plus() {
  double a = 1;
  double b = 0;
  double c = -1;
  double roots[2];

  int res = get_roots(a, b, c, roots);
  assert(res == 2);
  assert(roots[0] == -1 && roots[1] == 1);
}

void test_quad_d_zero() {
  double a = 1;
  double b = 0;
  double c = 0;
  double roots[2];

  int res = get_roots(a, b, c, roots);
  assert(res == 1);
  assert(roots[0] == 0);
}

void test_quad_d_minus() {
  double a = 1;
  double b = 0;
  double c = 1;
  double roots[2];

  int res = get_roots(a, b, c, roots);
  assert(res == 0);
}

void test_quad_c_is_low() {
  const double EPS = 1e-3;
  double a = 1;
  double b = 0;
  double c = -1e-7;
  double roots[2];

  int res = get_roots(a, b, c, roots);
  assert(res == 2);

  assert(fabs(roots[0]) < EPS && fabs(roots[1]) < EPS);
}

void test_quad_b_is_low() {
  const double EPS = 1e-9;
  double a = 1;
  double b = -1e10;
  double c = -1;
  double roots[2];

  int res = get_roots(a, b, c, roots);
  assert(res == 2);
  assert(fabs(roots[0]) < EPS);
  assert(fabs(roots[1]) == 1e10);
}

void test_quad_c_is_lowest() {
  const double EPS = 1e-6;
  double a = 1;
  double b = 0;
  double c = -1e-8;
  double roots[2];

  int res = get_roots(a, b, c, roots);
  assert(res == 1);
  assert(fabs(roots[0]) < EPS);
}

int main() {
  test_quad_a_is_zero();
  test_quad_d_plus();
  test_quad_d_zero();
  test_quad_d_minus();
  test_quad_c_is_low();
  test_quad_b_is_low();
  test_quad_c_is_lowest();
}