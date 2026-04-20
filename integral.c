#include "integral.h"
#include <math.h>
#include <stdio.h>

double x_squared(double x) { return x * x; }
double search_integral(double (*f)(double), double a, double b, int n){
    if (n <= 0 || f == NULL){
        return -1.0;
    }
    if (a > b){
        return -search_integral(f, b, a, n);
    }
    double sum = 0.0;
    double h = (b - a) / n;
    double x = a;
    for (int i = 0; i < n; i++){
        double y1 = f(x);
        double y2 = f(x + h);
        sum += ((y1 + y2) / 2.0) * h;
        x += h;
    }
    return sum;
}