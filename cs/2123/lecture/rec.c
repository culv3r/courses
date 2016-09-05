#include <stdio.h>

int mod (int a, int n);

int main(int argc, char *argv)
{
int a = 20;
int n = 4;

int answer = mod (a,n);

printf ("Mod is %d\n", answer);
}

int mod (int a, int n)
{
int count;
if (a-n < 0)
	return a;
mod(a-n,n);
}
