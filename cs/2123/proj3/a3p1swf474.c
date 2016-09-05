#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cs2123a3p1.h"

int iLCheck = 0;

int main (int argc, char *argv[])
{
	FILE *pfileData;
	char szInputBuffer[100];
	char szCommand[21];
	char szSubCommand[21];
	int iNum, iScanfCnt, iBufferPosition, bGotToken;
	Node *pCurrNode;
	Node *pHead = NULL;
	Node *p, *ptr;

	printf("Assignment 3 Problem 1 by Daniel Larsen.\n\n");

	pfileData = fopen("data_a3.txt", "r");

	iBufferPosition = 0;

	if (pfileData == NULL)
	{
		printf("Error: File Does Not Exist!\n");
		exit(-1);
	}

	while (fgets(szInputBuffer, 100, pfileData) != NULL)
	{
		iBufferPosition = 0;
		bGotToken = getSimpleToken(szInputBuffer, &iBufferPosition, szCommand);

		if (bGotToken && strcmp(szCommand, "REMOVE") == 0)
		{
			pHead = pqmindelete(pHead);
			printf("Queue contains:");
			for (pCurrNode = pHead; pCurrNode != NULL; pCurrNode = pCurrNode->pNext){
				printf(" %d", pCurrNode->iNum);
			}
			printf("\n");
		}
		else if (strcmp(szCommand, "INSERT") == 0){
			bGotToken = getSimpleToken(szInputBuffer, &iBufferPosition, szSubCommand);

			if (bGotToken)
			{
				sscanf(szSubCommand, "%d", &iNum);
				pqinsert(&pHead,iNum);
			}

			printf("Queue contains:");
			for (pCurrNode = pHead; pCurrNode != NULL; pCurrNode = pCurrNode->pNext){
				printf(" %d", pCurrNode->iNum);
			}
			printf("\n");
		}
		else
		{
			printf("Error: Invalid Command or Syntax.\n");
			printf("szCommand: %s", szCommand);
			printf("iNum: %d\n", iNum);
		}
	}

	fclose(pfileData);
	printf("Total checks: %d\n", iGCheck);

	for (p = pHead; p != NULL; p = ptr)
	{
		ptr = p->pNext;
		free(p);
	}
}

Node *pqinsert(Node **ppHead, int iNum)
{
	Node *pNew;
	Node *pFind = searchLL(*ppHead, iNum, &pPrecedes);

	if (pFind != NULL)
	{
		if (pPrecedes != NULL)
		{
			pPrecedes->pNext = pFind->pNext;
		}else{
			pFind->pNext = *ppHead;
			*ppHead = pFind;
		}
		free(pFind);
	}else{
		pNew = allocateNode(iNum);
		if (pPrecedes == NULL){
			pNew->pNext = *ppHead;
			*ppHead = pNew;
		}else{
			pNew->pNext = pPrecedes->pNext;
			pPrecedes->pNext = pNew;
		}
	}
	printf("Insert into queue. Checked %d nodes.\n", iLCheck);
}

Node *searchLL(Node *pHead, int iNum, Node **ppPrecedes)
{
        Node *pCurrNode; 
	iLCheck = 0;
        *ppPrecedes = NULL; //assign NULL to precedent node
        for (pCurrNode = pHead; pCurrNode != NULL; pCurrNode = pCurrNode->pNext){
		iGCheck++;
		iLCheck++;
                if (pCurrNode->iNum == iNum)
                        return pCurrNode;
                else if(pCurrNode->iNum > iNum)
		{
                        return NULL;
		}
                *ppPrecedes = pCurrNode; 
        } 
        return NULL;//not found
 }

Node *pqmindelete(Node *pHead)
{
	Node *pNewHead;
	pNewHead = pHead->pNext;
	free(pHead);
	return pNewHead;
}

Node * allocateNode(int iNum)
{
	Node *pNew;
	pNew = malloc(sizeof(Node));
	if (pNew == NULL)
	{
		printf("ERROR: Memory Not Allocated!\n");
		exit(-1);
	}
	pNew->iNum = iNum;
	pNew->pNext = NULL;
	return pNew;
}

int getSimpleToken(char szInput[], int *piBufferPosition, char szToken[]) 
{
    int iDelimPos;                      // found position of delim
    int iCopy;                          // number of characters to copy
    char szDelims[20] = " \n";          // delimiters
    
    // check for past the end of the string
    if (*piBufferPosition >= (int) strlen(szInput))
    {
        szToken[0] = '\0';              // mark it empty
        return FALSE;                   // no more tokens
    }

    // get the position of the first delim, relative to the iBufferPosition
    iDelimPos = strcspn(&szInput[*piBufferPosition], szDelims);

    // see if we have more characters than target token, if so, trunc
    if (iDelimPos > 20)
        iCopy = 20;             // truncated size
    else
        iCopy = iDelimPos;
    
    // copy the token into the target token variable
    memcpy(szToken, &szInput[*piBufferPosition], iCopy);    
    szToken[iCopy] = '\0';              // null terminate

    // advance the position
    *piBufferPosition += iDelimPos + 1;  
    return TRUE;
}

