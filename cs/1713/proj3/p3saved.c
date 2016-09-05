/**********************************************************************
p3swf474.c by Daniel Larsen
Purpose:
    This program reads book information and a command file.   It 
    processes the commands against the book information.
    This is the driver code for the routines written by the students.
Command Parameters:
    p3 -b bookFileName -c commandFileName
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
    p3 -?       will provide the usage information.  In some shells,
                you will have to type p3 -\?

**********************************************************************/

// If compiling using visual studio, tell the compiler not to give its warnings
// about the safety of scanf and printf
#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cs1713p3.h"

//Global Variable
double dTotalFee = 0;

/******************** getBooks **************************************
    int getBooks(Book bookM[], char * pszBookFileName)
Purpose:
    Retrieves the books from the file specified..
Parameters:
    O   Book bookM[]          an array which is used for returning the books.
    I   char *pszBookFileName name of the file containing the book data
Returns:
    A count of the number of read.
Notes:
    1. This opens the file based on the specified file name. 
    2. The data in the data file is in this format
    szBookId szCustomerId szCheckedOutDt dLateFeePerDay dMaxLateFee szTitle
    9s        6s             10s           lf            lf          40s (may contain spaces)
**************************************************************************/

int getBooks(Book bookM[], char * pszBookFileName)
{
    char szInputBuffer[100];		// input buffer for reading data
    int i = 0;                      // subscript in bookM
    int iScanfCnt;                  // returned by sscanf
    FILE *pFileBook;              // Stream Input for Books data. 

    /* open the Books stream data file */
    if (pszBookFileName == NULL)
        exitError(ERR_MISSING_SWITCH, "-b");

    pFileBook = fopen(pszBookFileName, "r");
    if (pFileBook == NULL)
        exitError(ERR_BOOK_FILENAME, pszBookFileName);

    /* get book information until EOF
    ** fgets returns null when EOF is reached.
    */

    while(fgets(szInputBuffer, 100, pFileBook) != NULL)
    {
    	if (i >= MAX_BOOKS)
		exitError(ERR_TOO_MANY_BOOKS, szInputBuffer);
	iScanfCnt = sscanf(szInputBuffer, "%9s %6s %10s %lf %lf %[^\n]\n"
		, bookM[i].szBookId
		, bookM[i].szCustomerId
		, bookM[i].szCheckedOutDt
		, &bookM[i].dLateFeePerDay
		, &bookM[i].dMaxLateFee
		, bookM[i].szTitle);

	// It should have populated all six variables, if not exit with
	// an error
	if (iScanfCnt < 6)
		exitError(ERR_BOOK_DATA, szInputBuffer);
	i++;
    }
    fclose(pFileBook);
    return i;
}

/******************** sortBooks **************************************
    void sortBooks(Book bookM[], int iBookCnt)
Purpose: Takes an array of books and sorts them by book ID.

Parameters:
    O   Book bookM[]            Array of books to be sorted
    I   int iBookCnt            Number of elements in array
Returns:
    No Direct Returns are Made.
Notes:
    
**************************************************************************/
void sortBooks(Book bookM[], int iBookCnt)
{
	int i, j;
	int bChange;
	Book bookTemp;

	for (i = 0; i < (iBookCnt - 1); i++)
	{
		bChange = FALSE;
		for (j = 0; j<(iBookCnt - i - 1); j++)
		{
			if (strcmp(bookM[j+1].szBookId, bookM[j].szBookId) < 0)
			{
				bookTemp = bookM[j+1];
				bookM[j+1] = bookM[j];
				bookM[j] = bookTemp;
				bChange = TRUE;
			}
		}
		if (! bChange)
		   break;
	}
}

/******************** printBooks **************************************
    void printBooks(char *pszHeading, Book bookM[], int iBookCnt)
Purpose:
    Prints each book in a table format
Parameters:
    I char *pszHeading    Heading to print before printing the table of books
    I Book bookM[]        Array of books
    I int iBookCnt        Number of elements in the bookM array.
Returns:
    n/a
Notes:
    
**************************************************************************/
void printBooks(char *pszHeading, Book bookM[], int iBookCnt)
{
    int i;
    printf("\n%s\n", pszHeading);

    // output column headings
    printf("   %-9s %-40s %-8s %-10s %-8s %-8s \n"
        , "Book Id", "Title", "Customer", "Ck Out Dt", "Late Fee", "Max Late");

    // output each book in a table format
    for (i = 0; i < iBookCnt; i++)
    {
        printf("   %-9s %-40s %-8s %-10s %8.2lf %8.2lf \n"
            , bookM[i].szBookId
            , bookM[i].szTitle
            , bookM[i].szCustomerId
            , bookM[i].szCheckedOutDt
            , bookM[i].dLateFeePerDay
            , bookM[i].dMaxLateFee
            );
    }
}


/********************processCustomerCommand *****************************
    void processCustomerCommand(Book bookM[], int iBookCnt
     , char *pszSubCommand, char *pszRemainingInput
     , Customer *pCustomer)
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
    I/O Book bookM[]                  Array of books
    I   int   iBookCnt                Number of elements in bookM[]
    I   char  *pszSubCommand          Should be BEGIN, ADDRESS, REQUEST or COMPLETE
    I   char  *pzRemainingInput       Points to the remaining characters in the input
                                      line (i.e., the characters that following the
                                      subcommand).
    I/O Customer *pCustomer           The BEGIN subcommand begins a new customer.  
Notes:

**************************************************************************/
void processCustomerCommand(Book bookM[], int iBookCnt
    , char *pszSubCommand, char *pszRemainingInput
    , Customer *pCustomer)
{
    int iscanfCnt;
    Transaction transaction;
    Customer customer;
    double dTransactionFee = 0;
    // Determine what to do based on the subCommand
    if (strcmp(pszSubCommand, "BEGIN") == 0)
    {
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

	printf("******************** Library Receipt ***********************\n");
        printf("%6s %s %s (Previously Owed: %3.2lf)\n"
                     , customer.szCustomerId
                     , customer.szEmailAddr
                     , customer.szFullName
                     , customer.dFeeBalance);


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
                        printf("%s \n%s %2s %5s\n"
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

        dTransactionFee = processTransaction(bookM, iBookCnt, customer, transaction);
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


       // printf("\n");
    }
    else printf("   *** %s %s\n", ERR_CUSTOMER_SUB_COMMAND, pszSubCommand);
}

/********************processBookCommand *****************************
    void processBookCommand(Book bookM[], int iBookCnt
         , char *pszSubCommand, char *pszRemainingInput)
Purpose:
    Processes the subcommands associated with the BOOK command:
        BOOK CHANGE szBookId dLateFeePerDay dMaxLateFee
            change the Late Fee Per Day and Max Late Fee to the specified values
        BOOK SHOW szBookId    
            requests a display of a particular book.  Show all of its information.
                
Parameters:
    I/O Book bookM[]                  Array of books
    I   int   iBookCnt                Number of elements in bookM[]
    I   char  *pszSubCommand          Should be CHANGE or SHOW
    I   char  *pzRemainingInput       Points to the remaining characters in the input
                                      line (i.e., the characters that following the
                                      subcommand).
Notes:

**************************************************************************/
void processBookCommand(Book bookM[], int iBookCnt
                             , char *pszSubCommand, char *pszRemainingInput)
{
    Book book;

    int iScanfCnt;
    int i;

    iScanfCnt = 0;

    //Determine what to do based upon the sub command
    if(strcmp(pszSubCommand, "CHANGE") == 0)
    {
	iScanfCnt = sscanf(pszRemainingInput, "%9s %lf %lf"
				, book.szBookId
				, &book.dLateFeePerDay
				, &book.dMaxLateFee);
	if (iScanfCnt != 3)
		exitError(ERR_BOOK_DATA, pszRemainingInput);

    	i = searchBooks(bookM,iBookCnt,book.szBookId);
	
	if (i == -1)
		printf("%s -- %s \n", ERR_BOOK_NOT_FOUND, book.szBookId);
	else
	{
	bookM[i].dLateFeePerDay = book.dLateFeePerDay;
	bookM[i].dMaxLateFee = book.dMaxLateFee;
	}
    }

    else if(strcmp(pszSubCommand, "SHOW") == 0)
    {
    	iScanfCnt = sscanf(pszRemainingInput, "%9s"
                                , book.szBookId);
        if (iScanfCnt != 1)
                exitError(ERR_BOOK_DATA, pszRemainingInput);

        i = searchBooks(bookM,iBookCnt,book.szBookId);

        if (i == -1)
                printf("%s -- %s \n", ERR_BOOK_NOT_FOUND, book.szBookId);
	else
	{
	        printf("   %-9s %-40s %-8s %-10s %-8s %-8s \n"
	        , "Book Id", "Title", "Customer", "Ck Out Dt", "Late Fee", "Max Late");

       		printf("   %-9s %-40s %-8s %-10s %8.2lf %8.2lf \n"
	            , bookM[i].szBookId
	            , bookM[i].szTitle
        	    , bookM[i].szCustomerId
	            , bookM[i].szCheckedOutDt
	            , bookM[i].dLateFeePerDay
        	    , bookM[i].dMaxLateFee);
	}
    }
    else printf("   *** %s %s\n", ERR_BOOK_SUB_COMMAND, pszSubCommand);

}
/******************** searchBooks *****************************
    int searchBooks(Book bookM[], int iBookCnt, char *pszMatchBookId)
Purpose:
    Using a binary search, it finds the specified book in the booKM array.
Parameters:
    I   Book bookM[]                 Array of books
    I   int   iBookCnt               Number of elements in bookM[]
    I   char *pszMatchBookId         Book Id to find in the array
Returns:
    >= 0   subscript of where the match value was found
    -1     not found
Notes:

**************************************************************************/
int searchBooks(Book bookM[], int iBookCnt, char *pszMatchBookId)
{
	int iLB, iUB, iMID;
	iLB = 0;
	iUB = iBookCnt -1;
	while ( iLB <= iUB)
	{
		iMID = (iLB+iUB) / 2;
		if (strcmp(bookM[iMID].szBookId, pszMatchBookId) == 0)
			return iMID;
		if (strcmp(bookM[iMID].szBookId, pszMatchBookId) > 0)
			iUB = iMID - 1;
		else
			iLB = iMID + 1;
	}
	return -1;
}


/******************** processTransaction *****************************
double processTransaction(Book bookM[], int iBookCnt, Customer customer, Transaction transaction)
Purpose:
    Processes Transaction Data and (if needed) modifies the bookM
    array.
Parameters:
    I Book bookM[]            Array of Books in Library
    I int iBookCnt            Count of Books in Array
    I Customer customer       Customer data structure access
    I Transaction transaction Transaction data structure access
Notes:
    1. Data that reaches this function has validated via basic error handling.
    2. If data is in conflict with database, following will return:
               -1: Book ID Not Found
               -2: Book Already Checked Out
               -3: Date Invalid
*********************************************************************/

double processTransaction(Book bookM[], int iBookCount, Customer customer, Transaction transaction)
{
 UtsaDate utsadate;     //UtsaDate Structure
 UtsaDate *pDate;       //Pointer for Struct to pass to validateDate()
 pDate = &utsadate;
 int index = 0;

 //Set index to subscript of book or -1
 index = searchBooks(bookM,iBookCount,transaction.szBookId);

 //Error handling -1.0 -> Invalid Book
 if (index == -1)
        return -1.0;

 //Error Handling for Date
 if (validateDate(transaction.szTransDt, pDate) != 0)
        return -3.0;

 //Error handling for book already checked out.
 if (transaction.cTransType == 'C' && strcmp(bookM[index].szCheckedOutDt,"0000-00-00") != 0 && strcmp(bookM[index].szCustomerId,"NONE") != 0)
        return -2.0;
 //Verifies book record is valid to check out.
 if (transaction.cTransType == 'C' && strcmp(bookM[index].szCheckedOutDt,"0000-00-00") == 0 && strcmp(bookM[index].szCustomerId,"NONE") == 0)
 {
        strcpy(bookM[index].szCustomerId,customer.szCustomerId);
        strcpy(bookM[index].szCheckedOutDt, transaction.szTransDt);
 }

 //Return Book statement
 if (transaction.cTransType == 'R')
 {
        int iDays = 0;          //Tracks date difference between checkout and transaction.
        double dFee = 0.00;     //Return variable for fees (if any)

        //Set iDays to # of days checked out and then subtract for two weeks.
        iDays = dateDiff(transaction.szTransDt,bookM[index].szCheckedOutDt) - 14;

	//If days is less than 0, book isn't overdue, no fees needed
        //Check book in and return.
        if (iDays < 0)
        {
                strcpy(bookM[index].szCustomerId,"NONE");
                strcpy(bookM[index].szCheckedOutDt,"0000-00-00");
                return 0.00;
        }

        //If function reaches this point, book is overdue
        //Set fee below
        dFee = iDays * bookM[index].dLateFeePerDay;

        //If fee is too large, set fee as max fee.
        if (dFee > bookM[index].dMaxLateFee)
                dFee = bookM[index].dMaxLateFee;

        //Turn book in and return fee.
        strcpy(bookM[index].szCustomerId,"NONE");
        strcpy(bookM[index].szCheckedOutDt,"0000-00-00");
        return dFee;
 }
}
