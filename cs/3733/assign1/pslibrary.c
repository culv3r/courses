#include "pslibrary.h"

void part0(char *s1, char *s2){
    strcpy(s1,"RRwwwwwRRRRRRRRR");
    strcpy(s2,"rrRRRRwwwwwwwwrrRRRRRRR");
    return;
}

void fcfsa(char *s1, char *s2, int x1, int y1, int z1, int x2, int y2, int z2){
    int move = 0;
    char *ptr;
    int r1 = 0;
    
    for (int x = 0; x < x1; x++){
        s1[x] = 'R';
        s2[x] = 'r';
    }
    for (int yOne = 0; yOne < y1; yOne++){
        s1[yOne+x1] = 'w';
    }
    for (int zOne = 0; zOne < z1; zOne++){
        s1[zOne+x1+y1] = 'R';
    }
    for (int xTwo = 0; xTwo < x2; xTwo++){
        s2[x1+xTwo] = 'R';
    }
    for (int yTwo = 0; yTwo < y2; yTwo++){
        s2[x1+x2+yTwo] = 'w';
    }
    for (int zTwo = 0; zTwo < z2; zTwo++){
        s2[x1+x2+y2+zTwo] = 'R';
    }

    if (y1 < x2){
        move = x2 - y1;
        r1 = x2 - y1;       
        
        int len = x1 + y1 + z1;

        for (int i = len; i >= len - z1; i--){
            s1[i] = s1[i-1];
        }

        for (int j = 0; j < move; j++){
            s1[x1+y1+j] = 'r';
        }

        //ptr = s1+(x1+y1);
        //memmove(ptr + move, ptr, strlen(ptr)+1);
        //for (int i = 0; i < move; i++){
        //    s1[x1+y1+i] = 'r';
        //}
    }

    if ((x1 + x2 + y2) < (x1 + y1 + r1 + z1)){
        int diff = (x1 + y1 + r1 + z1) - (x1 + x2 + y2);

        int len = x1 + x2 + y2 + z2 + diff;

        for (int i = len; i >= len - z2; i--){
             s2[i] = s2[i-diff];
         }

         for (int j = 0; j < diff; j++){
             s2[x1+x2+y2+j] = 'r';
         }


        //ptr = s2+(x1+x2+y2);
        //memmove(ptr + diff, ptr, strlen(ptr)+1);
        //for (int j = 0; j < diff; j++){
        //    s2[x1+x2+y2+j] = 'r';
        //}
    }
        
    
}

void display(char *heading, char *s1, char *s2){
    int rs1 = 0;
    int rs2 = 0;
    double rAvg = 0;
    int strLen = 0;
    int upR = 0;
    float bigR = 0.0;
    for (int i = 0; i<=1; i++){
        if (i == 0){
            strLen = strlen(s1);
        } else {
            strLen = strlen(s2);
        }

        for (int j = 0; j<strLen; j++){
            if (i == 0){
                if (s1[j] == 'r'){
                    rs1++;
                }
                if (s1[j] == 'R'){
                    upR++;
                }
            }
            if (i == 1){
                if (s2[j] == 'r'){
                     rs2++;
                 }
                 if (s2[j] == 'R'){
                     upR++;
                 }
            }
        }
    }

    if (strlen(s1) >= strlen(s2)){
        strLen = strlen(s1);
    } else {
        strLen = strlen(s2);
    }

    rAvg = (rs1 + rs2)/2.0;

    bigR = (double) upR / (double) strLen;

    printf("\n");
    if (heading != NULL)
        printf("%s", heading);
    printf("%s\n",s1);
    printf("%s\n",s2);
    printf("%d %d %.1f %.5f\n", rs1, rs2, rAvg, bigR);
}
