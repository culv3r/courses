/**************************************************************************************
Assignment 1
**************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cs2123a1.h"

int n;

int main (int argc, char *argv[])
{
FILE *pfileStudent;
int iScanfCnt, iAge, iId, i;
float fGPA;
char szName[255];
int numStudents = 0;
char szInputBuffer[100];
int iLineCount = 0;
int bValidId = FALSE;
pfileStudent = fopen("data_a1.txt", "r");

printf("CS 2123 Assignment 1 written by Daniel Larsen.\n");

	if (pfileStudent == NULL)
	{
		printf("Error: file does not exist.\n");
		exit(-1);
	}

	while (fgets(szInputBuffer, 100, pfileStudent) != NULL)
	{
		iScanfCnt = 0;

		if (szInputBuffer[0] == '\n')
			continue;

		if (iLineCount == 0)
		{
			iScanfCnt = sscanf(szInputBuffer, "%d", &iId);
		}

		if (iLineCount == 1)
		{
			iScanfCnt = sscanf(szInputBuffer, "%[^\n]", szName);
		}
	
		if (iLineCount == 2)
		{
			iScanfCnt = sscanf(szInputBuffer, "%d", &iAge);
		}
	
		if (iLineCount == 3)
		{
			iScanfCnt = sscanf(szInputBuffer, "%f", &fGPA);
			if (iId > 0 && strlen(szName) >= 1)
			{
				bValidId = TRUE;
			}	
		}
		if (bValidId == TRUE)
			numStudents++;
	
		if (iLineCount != 3)
			iLineCount++;
		else
		{
			iLineCount = 0;
			bValidId = FALSE;
		}


	}

fclose(pfileStudent);

Student * students = (Student *) malloc(sizeof(Student) * numStudents);

pfileStudent = fopen("data_a1.txt", "r");


	if (students == NULL)
	{
		printf("Error! Memory not allocated!\n");
		exit(-1);
	}

n = 1;

	while (fgets(szInputBuffer, 100, pfileStudent) != NULL)
	{
		iScanfCnt = 0;

        	if (szInputBuffer[0] == '\n')
                	continue;

	        if (iLineCount == 0)
	        {
	                iScanfCnt = sscanf(szInputBuffer, "%d", &iId);
	        }

	        if (iLineCount == 1)
	        {
	                iScanfCnt = sscanf(szInputBuffer, "%[^\n]", szName);
        	        if (iId > 0 && strlen(szName) >= 1)
			{
				bValidId = TRUE;
				makeStudent(students, iId, szName);
			}
			else
				printf("Error: %d is not a positive integer for student's ID\n", iId);
	        }

	        if (iLineCount == 2)
	        {
        	        iScanfCnt = sscanf(szInputBuffer, "%d", &iAge);

	                if (bValidId == TRUE)
				setAge(students, iAge);
	        }
        
	        if (iLineCount == 3)
	        {
        	        iScanfCnt = sscanf(szInputBuffer, "%f", &fGPA);

	                if (bValidId == TRUE)
				setGPA(students, fGPA);
        	}

	        if (iLineCount != 3)
		{
       	         iLineCount++;
		}
       		else
		{
			if (bValidId == TRUE)
				n++;
			iLineCount = 0;
			bValidId = FALSE;
		}

	}
fGPA = 0.0;
iAge = 0;
	for (i = 0; i < numStudents; i++)
	{	
		fGPA += (*(students + i)).fGPA;
		iAge += (*(students + i)).iAge;
	}
printf("# of students: %d\nAverage age: %d\nAverage GPA: %f\n", numStudents, (iAge/numStudents), (fGPA/numStudents));

fclose(pfileStudent);

free(students);

return 0;
}


Student * makeStudent(Student * students,int iId, char *szName)
{
(*(students + (n-1))).iId = iId;
strcpy((*(students + (n-1))).szName,szName);
}

void setAge(Student * students, int iAge)
{
(*(students + (n-1))).iAge = iAge;
}

void setGPA(Student * students, float fGPA)
{
(*(students + (n-1))).fGPA = fGPA;
}
