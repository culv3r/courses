#include <stdio.h>
#include <time.h>
#include <string.h>

void function_to_time(void);

int main(void) {
   char end[255];
   char start[255];
   time_t tend, tstart;  

   tstart = time(NULL);
   function_to_time();
   tend = time(NULL);

   ctime_r(&tstart,start);
   ctime_r(&tend,end);

   //The time before was Thu Sep  7 17:46:53 2017
   printf("The time before was %sThe time after  was %s", start,end);
   return 0;
}
