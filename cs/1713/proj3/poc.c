#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
 int theory = strcmp("JOYPGM001", "PYTHON001");
 int theory2 = strcmp("JOYPGM001","JOYPGM002");
 if (theory == -1)
	printf("JOYPGM001 has a character that is less than PYTHON001 %d\n", theory);
 if (theory2 != 1)
	printf("JOYPGM002 is not less than JOYPGM001 %d\n", theory2);
}
