#include <stdio.h>
 
int func2(int *a) {
	int b = 6;
	printf("[5b]: %d, %d\n", *a, b);
 
	*a = 3;
	printf("[6b]: %d, %d\n", *a, b);
 
	return(*a + 2);
}
 
int func1(int a, int b) {
	printf("[4]: %d, %d\n", a, b);
 
	b = func2(&a);
	printf("[7]: %d, %d\n", a, b);
 
	return(a + 3);
}
 
int main() {
	int a = 3;
	int b = 2;
 
	printf("[1]: %d, %d\n", a, b);
	{
		int *c = &a;
		*c = 4;
		b = 7;
		printf("[2]: %d, %d\n", a, b);
	}

	printf("[3]: %d, %d\n", a, b);
	a = func1(b, a);
	{
	a=0;
	b=1;	
	}
	printf("[8]: %d, %d\n", a, b);
 
	return 0;
}