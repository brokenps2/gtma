#include <stdlib.h>

int randBetween(int a, int b) { 
    return rand() % (b - a + 1) + a;
}
