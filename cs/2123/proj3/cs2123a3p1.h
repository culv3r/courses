
typedef struct Node
{
	int iNum;			// Number
	struct Node *pNext;		// next node in the linked list
} Node;

/* prototypes */

Node * pqinsert(Node **ppHead, int iNum);
Node * pqmindelete(Node *ppHead);
Node *insertLL(Node **ppHead, int iNum);
Node *searchLL(Node *pHead, int iNum, Node **ppPrecedes);
Node *allocateNode(int iNum);
int getSimpleToken(char szInput[], int *piBufferPosition, char szToken[]);

/* global vars */

int iGCheck;				// Global Count
Node *pPrecedes;			// Preceding Node

// boolean
#define FALSE		0
#define TRUE 		1
