#include <stdio.h>
int func (int tmpi, int *ptrInt, char buffer[]);

int main (void){
    static int tmpIS = 60;
    int tmpi;
    int *ptrInt;
    char buffer[8];

    tmpi = 0xC842;
    ptrInt = &tmpi;
    
    func(tmpi, ptrInt, buffer);

    return 1;
}

int func (int tmpi, int *ptrInt, char buffer[]){
    tmpi = 0x12345;
    *ptrInt = 0xFFFFFFFF;
    ptrInt = &tmpi;
    *ptrInt = 0xBEBAFECA;
    buffer[5] = 0x0D;
    buffer[6] = 0xF0;
    return 1;
}
