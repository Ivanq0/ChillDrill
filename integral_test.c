#include "integral.h"
#include <assert.h>
#include <math.h>
#define PI 3.14159265358979323846

void test_x_squared() {
  double a = 0.0;
  double b = 1.0;
  int n = 10000;
  double res = search_integral(x_squared, a, b, n);

  double eps = 1E-5;
  double nl = (pow(b, 3) / 3) - (pow(a, 3) / 3);
  assert(fabs(res - nl) < eps);
}
void test_exp() {
  double a = 3.5;
  double b = 6.0;
  int n = 10000;
  double res = search_integral(exp, a, b, n);

  double eps = 1E-5;
  double nl = exp(b) - exp(a);
  assert(fabs(res - nl) < eps);
}
void test_sin() {
  double a = 5.0;
  double b = 30.0;
  int n = 10000;
  double res = search_integral(sin, a, b, n);

  double eps = 1E-5;
  double nl = -cos(b) - (-cos(a));
  assert(fabs(res - nl) < eps);
}
void test_invertrd_sin() {
  double a = PI;
  double b = 0;
  int n = 10000;
  double res = search_integral(sin, a, b, n);

  double eps = 1E-5;
  double nl = -cos(b) - (-cos(a));
  assert(fabs(res - nl) < eps);
}

int main() {
  test_x_squared();
  test_exp();
  test_sin();
  test_invertrd_sin();
  return 0;
}
