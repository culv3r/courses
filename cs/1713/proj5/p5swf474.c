/**********************************************************************
p5swf474.c by Daniel Larsen
Purpose:
    This program reads book information and a command file.   It 
    processes the commands against the book information.
    This is the driver code for the routines written by the students.
Command Parameters:
    p5 -b bookFileName -c commandFileName
Input:
    Book	same as Programming Assignment #2 although there is different data.

    Command	This is different from the previous assignment.  The file 
            contains  text in the form of commands.  
        CUSTOMER BEGIN szBookId szCustomerId szCheckedOutDt dLateFeePerDay dMaxLateFee szTitle  
            specifies the beginning of customer request and includes all the 
            identification information from program 2.
        CUSTOMER ADDRESS szStreetAddress,szCity,szStateCd,szZipCd
            specifies the address for a customer (separated by commas)
        CUSTOMER TRANS cTransType    szBookId   szTransDt
            specifies a single book transaction.  Steps:
            -	Print the Transaction Type, Book Id, and Transaction Date 
            -	Lookup the book ID using a binary search.  If not found, print a 
                warning (but do not terminate your program) and return.
            -	If the transaction date is invalid, show a message stating "invalid date", 
                but do not terminate.  Use the validateDate  function.
            -	If the transaction type is C (meaning checkout):
                o	If the book is already checked out, show a message stating 
                    "already checked out", but do not terminate.
                o	Otherwise, check out the book to this customer, setting the book's 
                    customer ID.  The book's checked out date needs to be set to the 
                    transaction's date.
            -	If the transaction type is R (meaning return):
                o	Use dateDiff to subtract the book's szCheckOutDt from the transaction's szTransDt
                o	If the difference is more than 14:
                    --	Determine the number of days late by subtracting 14 from that difference.  
                    --	Compute the late fee by multiplying the number of days late 
                        by the book's dLateFeePerDay.
                    --	If that late fee is greater than the book's dMaxLateFee, 
                        change it to dMaxLateFee.
                    --  Print the late fee.
                    --	Add the computed late fee to the customer's dFeeBalance
                o	Set the book's customer ID to "NONE".
                o	Set the book's check out date to "0000-00-00".
        CUSTOMER COMPLETE
            specifies the completion   of a customer.  Print the total fees for this customer.
        BOOK CHANGE szBookId dLateFeePerDay dMaxLateFee
            change the Late Fee Per Day and Max Late Fee to the specified values
        BOOK SHOW szBookId    
            requests a display of a particular book.  Show all of its information.
               
Results:
    Prints the Books prior to sorting
    Prints the Books after sorting.
    Processes the commands (see above) and shows any errors.
    Prints the resulting Books
Returns:
    0  normal
    -1 invalid command line syntax
    -2 show usage only
    -3 error during processing, see stderr for more information

Notes:
    p5 -?       will provide the usage information.  In some shells,
                you will have to type p5 -\?

**********************************************************************/

// If compiling using visual studio, tell the compiler not to give its warnings
// about the safety of scanf and printf
#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cs1713p5.h"

//Global Variables
double dTotalFee = 0;
Customer customer;
NodeT *pRoot;

/******************** printBooks **************************************
    void printBooks(char *pszHeading, Node *pHead)
Purpose:
    Prints each book in a table format
Parameters:
    I char *pszHeading    Heading to print before printing the table of books
    I NodeT *pRoot        Binary Tree of Books
Returns:
    n/a
Notes:
    
**************************************************************************/
void printBooks(char *pszHeading, NodeT *pRoot)
{
    printf("\n%s\n", pszHeading);

    // output column headings
    printf("   %-9s %-40s %-8s %-10s %-8s %-8s \n"
        , "Book Id", "Title", "Customer", "Ck Out Dt", "Late Fee", "Max Late");

    //send tree root to recursive printing function.
    printInOrder(pRoot);
}


/********************processCustomerCommand *****************************
    void processCustomerCommand(NodeT *pRoot, char *pszSubCommand
     , char *pszRemainingInput, Customer *pCustomer)
Purpose:
    Processes the subcommands associated with the CUSTOMER command:
        CUSTOMER BEGIN szBookId szCustomerId szCheckedOutDt dLateFeePerDay dMaxLateFee szTitle  
            specifies the beginning of customer request and includes all the identification information from program 2.
        CUSTOMER ADDRESS szStreetAddress,szCity,szStateCd,szZipCd
            specifies the address for a customer (separated by commas)
        CUSTOMER TRANS cTransType    szBookId   szTransDt
            specifies a single book transaction.  Steps:
            -	Print the Transaction Type, Book Id, and Transaction Date 
            -	Lookup the book ID using a binary search.  If not found, print a warning (but do not terminate your program) and return.
            -	If the transaction date is invalid, show a message stating "invalid date", but do not terminate.  Use the validateDate  function.
            -	If the transaction type is C (meaning checkout):
                o	If the book is already checked out, show a message stating "already checked out", but do not terminate.
                o	Otherwise, check out the book to this customer, setting the book's customer ID.  The book's checked out date needs to be set to the transaction's date.
            -	If the transaction type is R (meaning return):
                o	Use dateDiff to subtract the book's szCheckOutDt from the transaction's szTransDt
                o	If the difference is more than 14:
                    --	Determine the number of days late by subtracting 14 from that difference.  
                    --	Compute the late fee by multiplying the number of days late by the book's dLateFeePerDay.
                    --	If that late fee is greater than the book's dMaxLateFee, change it to dMaxLateFee.
                    --  Print the late fee.
                    --	Add the computed late fee to the customer's dFeeBalance
                o	Set the book's customer ID to "NONE".
                o	Set the book's check out date to "0000-00-00".
        CUSTOMER COMPLETE
            specifies the completion   of a customer.  Print the total fees for this customer.


Parameters:
    I/O NodeT *pRoot                 Binary Tree of Books
    I   char  *pszSubCommand          sscanf segment of command input
    I   char  *pzRemainingInput       Points to the remaining characters in the input
                                      line (i.e., the characters that following the
                                      subcommand).
    I/O Customer *pCustomer           The BEGIN subcommand begins a new customer.  
Notes:

**************************************************************************/
void processCustomerCommand(NodeT *pRoot, char *pszSubCommand
    , char *pszRemainingInput, Customer *pCustomer)
{
    int iscanfCnt;
    Transaction transaction;
    double dTransactionFee = 0;
    // Determine what to do based on the subCommand
    if (strcmp(pszSubCommand, "BEGIN") == 0)
    {
	customer = *pCustomer;
        // get the Customer Identification Information
	iscanfCnt = sscanf(pszRemainingInput, "%6s %lf %40s %30[^\n]"
                         , customer.szCustomerId
                         , &customer.dFeeBalance
                         , customer.szEmailAddr
                         , customer.szFullName);
        // Check for bad customer identification data
        if (iscanfCnt < 4)
            exitError(ERR_CUSTOMER_ID_DATA, pszRemainingInput);
	
	dTotalFee += customer.dFeeBalance;

    }
    else if (strcmp(pszSubCommand, "COMPLETE") == 0)
    {
        // print the customer's total fees
        printf("\t\t\t\t\t\t\t\t\tTotal Fees = %3.2lf\n", dTotalFee);
        dTotalFee = 0;

    }
    else if (strcmp(pszSubCommand, "ADDRESS") == 0)
    {
        // get the postal address and print it
        iscanfCnt = sscanf(pszRemainingInput, "%30[^,],%20[^,],%2s,%5s"
                                                        , customer.szStreetAddress
                                                        , customer.szCity
                                                        , customer.szStateCd
                                                        , customer.szZipCd);
	if (iscanfCnt != 4)//Error Handling
        	exitError(ERR_CUSTOMER_ADDRESS_DATA, pszRemainingInput);


	//print second line of  address information.
			printf("******************** Library Receipt ***********************\n");
		        printf("%6s %s %s (Previously Owed: %3.2lf)\n%s \n%s %2s %5s\n"
                	        , customer.szCustomerId
                     		, customer.szEmailAddr
                     		, customer.szFullName
                     		, customer.dFeeBalance
				, customer.szStreetAddress
                                , customer.szCity
                                , customer.szStateCd
                                , customer.szZipCd);


        // print the column heading for the transactions
        printf("\t\t\t\t\t%-5s  %-9s  %-10s\n", "Trans", "Book", "Date");

    }
    else if (strcmp(pszSubCommand, "TRANS") == 0)
    {
	iscanfCnt = sscanf(pszRemainingInput, "%c %9s %10s"
                           , &transaction.cTransType
                           , transaction.szBookId
                           , transaction.szTransDt);

        if (iscanfCnt != 3)//Error Handling
        	exitError(ERR_TRANSACTION_DATA, pszRemainingInput);

        dTransactionFee = processTransaction(pRoot, customer, transaction);

        //process transaction details
        printf("\t\t\t\t\t%c   %-9s %-10s"
               , transaction.cTransType
               , transaction.szBookId
               , transaction.szTransDt);

        //If there is a fee, print the fee.
        if (dTransactionFee > 0.01)
        {
        	printf("\t  Late Fee = %3.2lf\n", dTransactionFee);
                dTotalFee = dTotalFee + dTransactionFee;
        }

        //Error Handling
        else if (dTransactionFee == -1.0)
        	printf(" *** Book Not Found\n");
        else if (dTransactionFee == -2.0)
                printf(" *** Already Checked Out\n");
        else if (dTransactionFee == -3.0)
                printf(" *** Invalid Date\n");
        //If there is no fee, or error, just print a new line.
        else
     	   printf("\n");

    }
    else printf("   *** %s %s\n", ERR_CUSTOMER_SUB_COMMAND, pszSubCommand);
}

/********************processBookCommand *****************************
    void processBookCommand(NodeT **ppRoot, char *pszSubCommand
         , char *pszRemainingInput)
Purpose:
    Processes the subcommands associated with the BOOK command:
        BOOK CHANGE szBookId dLateFeePerDay dMaxLateFee
            change the Late Fee Per Day and Max Late Fee to the specified values
        BOOK SHOW szBookId    
            requests a display of a particular book.  Show all of its information.
                
Parameters:
    I/O NodeT **ppRoot                Binary Tree of Books
    I   char  *pszSubCommand          sscanf segment of command input
    I   char  *pzRemainingInput       Points to the remaining characters in the input
                                      line (i.e., the characters that following the
                                      subcommand).
Notes:

**************************************************************************/
void processBookCommand(NodeT **ppRoot, char *pszSubCommand
				, char *pszRemainingInput)
{
    Book book;

    int iScanfCnt;

    iScanfCnt = 0;

    NodeT *p;

    //Determine what to do based upon the sub command
    if(strcmp(pszSubCommand, "CHANGE") == 0)
    {
	iScanfCnt = sscanf(pszRemainingInput, "%9s %lf %lf"
				, book.szBookId
				, &book.dLateFeePerDay
				, &book.dMaxLateFee);
	if (iScanfCnt != 3)
		exitError(ERR_BOOK_DATA, pszRemainingInput);

    	p = searchT(*ppRoot, book.szBookId);
	
	if (p == NULL)
		printf("%s -- %s \n", ERR_BOOK_NOT_FOUND, book.szBookId);
	else
	{
	p->book.dLateFeePerDay = book.dLateFeePerDay;
	p->book.dMaxLateFee = book.dMaxLateFee;
	}
    }

    else if(strcmp(pszSubCommand, "SHOW") == 0)
    {
    	iScanfCnt = sscanf(pszRemainingInput, "%9s"
                                , book.szBookId);
        if (iScanfCnt != 1)
                exitError(ERR_BOOK_DATA, pszRemainingInput);

    	p = searchT(*ppRoot, book.szBookId);

        if (p == NULL)
                printf("%s -- %s \n", ERR_BOOK_NOT_FOUND, book.szBookId);
	else
	{
	        printf("   %-9s %-40s %-8s %-10s %-8s %-8s \n"
	        , "Book Id", "Title", "Customer", "Ck Out Dt", "Late Fee", "Max Late");

       		printf("   %-9s %-40s %-8s %-10s %8.2lf %8.2lf \n"
	            , p->book.szBookId
	            , p->book.szTitle
        	    , p->book.szCustomerId
	            , p->book.szCheckedOutDt
	            , p->book.dLateFeePerDay
        	    , p->book.dMaxLateFee);
	}
    }
    else if(strcmp(pszSubCommand, "NEW") == 0)
    {
    	iScanfCnt = sscanf(pszRemainingInput, "%9s %6s %10s %lf %lf %[^\n]\n"
            , book.szBookId
            , book.szCustomerId
            , book.szCheckedOutDt
            , &book.dLateFeePerDay
            , &book.dMaxLateFee
            , book.szTitle);
	if (iScanfCnt < 6)
		exitError(ERR_BOOK_DATA, pszRemainingInput);

    	p = searchT(*ppRoot, book.szBookId);

	if (p != NULL)
		printf("%s -- %s \n", ERR_BOOK_ALREADY_EXISTS, book.szBookId);
	else
		{
			p = insertT(*ppRoot, book);
		}
	
    }
    else if(strcmp(pszSubCommand, "PPRINT") == 0)
    {
	prettyPrintT(*ppRoot,0);
    }
    else printf("   *** %s %s\n", ERR_BOOK_SUB_COMMAND, pszSubCommand);

}

/******************** processTransaction *****************************
double processTransaction(NodeT *pRoot, Customer customer, Transaction transaction)
Purpose:
    Processes Transaction Data and (if needed) modifies the linked list data.
Parameters:
    I NodeT *pRoot            Binary Tree of Books
    I Customer customer       Customer data structure access
    I Transaction transaction Transaction data structure access
Notes:
    1. Data that reaches this function has validated via basic error handling.
    2. If data is in conflict with database, following will return:
               -1: Book ID Not Found
               -2: Book Already Checked Out
               -3: Date Invalid
*********************************************************************/

double processTransaction(NodeT *pRoot, Customer customer, Transaction transaction)
{
 UtsaDate utsadate;     //UtsaDate Structure
 UtsaDate *pDate;       //Pointer for Struct to pass to validateDate()
 pDate = &utsadate;
 NodeT *p;
 

 //Return address of Node containing book or NULL
 p = searchT(pRoot, transaction.szBookId);

 //Error handling -1.0 -> Invalid Book
 if (p == NULL)
        return -1.0;

 //Error Handling for Date
 if (validateDate(transaction.szTransDt, pDate) != 0)
        return -3.0;

 //Error handling for book already checked out.
 if (transaction.cTransType == 'C' && strcmp(p->book.szCheckedOutDt,"0000-00-00") != 0 && strcmp(p->book.szCustomerId,"NONE") != 0)
        return -2.0;
 //Verifies book record is valid to check out.
 if (transaction.cTransType == 'C' && strcmp(p->book.szCheckedOutDt,"0000-00-00") == 0 && strcmp(p->book.szCustomerId,"NONE") == 0)
 {
        strcpy(p->book.szCustomerId,customer.szCustomerId);
        strcpy(p->book.szCheckedOutDt, transaction.szTransDt);
 }

 //Return Book statement
 if (transaction.cTransType == 'R')
 {
        int iDays = 0;          //Tracks date difference between checkout and transaction.
        double dFee = 0.00;     //Return variable for fees (if any)

        //Set iDays to # of days checked out and then subtract for two weeks.
        iDays = dateDiff(transaction.szTransDt,p->book.szCheckedOutDt) - 14;

	//If days is less than 0, book isn't overdue, no fees needed
        //Check book in and return.
        if (iDays < 0)
        {
                strcpy(p->book.szCustomerId,"NONE");
                strcpy(p->book.szCheckedOutDt,"0000-00-00");
                return 0.00;
        }

        //If function reaches this point, book is overdue
        //Set fee below
        dFee = iDays * p->book.dLateFeePerDay;

        //If fee is too large, set fee as max fee.
        if (dFee > p->book.dMaxLateFee)
                dFee = p->book.dMaxLateFee;

        //Turn book in and return fee.
        strcpy(p->book.szCustomerId,"NONE");
        strcpy(p->book.szCheckedOutDt,"0000-00-00");
        return dFee;
 }
}

/************************* insertT *********************************
Node *insertT(NodeT *p, Book book)
Purpose:
    This function is used to insert a new node into the existing
    binary tree.
Parameters:
    I NodeT *p            Binary Tree of books
    I Book book		  Book information to be inserted
Notes:
*********************************************************************/
NodeT *insertT(NodeT *p, Book book)
{
        //Base case, if it's null, you've reached the end, create new node.
	if (p == NULL)
		return allocateNodeT(book);

	//If it's equal, return the node as it exists.
	if (strcmp(book.szBookId,p->book.szBookId) == 0)
		return p;

	//Recurse through the tree till the base condition is met or the second
	//condition is met.
	else if (strcmp(book.szBookId,p->book.szBookId) < 0)
	{
		p->pLeft = insertT(p->pLeft, book);
		return p;
	}
	else
	{
		p->pRight = insertT(p->pRight, book);
	}
	return p;
}

/************************* searchLL *********************************
Node *searchT(NodeT *p, char szMatchBookId[])
Purpose:
    Searches the binary tree to determine if a node exists.
Parameters:
    I NodeT *p                  Binary Tree of Books
    I char szMatchBookId[]      Character Array to match against
Notes:
*********************************************************************/
NodeT *searchT(NodeT *p, char szMatchBookId[])
{

	//Base Case
	if (p == NULL)
		return NULL;

	//You've found the node, return
	if (strcmp(p->book.szBookId, szMatchBookId) == 0)
		return p;

	//Recursively traverse the tree till null is reached or book is found.
	if (strcmp(p->book.szBookId, szMatchBookId) > 0)
		return searchT(p->pLeft, szMatchBookId);
	else
		return searchT(p->pRight, szMatchBookId);
 }

/************************* printInOrder *****************************
void printInOrder(NodeT *p)
Purpose:
    Recursive function to print out the information in order.
Parameters:
    I NodeT *p            Binary Tree of books
Notes:
*********************************************************************/
void printInOrder(NodeT *p)
{
	//Base case..
	if (p == NULL)
		return;

	//Traverse tree and start from lowest value
	printInOrder(p->pLeft);
	
	//Print node information
	printf("   %-9s %-40s %-8s %-10s %8.2lf %8.2lf \n"
        , p->book.szBookId
        , p->book.szTitle
        , p->book.szCustomerId
        , p->book.szCheckedOutDt
        , p->book.dLateFeePerDay
        , p->book.dMaxLateFee
        );
	printInOrder(p->pRight);

}

/********************** prettyPrintT ********************************
void prettyPrintT(NodeT *p, int iIndent)
Purpose:
    Recursive function that prints out the tree structure.
Parameters:
    I  NodeT *p       Node currently being processed.
   I/O int iIndent    Integer used to track indents.
********************************************************************/

void prettyPrintT(NodeT *p, int iIndent)
{
 //local variable initalization
 int i;

 //base case
 if (p == NULL)
	return;

 //traverse tree to "largest" values and print from top to bottom.
 prettyPrintT(p->pRight,iIndent+1);

 //for loop that creates the indent space.
 for (i=0;i<iIndent; i++)
	printf("     ");

 //Prints book id's
 printf("%s \n", p->book.szBookId);

 prettyPrintT(p->pLeft,iIndent+1);


}

