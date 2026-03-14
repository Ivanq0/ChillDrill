#include <assert.h>
#include <math.h>

#include "add.h"

void test_add_int_1_2(){
    int a = 1;
    int b = 2;

    int res = add_int(a, b);
    assert(res == 3);
}

int main() {
    test_add_int_1_2();
    return 0;
}