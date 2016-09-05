/***************************************************************************
a2p1swf474.c
Assignment 2 - Part 1 - "Stacks in C"
***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0
#define EMPTY -99999

typedef struct
{
   int iElement;      //Integer in stack
} Stack;

/** prototype functions **/
void display(Stack * stack);
void stkPush(Stack * stack, int iElement);
void stkPop(Stack * stack);
int stkEmpty(Stack * stack);
int stkFull(Stack * stack);

int iTop;
int iSize;

int main (int argc, char *argv[])
{
	FILE *pfileData;
	int iElement;
	char szInputBuffer[100];

	iTop = 0;
	iSize = 10;

	printf("Assignment 2 Problem 1 by Daniel Larsen\n");

	pfileData = fopen("data_a2.txt", "r");

	if (pfileData == NULL)
	{
		printf("ERROR: No Data File Found\n");
		return -1;
	}

	Stack * stack = (Stack *) malloc(sizeof(Stack) * iSize);

	if (stack == NULL)
	{
		printf("ERROR: Memory not allocated!\n");
		return -1;
	}

	while (fgets(szInputBuffer, 100, pfileData) != NULL)
	{
		sscanf(szInputBuffer, "%d", &iElement);
		if (iElement > -99999)
			stkPush(stack, iElement);
		else
			stkPop(stack);
	}

	return 0;

}

void stkPush (Stack * stack, int iElement)
{
	if (stkFull(stack) == TRUE)
	{
		stack = (Stack *) realloc(stack, sizeof(Stack) * (iSize + 10));
		printf("Stack capacity has grown from %d elements to %d elements.\n", iSize, (iSize+10));
		(*(stack + iTop)).iElement = iElement;
		iTop++;
		iSize += 10;
	}
	else
	{
		(*(stack + iTop)).iElement = iElement;
		iTop++;
	}
}

void stkPop (Stack * stack)
{
	if (stkEmpty(stack) == TRUE)
	{
		printf("Error: No elements available to pop.\n");
	}
	else
	{
		int iPopped = (*(stack + iTop - 1)).iElement;
		(*(stack + (iTop - 1))).iElement = EMPTY;
		iTop--;
		printf("# elements after popping: %d \t\t integer popped: %d\n", iTop, iPopped);
	}
}

int stkEmpty(Stack * stack)
{
	if (iTop == 0 && (*(stack + iTop)).iElement == EMPTY)
		return TRUE;
	else
		return FALSE;
}

int stkFull(Stack * stack)
{
	if (iTop == iSize)
		return TRUE;
	return FALSE;
}


