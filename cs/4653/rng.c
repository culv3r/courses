#include <stdio.h>

int main(int argc, char *argv[]){
    static long state = 1;

    const long A = 17;
    const long M = 65337;
    const long Q = M / A;
    const long R = M % A;
    long t = 1;

    for(int i = 0; i< 512; i++){

    t = A * (state % Q) - R * (state / Q);

    if (t > 0)
        state = t;
    else
        state = t + M;
    printf("%lf\n",((double) state / M));
    }

    return 1;
}
