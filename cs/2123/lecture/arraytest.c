#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){

int a[2][3]={10,20,30,15,25,35};
int i,j;

for (i=0; i<2; i++){
	for(j=0; j<3; j++)
		printf("%d ",a[i][j]);
	printf("\n");
}
}
