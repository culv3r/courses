#include <stdio.h>
#include <inttypes.h>

typedef struct {
    char a;
    double b;
    char c;
    double d;
    char e;
    double f;
    char g;
    double h;
    char i;
} S1;

typedef struct {
    char a;
    char c;
    char e;
    char g;
    char i;
    double b;
    double d;
    double f;
    double h;
} S2;

int main() {
    double size1 = sizeof(S1);
    double size2 = sizeof(S2);
    printf("size: %lu\n", sizeof(S1));
    printf("size: %lu\n", sizeof(S2));
    printf("size-inefficiency: %.2f\n",100 * (size1 - size2) / size2);
    return 0;
}
