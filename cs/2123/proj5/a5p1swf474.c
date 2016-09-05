#include <stdio.h>
#include <stdlib.h>

//Prototypes
int insertSort(int arr[], int iElements);

int main(int argc, char *argv[])
{
FILE *pfileData;
char szInputBuffer[100];
int iValue, iSize, iElements, iAssign, i;
int arr[500];
printf("Assignment 5 Part 1 by Daniel Larsen.\n\n");
pfileData = fopen("data_a5.txt", "r");

iValue = 0;
iElements = 0;
iAssign = 0;

if (pfileData == NULL)
{
	printf("Error: File does not Exist!");
	exit(-1);
}

while (fgets(szInputBuffer, 100, pfileData) != NULL)
{
	sscanf(szInputBuffer, "%d", &iValue);
	arr[iElements] = iValue;
	iElements++;
}

iAssign = insertSort(arr,iElements);

printf("# of Assignments: %d\n", iAssign);

//for (i = 0; i<iElements-1; i++)
//{
//	printf("%d, ", arr[i]);
//}

printf("\n");

fclose(pfileData);


}

int insertSort (int arr[], int iElements)
{
	int i = 0;
	int iKey,j;
	int iAssign = 0;
	iKey = 0;

	for (i = 1; i < iElements - 1; i++)
	{
		iKey = arr[i];
		j = i - 1;

		while(j >= 0 && iKey < arr[j])
		{
			arr[j+1] = arr[j];
			j--;
			iAssign++;
		}
		arr[j+1] = iKey;
		iAssign++;
	}
	return iAssign;
}


