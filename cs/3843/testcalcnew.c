#include <stdio.h>

int calcnew(int x, int y, int z);

int main(){
    int x = 2;
    int y = 6;
    int z = 11;
    
    int ret = calcnew(x,y,z);

    printf("x=%d, y=%d, z=%d, result=%d\n", x, y, z, ret);
    return 0;
}
