/**********************************************************************
p2swf474.c  (change the name to use your abc123 id)
Purpose:
    This program reads customer transactions to produce output 
    suitable as a Library Receipt. 
Command Line Arguments:
    p2 -b libraryBookFileName  -c  customerTransactionFileName  
Input:  
    Stream input file which contains many customers, each 
    containing possibly many book transactions.  There are three different 
    kinds of lines of data for each transaction:
    - Customer Identification Information:
        o One line per customer (separated by spaces)
            szCustomerId  dFeeBalanace  szEmailAddr  szFullName 
             6s              lf           40s           30s (may contain spaces)
        o Although szFullName is a maximum of 30 characters, it may 
          contain spaces; therefore, you cannot simply use %30s.  
          For szFullName, you will have to use a bracket format 
          code: %30[^\n] which doesn't have an 's' in it.
    - Customer Address Information:
        o One line per customer (separated by commas)
        o szStreetAddress              szCity   szStateCd   szZipCd
              30s (may contain spaces)   20s      2s          5s
        o Although szStreetAddress is a maximum of 30 characters, 
          it may contain spaces; therefore, you cannot simply use %30s.  
          You will have to use a bracket format code using %[^,]
        o You do not need another while loop for this address information 
          since there is only one per customer.  You will need to use fgets 
          and an if statement.
    - Book Transaction:
        o 0 to many book transactions per customer (terminated by END in the Book ID)
        o cTransType    szBookId   szTransDt 
           1c            s9              s10
        o cTransType must be either C for check out or R for return

Results:
    Prints each Library Receipt in a readable format.
    Examples:
        ******************** Library Receipt ***********************
        111111 petem@xyz.net Pete Moss (previously owed 0.75)
        123 Boggy Lane
        New Orleans LA 70112
        Trans  Book       Date
          C    JOYPGM001  2016-01-25
          C    TECHDR001  2016-01-25
        ******************** Library Receipt ***********************
        222222 pcorn@abc.net Pop Corn (previously owed 0.00)
        456 Kernel
        San Antonio TX 78210
        Trans  Book       Date
          C    TECHDR001  2016-01-25
          C    TECHDR002  2016-01-25
          C    JOYPGM004  2016-01-25
Returns:
    0  normal
    -1 invalid command line syntax
    -2 show usage only
    -3 error during processing, see stderr for more information
Notes:
    p1 -?  will provide the usage information.  In some shells,
                you will have to type reserve -\?
  
**********************************************************************/
// If compiling using visual studio, tell the compiler not to give its warnings
// about the safety of scanf and printf
#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cs1713p2.h"
FILE *pFileCust;               // stream Input for Customer Transaction data
FILE *pFileBook;               // stream input for Library books

void processCommandSwitches(int argc, char *argv[], char **ppszCustomerFileName, char **ppszBookFileName);
void sortBooks(Book bookM[], int iBookCnt);

int main(int argc, char *argv[])
{
    char *pszCustomerFileName = NULL;
    char *pszBookFileName = NULL;
    Book bookM[MAX_BOOKS];
    int iBookCount;
 
    // Process the command switches
    processCommandSwitches(argc, argv,  &pszCustomerFileName, &pszBookFileName);

    // open the book stream data file
    if (pszBookFileName == NULL)
	exitError(ERR_MISSING_SWITCH, "-b");

    pFileBook = fopen(pszBookFileName, "r");
    if (pFileBook == NULL)
	exitError(ERR_BOOK_FILENAME, pszBookFileName);

    // Populate the Book Array
    iBookCount = getLibrary(bookM);

    fclose(pFileBook);

    // print the unmodified book data
    printf("Original Book Information\n");
    printLibrary(bookM, iBookCount);

    // Testing my bubble sort code
    sortBooks(bookM,iBookCount);
    printf("Sorted Book Information\n");
    printLibrary(bookM, iBookCount);
    
    // open the Customer Transaction stream data file
    if (pszCustomerFileName == NULL)
        exitError(ERR_MISSING_SWITCH, "-c");
    
    pFileCust = fopen(pszCustomerFileName, "r");
    if (pFileCust == NULL)
        exitError(ERR_CUSTOMER_FILENAME, pszCustomerFileName);
   
    // process the Customers Data
//    processCustomers(bookM, iBookCount);

    fclose(pFileCust);

    // print out modified library information

  //  printf("Post Transaction Information\n");
   // printLibrary(bookM, iBookCount);

    printf("\n");    // included so that you can put a breakpoint on this line
    return 0;
}

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

/************************** processCustomers *****************************
void processCustomers(char *pszCustomerFileName)
Purpose:
    Takes data stream from file and processes data into a human readable
    format. If it encounters any errors with the data, it exits and 
    explains the error.
Parameters:
	I Book bookM[]
	I int iBookCount
Notes:
    Customer file name is passed to this function to facilitate passing
    it to the error processor when required.
**************************************************************************/

void processCustomers(Book bookM[], int iBookCount)
{
Customer customer; //Customer Structure
Transaction transaction; //Transaction Structure
int iScanfCnt; //Counts successful sscanf transactions
char szInputBuffer[100]; //Buffer for reading from file
int iLineCount = 0;   //Counts the line that is being read to switch between variables.
double dTotalFee = 0; //Tracks total fees
double dTransactionFee = 0; //Tracks transaction fees

	//Read file to EOF

	while (fgets(szInputBuffer, 100, pFileCust) != NULL)
	{
		iScanfCnt = 0;

		//If line is blank, continue

		if (szInputBuffer[0] == '\n')
			continue;

		//If statement below should reference the first line of a customer entry
		if (iLineCount == 0)
		{

			iScanfCnt = sscanf(szInputBuffer, "%6s %lf %40s %30[^\n]"
							, customer.szCustomerId
							, &customer.dFeeBalance
							, customer.szEmailAddr
							, customer.szFullName);


			if (iScanfCnt != 4)	//Error Handling
				exitError(ERR_CUSTOMER_ID_DATA, szInputBuffer);

			//print formatted information from first line

			printf("******************** Library Receipt ***********************\n");
			printf("%6s %s %s (Previously Owed: %3.2lf)\n"
			        , customer.szCustomerId
			       	, customer.szEmailAddr
				, customer.szFullName
				, customer.dFeeBalance);

		}

		//If statement below should reference the address line of a customer entry
		if (iLineCount == 1)
		{
			iScanfCnt = sscanf(szInputBuffer, "%30[^,],%20[^,],%2s,%5s"
							, customer.szStreetAddress
							, customer.szCity
							, customer.szStateCd
 							, customer.szZipCd);


			if (iScanfCnt != 4)//Error Handling
				exitError(ERR_CUSTOMER_ADDRESS_DATA, szInputBuffer);

			//print second line of  address information.
			printf("%s \n%s %2s %5s\n"
			        , customer.szStreetAddress
			       	, customer.szCity
        			, customer.szStateCd
	        		, customer.szZipCd);

			//headers for transactions
			printf("Trans Book      Date\n");

		}

		//remaining If statement handles the book transaction records

		if (iLineCount > 1)
		{
			iScanfCnt = sscanf(szInputBuffer, "%c %9s %10s"
							, &transaction.cTransType
							, transaction.szBookId
							, transaction.szTransDt);

			if (iScanfCnt != 3)//Error Handling
				exitError(ERR_TRANSACTION_DATA, szInputBuffer);

			//Nested if statement looks for end statement to print out the total fees due (if any)

			if (strcmp(transaction.szBookId,"END") == 0)
			{
				dTotalFee = dTotalFee + customer.dFeeBalance;
				printf("\t\t\t\tTotal Fees = %3.2lf\n", dTotalFee);
				//printf("************************************************************\n");
				dTotalFee = 0;
			}

			//If not ended, process transactions and handle errors of transactions
			else
			{
                              	dTransactionFee = processTransaction(bookM, iBookCount, customer, transaction);
				//process transaction details
				printf("  %c   %-9s %-10s"
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
					printf(" *** Invalid Date");
				//If there is no fee, or error, just print a new line.
				else
					printf("\n");
			}
		}
		//Increment the line count, to step between the if statements
		iLineCount++;

		//Checks if the END ID has been transacted and if so, resets line count
		// and clears transaction.szBookId.
		if (iLineCount > 1 && strcmp(transaction.szBookId,"END") == 0)
			iLineCount = 0;
			strcpy(transaction.szBookId,"");
	}
}

/******************** getLibrary *****************************
int getLibrary(Book bookM[])
Purpose:
    Retrieves the books using the FILE pointer, pFileBook, that 
    you defined above the main function and returns a count of the number of books.
Parameters:
    O   Book bookM[]    an array which is used for returning the books.
Returns:
    A count of the number of read.
Notes:
    1. Uses pFileBook as the FILE pointer for reading the file.  It must
       have been opened with fopen prior to calling this function.
    2. The data in the data file is in this format
    szBookId szCustomerId szCheckedOutDt dLateFeePerDay dMaxLateFee szTitle
    9s        6s             10s           lf            lf          40s (may contain spaces)
**************************************************************************/
int getLibrary(Book bookM[])
{
    int i = 0;
    char szInputBuffer[100];
    int iScanfCnt = 0;
    // Read the books until EOF
    while (fgets(szInputBuffer, 100, pFileBook) != NULL)
    {
        // check for too many books to fit in the bookM array
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
        // an error.
        if (iScanfCnt < 6)
            exitError(ERR_BOOK_DATA, szInputBuffer);
        i++;
    }
    return i;
}

/******************* printLibrary ************************************
void printLibrary(Book bookM[])
Purpose:
    Prints out the library book information in a human readable
    format.
Parameters:
    I Book bookM[]          Array of Library Books
Notes:
    1. Information should be pre-validated by getLibrary()
    2. Prints out information in the following format:
	Book Id -- Title -- Customer -- Ck Out Dt -- Late Fee -- Max
		Late

*********************************************************************/
void printLibrary(Book bookM[], int iBookCount)
{
 int i;		//Index variable for array traversal

 //Headers for library information
 printf("%-9s %-35s %-8s %-10s %-8s %-8s\n", "Book Id", "Title", "Customer", "Ck Out Dt", "Late Fee", "Max Late");

 //Iterate through the list of books and print the data in the proper format.
 for (i = 0; i<iBookCount; i++)
 {
 	printf("%-9s %-35s %-8s %-10s %6.2lf   %6.2lf\n"
			, bookM[i].szBookId
			, bookM[i].szTitle
			, bookM[i].szCustomerId
			, bookM[i].szCheckedOutDt
			, bookM[i].dLateFeePerDay
			, bookM[i].dMaxLateFee);
 }
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
 UtsaDate utsadate;	//UtsaDate Structure
 UtsaDate *pDate;	//Pointer for Struct to pass to validateDate()
 pDate = &utsadate;
 int index = 0;

 //Set index to subscript of book or -1
 index = findBook(bookM,iBookCount,transaction.szBookId);

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
	int iDays = 0;		//Tracks date difference between checkout and transaction.
	double dFee = 0.00; 	//Return variable for fees (if any)

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
/******************** findBook **************************************
int findBook(Book bookM[], int iBookCount, char szBookId[])
Purpose:
    Searches through bookM array and returns subscript of matching
    book ID.
Parameters:
    I Book bookM[]           Array of Library Books
    I int iBookCount         Size of Book Array
    I char szBookId[]        ID of Book to search for
*********************************************************************/

int findBook(Book bookM[], int iBookCount, char szBookId[])
{
 int i = 0;

 //Iterate through list and return the subscript of the book.
 for (i = 0; i<iBookCount; i++)
 {
 	if (strcmp(bookM[i].szBookId,szBookId) == 0)
		return i;
 }

 //If the loop completes, id is not found, return error
 return -1;
}
/******************** validateDate ***********************************
int validateDate(char szDate[], UtsaDate *pDate);
Purpose:
    Validates the date and returns (via the second parameter) the 
    UtsaDate.   
Parameters:
    I char szDate[]         Date as a string in the form "yyyy-mm-dd"
    O UtsaDate *pDate       Address of a UtsaDate structure for 
                            returning the date.
Notes:
    1. The length must be 10 characters.
    2. The date must be in the form "yyyy-mm-dd".
    3. The month must be 01-12.
    4. The day must be between 1 and the max for each month
               Mar 31 Aug 31 Jan 31
               Apr 30 Sep 30 Feb 29
               May 31 Oct 31
               Jun 30 Nov 30
               Jul 31 Dec 31
    5. If Feb 29 was specified, validate that the year is a leap year.
Return Value:
    0     date is valid
    1     year is invalid
    2     month is invalid
    3     day is invalid
    4     invalid length or format
**********************************************************************/
int validateDate(char szDate[], UtsaDate *pDate)
{
    static int iDaysPerMonth[] = 
       { 0, 31, 29, 31
          , 30, 31, 30
          , 31, 31, 30
          , 31, 30, 31 };
    int iCharCnt = 0;
    int iScanfCnt;

    // Check for too few characters for the yyy-mm-dd format
    if (strlen(szDate) != 10)
        return 4;  // invalid format due to length
    
    // The year should be 4 characters 
    iCharCnt = strspn(szDate, "0123456789");
    if (iCharCnt != 4)
        return 1;  // invalid year
    
    // Use sscanf to pluck out the year, month, and day
    iScanfCnt = sscanf(szDate, "%4d-%2d-%2d"
        , &pDate->iYear
        , &pDate->iMonth
        , &pDate->iDay);
    // If sscanf returned less than 3, then something was bad
    if (iScanfCnt < 3)
        return 4;  // invalid format
    
    // Validate Month
    if (pDate->iMonth < 1 || pDate->iMonth > 12)
        return 2;  // month invalid
    
    // Validate day based on max days per month 
    if (pDate->iDay < 1 || pDate->iDay > iDaysPerMonth[pDate->iMonth])
        return 3;  // day invalid

    // if the 29th of Feb, check for leap year
    if (pDate->iDay == 29 && pDate->iMonth == 2)
    {
        if (pDate->iYear % 4 == 0 && (pDate->iYear % 100 != 0 || pDate->iYear % 400 == 0))
            return 0;    // it is a leap year
        else return 3;   // not a leap year, so the day is invalid
    }
    return 0;
}
/******************** dateDiff ***********************************
int dateDiff(char szDate1[], char szDate2[])
Purpose:
    Validates the dates and returns the difference in days of the 
    first date minus the second date.   
Parameters:
    I char szDate1[]        First date as a string in the form "yyyy-mm-dd"
    I char szDate2[]        Second date as a string in the form "yyyy-mm-dd"
Notes:
    1. We validate/convert the days to UtsaDate format.  If the date is
       invalid, we exit and show a message.
    2. For each of the dates, we determine the number of days since
       "0000-03-01" by starting the count at 1 for 0000-03-01. Using
       March 1st eliminates some leap day issues. 
    3. Return the difference in days
Return Value:
    the difference in dates
**********************************************************************/
int dateDiff(char szDate1[], char szDate2[])
{
    UtsaDate date1;
    UtsaDate date2;
    int iJulian1;
    int iJulian2;
  
    if (validateDate(szDate1, &date1) != 0)
        exitError("Invalid 1st date for dateDiff: ", szDate1);

    if (validateDate(szDate2, &date2) != 0)
        exitError("Invalid 2nd date for dateDiff: ", szDate2);

    iJulian1 = utsaDateToUtsaJulian(date1);
    iJulian2 = utsaDateToUtsaJulian(date2);

    return iJulian1 - iJulian2;
}

/******************** utsaDateToUtsaJulian ***********************************
int utsaDateToUtsaJulian(UtsaDate date)
Purpose:
    Converts a date to a UTSA Julian Days value.  This will start numbering
    at 1 for 0000-03-01. Making dates relaive to March 1st helps eliminate
    some leap day issues. 
Parameters:
    I char szDate1[]        First date as a string in the form "yyyy-mm-dd"
    I char szDate2[]        Second date as a string in the form "yyyy-mm-dd"
Notes:
    1 We replace the month with the number of months since March.  
      March is 0, Apr is 1, May is 2, ..., Jan is 10, Feb is 11.
    2 Since Jan and Feb are before Mar, we subtract 1 from the year
      for those months.
    3 Jan 1 is 306 days from Mar 1.
    4 The days per month is in a pattern that begins with March
      and repeats every 5 months:
           Mar 31 Aug 31 Jan 31
           Apr 30 Sep 30
           May 31 Oct 31
           Jun 30 Nov 30
           Jul 31 Dec 31
       Therefore:
           Mon  AdjMon  NumberDaysFromMarch (AdjMon*306 + 5)/10
           Jan    10      306
           Feb    11      337
           Mar     0        0
           Apr     1       31
           May     2       61
           Jun     3       92
           Jul     4      122 
           Aug     5      153
           Sep     6      184
           Oct     7      214
           Nov     8      245
           Dec     9      275
    5 Leap years are 
           years that are divisible by 4 and
           either years that are not divisible by 100 or 
           years that are divisible by 400
Return Value:
    the number of days since 0000-03-01 beginning with 1 for 
    0000-03-01.
**********************************************************************/
int utsaDateToUtsaJulian(UtsaDate date)
{
    int iCountDays;
    // Calculate number of days since 0000-03-01

    // If month is March or greater, decrease it by 3.
    if (date.iMonth > 2)
        date.iMonth -= 3;
    else
    {
        date.iMonth += 9;  // adjust the month since we begin with March
        date.iYear--;      // subtract 1 from year if the month was Jan or Feb
    }
    iCountDays = 365 * date.iYear                                // 365 days in a year
        + date.iYear / 4 - date.iYear / 100 + date.iYear / 400   // add a day for each leap year
        + (date.iMonth * 306 + 5) / 10                           // see note 4
        + (date.iDay );                                          // add the days
    return iCountDays;
}

/******************** processCommandSwitches *****************************
void processCommandSwitches(int argc, char *argv[], char **ppszCustomerFileName)
Purpose:
    Checks the syntax of command line arguments and returns the filenames.
    If any switches are unknown, it exits with an error.
Parameters:
    I   int argc                        count of command line arguments
    I   char *argv[]                    array of command line arguments
    O   char **ppszCustomerFileName     customer transaction file name
Notes:
    If a -? switch is passed, the usage is printed and the program exits
    with USAGE_ONLY.
    If a syntax error is encountered (e.g., unknown switch), the program
    prints a message to stderr and exits with ERR_COMMAND_LINE_SYNTAX.
**************************************************************************/
void processCommandSwitches(int argc, char *argv[], char **ppszCustomerFileName, char **ppszBookFileName)
{
    int i;
    
    for (i = 1; i < argc; i++)
    {
        // check for a switch
        if (argv[i][0] != '-')
            exitUsage(i, ERR_EXPECTED_SWITCH, argv[i]);
        // determine which switch it is
        switch (argv[i][1])
        {
       	    case 'b':   // Book Information File Name
	      if (++i >= argc)
		exitUsage(i, ERR_MISSING_ARGUMENT, argv[i - 1]);
	      else
		*ppszBookFileName = argv[i];
	      continue;
            case 'c':                   // Customer Transaction File Name
                if (++i >= argc)
                    exitUsage(i, ERR_MISSING_ARGUMENT, argv[i - 1]);
                else
                    *ppszCustomerFileName = argv[i];
                break;
            case '?':
                exitUsage(USAGE_ONLY, "", "");
                break;
            default:
                exitUsage(i, ERR_EXPECTED_SWITCH, argv[i]);
        }
    }
}

/******************** exitError *****************************
    void exitError(char *pszMessage, char *pszDiagnosticInfo)
Purpose:
    Prints an error message and diagnostic to stderr.  Exits with
    ERROR_PROCESSING.
Parameters:
    I char *pszMessage              error message to print
    I char *pszDiagnosticInfo       supplemental diagnostic information
Notes:
    This routine causes the program to exit.
**************************************************************************/
void exitError(char *pszMessage, char *pszDiagnosticInfo)
{
    fprintf(stderr, "Error: %s %s\n"
        , pszMessage
        , pszDiagnosticInfo);
    exit(ERROR_PROCESSING);
}
/******************** exitUsage *****************************
    void exitUsage(int iArg, char *pszMessage, char *pszDiagnosticInfo)
Purpose:
    If this is an argument error (iArg >= 0), it prints a formatted message
    showing which argument was in error, the specified message, and
    supplemental diagnostic information.  It also shows the usage. It exits
    with ERR_COMMAND_LINE_SYNTAX.

    If this is just asking for usage (iArg will be -1), the usage is shown.
    It exits with USAGE_ONLY.
Parameters:
    I int iArg                      command argument subscript
    I char *pszMessage              error message to print
    I char *pszDiagnosticInfo       supplemental diagnostic information
Notes:
    This routine causes the program to exit.
**************************************************************************/
void exitUsage(int iArg, char *pszMessage, char *pszDiagnosticInfo)
{
    if (iArg >= 0)
        fprintf(stderr, "Error: bad argument #%d.  %s %s\n"
            , iArg
            , pszMessage
            , pszDiagnosticInfo);
    fprintf(stderr, "p2 -b bookInformationFile -c customerTransactionFile\n");
    if (iArg >= 0)
        exit(-1);
    else
        exit(-2);
}
