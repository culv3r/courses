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

static char stateChars[] = {'r','R','w',0};

/* 1) handle state changes:
         running process completes CPU burst
         IO complete
   2) do context switch if necessary
         both ready
         one ready and CPU free
   3) append appropriate characters to character arrays
         avoid putting in multiple string terminators
*/
/* assume s1 and s2 point to buffers with enough space to hold the result */
/* assume that the int parameters are strictly greater than 0 */
void fcfs(char *s1, char *s2, int x1, int y1, int z1,
                              int x2, int y2, int z2) {
   int i;                                   /* next string position (time) */
   int state1 = READY;                            /* start with both ready */
   int state2 = READY;
   int cpuLeft1 = x1;                       /* P1 next CPU burst remaining */
   int cpuLeft2 = x2;                       /* P2 next CPU burst remaining */
   int ioLeft1 = y1;        /* P1 next IO burst remaining, 0 if no more IO */
   int ioLeft2 = y2;        /* P2 next IO burst remaining, 0 if no more IO */

   for (i=0; (state1 != DONE) || (state2 != DONE); i++) {
                                /* running process completes its CPU burst */
      if ((state1 == RUNNING) && (cpuLeft1== 0)) {
         if (ioLeft1 == 0) {
            state1 = DONE;
            s1[i] = stateChars[state1];            /* terminate the string */
         }
         else
            state1 = WAITING;
      }
      else if ((state2 == RUNNING) && (cpuLeft2 == 0) ) {
         if (ioLeft2 == 0) {
             state2 = DONE;
             s2[i] = stateChars[state2];
         } else {
             state2 = WAITING;
         }
      }
                                                     /* handle IO complete */
      if ((state1 == WAITING) && (ioLeft1 == 0)) {
          state1 = READY;
          cpuLeft1 = z1;
      }
      if ((state2 == WAITING) && (ioLeft2 == 0)) {
          state2 = READY;
          cpuLeft2 = z2;
      }
                                    /* if both ready, depends on algorithm */
      if ( (state1 == READY) && (state2 == READY)) {
          /* FCFS says that process 1 wins */
          state1 = RUNNING;
          state2 = READY;
      }
                                     /* handle one ready and CPU available */
      else if ( (state1 == READY) && (state2 != RUNNING)) {
         state1 = RUNNING;
      }
      else if ( (state2 == READY) && (state1 != RUNNING)) {
         state2 = RUNNING;
      }
   /* insert chars in string, but avoid putting in extra string terminators */
      if (state1 != DONE)
         s1[i] = stateChars[state1];
      if (state2 != DONE)
         s2[i] = stateChars[state2];
/* decrement counts */
      if (state1 == RUNNING)
         cpuLeft1--;
      if (state1 == WAITING)
         ioLeft1--;
      if (state2 == RUNNING)
         cpuLeft2--;
      if (state2 == WAITING)
         ioLeft2--;
   }                                               /* end of main for loop */
}

void sjf(char *s1, char *s2, int x1, int y1, int z1, int x2, int y2, int z2){
    int i;                                   /* next string position (time) */
   int state1 = READY;                            /* start with both ready */
   int state2 = READY;
   int cpuLeft1 = x1;                       /* P1 next CPU burst remaining */
   int cpuLeft2 = x2;                       /* P2 next CPU burst remaining */
   int ioLeft1 = y1;        /* P1 next IO burst remaining, 0 if no more IO */
   int ioLeft2 = y2;        /* P2 next IO burst remaining, 0 if no more IO */

   for (i=0; (state1 != DONE) || (state2 != DONE); i++) {
                                /* running process completes its CPU burst */
      if ((state1 == RUNNING) && (cpuLeft1== 0)) {
         if (ioLeft1 == 0) {
            state1 = DONE;
            s1[i] = stateChars[state1];            /* terminate the string */
         }
         else
            state1 = WAITING;
      }  
      else if ((state2 == RUNNING) && (cpuLeft2 == 0) ) {
         if (ioLeft2 == 0) {
             state2 = DONE;
             s2[i] = stateChars[state2];
         } else {
             state2 = WAITING;
         }
      }  
                                                     /* handle IO complete */
      if ((state1 == WAITING) && (ioLeft1 == 0)) {
          state1 = READY;
          cpuLeft1 = z1;
      }  
      if ((state2 == WAITING) && (ioLeft2 == 0)) {
          state2 = READY;
          cpuLeft2 = z2;
      }  
                                    /* if both ready, depends on algorithm */
      if ( (state1 == READY) && (state2 == READY)) {
          if (cpuLeft1 < cpuLeft2){
              state1 = RUNNING;
              state2 = READY;
          } else if (cpuLeft1 == cpuLeft2){
              state1 = RUNNING;
              state2 = READY;
          } else {
              state1 = READY;
              state2 = RUNNING;
          }
      }  
                    /* handle one ready and CPU available */
      else if ( (state1 == READY) && (state2 != RUNNING)) {
         state1 = RUNNING;
      }  
      else if ( (state2 == READY) && (state1 != RUNNING)) {
         state2 = RUNNING;
      }  
   /* insert chars in string, but avoid putting in extra string terminators */
      if (state1 != DONE)
         s1[i] = stateChars[state1];
      if (state2 != DONE)
         s2[i] = stateChars[state2];
                                                        /* decrement counts */
      if (state1 == RUNNING)
         cpuLeft1--;
      if (state1 == WAITING)
         ioLeft1--;
      if (state2 == RUNNING)
         cpuLeft2--;
      if (state2 == WAITING)
         ioLeft2--;
   }                                               /* end of main for loop */
}

void psjf(char *s1, char *s2, int x1, int y1, int z1, int x2, int y2, int z2){
    int i;                                   /* next string position (time) */
   int state1 = READY;                            /* start with both ready */
   int state2 = READY;
   int cpuLeft1 = x1;                       /* P1 next CPU burst remaining */
   int cpuLeft2 = x2;                       /* P2 next CPU burst remaining */
   int ioLeft1 = y1;        /* P1 next IO burst remaining, 0 if no more IO */
   int ioLeft2 = y2;        /* P2 next IO burst remaining, 0 if no more IO */

   for (i=0; (state1 != DONE) || (state2 != DONE); i++) {
                                /* running process completes its CPU burst */
      if ((state1 == RUNNING) && (cpuLeft1== 0)) {
         if (ioLeft1 == 0) {
            state1 = DONE;
            s1[i] = stateChars[state1];            /* terminate the string */
         }
         else
            state1 = WAITING;
      }  
      else if ((state2 == RUNNING) && (cpuLeft2 == 0) ) {
         if (ioLeft2 == 0){
             state2 = DONE;
             s2[i] = stateChars[state2];
         } else {
             state2 = WAITING;
         }
      }
      else if ( ((state1 == RUNNING) && (cpuLeft2 < cpuLeft1)) && cpuLeft2 != 0){
           state1 = READY;
           state2 = RUNNING;
       }
       else if ( ((state2 == RUNNING) && (cpuLeft1 < cpuLeft2)) && cpuLeft1 != 0){
           state1 = RUNNING;
           state2 = READY;
       }
                                                     /* handle IO complete */
      if ((state1 == WAITING) && (ioLeft1 == 0)) {
          state1 = READY;
          if (cpuLeft1 == 0)
            cpuLeft1 = z1;
      }  
      if ((state2 == WAITING) && (ioLeft2 == 0)) {
          state2 = READY;
          if (cpuLeft2 == 0)
              cpuLeft2 = z2;
      }  
                                    /* if both ready, depends on algorithm */
      if ( (state1 == READY) && (state2 == READY)) {
          if (cpuLeft1 < cpuLeft2){
              state1 = RUNNING;
              state2 = READY;
          } else if (cpuLeft1 == cpuLeft2){
              state1 = RUNNING;
              state2 = READY;
          } else {
              state1 = READY;
              state2 = RUNNING;
          }
      }  
                                     /* handle one ready and CPU available */
      else if ( (state1 == READY) && (state2 != RUNNING)) {
         state1 = RUNNING;
      }  
      else if ( (state2 == READY) && (state1 != RUNNING)) {
         state2 = RUNNING;
      }  
   /* insert chars in string, but avoid putting in extra string terminators */
      if (state1 != DONE)
         s1[i] = stateChars[state1];
      if (state2 != DONE)
         s2[i] = stateChars[state2];
                                                        /* decrement counts */
      if (state1 == RUNNING)
         cpuLeft1--;
      if (state1 == WAITING)
         ioLeft1--;
      if (state2 == RUNNING)
         cpuLeft2--;
      if (state2 == WAITING)
         ioLeft2--;
   }                                               /* end of main for loop */
}

void rr(char *s1, char *s2, int q, int x1, int y1, int z1, int x2, int y2, int z2){
    int i;                                   /* next string position (time) */
   int state1 = READY;                            /* start with both ready */
   int state2 = READY;
   int cpuLeft1 = x1;                       /* P1 next CPU burst remaining */
   int cpuLeft2 = x2;                       /* P2 next CPU burst remaining */
   int ioLeft1 = y1;        /* P1 next IO burst remaining, 0 if no more IO */
   int ioLeft2 = y2;        /* P2 next IO burst remaining, 0 if no more IO */
   int qleft;                                         /* quantum remaining */

   for (i=0; (state1 != DONE) || (state2 != DONE); i++) {
                                /* running process completes its CPU burst */
      if ((state1 == RUNNING) && (cpuLeft1 == 0)) {
         if (ioLeft1 == 0) {
            state1 = DONE;
            s1[i] = stateChars[state1];            /* terminate the string */
         }
         else{
             state1 = WAITING;
         }
      }  
      else if ((state2 == RUNNING) && (cpuLeft2 == 0 ) ) {
         if (ioLeft2 == 0) {
             state2 = DONE;
             s2[i] = stateChars[state2];
         } else {
             state2 = WAITING;
         }
      }  
                                     /* running process has quantum expire */
      if ((state1 == RUNNING) && (qleft == 0) ) {
         if (!(cpuLeft2<=0)){
            state1 = READY;
            state2 = RUNNING;
            qleft = q;
         }
      }  
      if ((state2 == RUNNING) && (qleft == 0) ) {
         if (!(cpuLeft1<=0)){
            state1 = RUNNING;
            state2 = READY;
            qleft = q;
         }
      }  
                                                     /* handle IO complete */
      if ((state1 == WAITING) && (ioLeft1 == 0)) {
          state1 = READY;
          cpuLeft1 = z1;
      }  
      if ((state2 == WAITING) && (ioLeft2 == 0)) {
          state2 = READY;
          cpuLeft2 = z2;
      }  
                                    /* if both ready, depends on algorithm */
      if ( (state1 == READY) && (state2 == READY)) {
          state1 = RUNNING;
          state2 = READY;
          qleft = q;
      }  
                                     /* handle one ready and CPU available */
      else if ( (state1 == READY) && (state2 != RUNNING)) {
         state1 = RUNNING;
      }  
      else if ( (state2 == READY) && (state1 != RUNNING)) {
         state2 = RUNNING;
      }  
   /* insert chars in string, but avoid putting in extra string terminators */
      if (state1 != DONE)
         s1[i] = stateChars[state1];
      if (state2 != DONE)
         s2[i] = stateChars[state2];
      
      //printf("QLeft: %d, CPULeft1: %d, CPULeft2: %d, ioLeft1: %d, ioLeft2: %d\n", qleft, cpuLeft1, cpuLeft2, ioLeft1, ioLeft2);
      /* decrement counts */
      qleft--;                   /* OK to decrement even if nothing running */
      //printf("Q Left: %d\n", qleft);
      if (state1 == RUNNING)
         cpuLeft1--;
      if (state1 == WAITING)
         ioLeft1--;
      if (state2 == RUNNING)
         cpuLeft2--;
      if (state2 == WAITING)
         ioLeft2--;
   }                                               /* end of main for loop */
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
