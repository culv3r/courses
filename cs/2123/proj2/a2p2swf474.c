#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* function prototypes */
int iterGCD(int x, int y);
int recGCD(int x, int y);

int main(int argc, char *argv[])
{
    int num_vals = 6;
    int x_vals[] = {3, 9, 12, 36, 1, 105};
    int y_vals[] = {3, 21, 18, 27, 12, 91};
    int i = 0;

    printf("Assignment 2 Problem 2 by Daniel Larsen.\n\n");

    while (i<num_vals)
    {
       printf("Iterative GCD: x = %d, y = %d, result = %d\n", x_vals[i], y_vals[i], iterGCD(x_vals[i],y_vals[i]));
       printf("Recursive GCD: x = %d, y = %d, result = %d\n\n", x_vals[i], y_vals[i], recGCD(x_vals[i],y_vals[i]));

        i++;
    }

    return 0;
}

int iterGCD(int x, int y)
{
        int iX, iY, iTemp;
    	iX = x;
	iY = y;

	if (iX == iY)
		return iY;
	if (iX < iY)
	{
		iTemp = iX;
		iX = iY;
		iY = iTemp;
	}
	while (iX != 0 && iX%iY != 0)
	{
	 	iY = iX%iY;
	}

    return iY;

}

int recGCD(int x, int y)
{
    if(y <= x && x%y == 0)
	return y;
    else if(x < y)
	recGCD(y,x);
    else
	recGCD(x,x%y);
    //return -1;
}
