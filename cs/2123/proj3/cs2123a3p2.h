
typedef struct Node
{
	int iNum;			// Number
	struct Node *pNext;		// next node in the linked list
	struct Node *pPrev;		// previous node in the linked list
} Node;

/* prototypes */

Node * pqinsert(Node **ppHead, Node **ppEnd, int iNum, int bReverse);
Node * pqmindelete(Node *ppHead);
Node *insertLL(Node **ppHead, Node **ppEnd, int iNum, int bReverse);
Node *searchLL(Node *pHead, Node *pEnd, int iNum, Node **ppPrecedes, int bReverse);
Node *allocateNode(int iNum);
int getSimpleToken(char szInput[], int *piBufferPosition, char szToken[]);

/* global vars */

int iGCheck;				// Global Count
Node *pPrecedes;			// Preceding Node

// boolean
#define FALSE		0
#define TRUE 		1
