#include <stdio.h>
#include <stdlib.h>

int main(){
	int *a;
	int *b;
	a=NULL;
	b=malloc(sizeof(int));
	*b=45;
	b=a;
	*b-=*a*-2;
	*a=*b;
	printf("%d\n", *a);
    return 0;
}
