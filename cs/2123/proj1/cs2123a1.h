/************************************************************************
cs2123a1.h
Purpose:
    Defines typedefs for
	Student - includes Id, name, age, gpa
    Prototypes
************************************************************************/


/**** typedefs ****/

// Student information
typedef struct
{
   int          iId;                    // Unique integer identifier for student
   char         szName[255];            // Student's name
   int          iAge;                   // Student's age
   float        fGPA;                   // Student's GPA
} Student;

#define TRUE 1
#define FALSE 0

Student * makeStudent(Student * students, int iId, char szName[]);
void setAge(Student * students, int iAge);
void setGPA(Student * students, float fGPA);
