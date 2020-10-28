#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define test(x) if (x()) { printf("PASS\n"); } else { printf("FAIL\n\n"); exit(1); }

bool test0() {
    printf("should add a listener for an event emitter that fires on every dispatch: ");
    return false;
}

bool test1() {
    printf("should add multiple listeners that persist: ");
    return false;
}

bool test2() {
    printf("should remove a listener: ");
    return false;
}

bool test3() {
    printf("should add a listener that persists and one that fires only once: ");
    return false;
}

int main() {
    printf("\nrunning tests\n\n");

    test(test0);
    test(test1);
    test(test2);
    test(test3);

    printf("\n\nSUCCESS! all tests PASSED\n\n");
    return 0;
}
