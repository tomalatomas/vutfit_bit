#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



#define EPS 0.0000001

bool endCondition(double yValue,double  ypValue);
double calculateY(double x, double ypValue);

int main(int argc, char *argv[]){ 
	double x = atof(argv[1]);

	double yValue=x;
	double ypValue;
	do{
		ypValue=yValue;
		yValue=calculateY(x, ypValue);
	} while(!endCondition(yValue,ypValue));
		
	
	fprintf(stdout, "odmX je %lf\n",yValue);
	return 0;
	(void) argc;
}

double calculateY(double x, double ypValue){
	double mezi=x/ypValue;
	return 0.5*(ypValue+mezi);
}

bool endCondition(double yValue,double ypValue){
	double odchylka = yValue-ypValue;
	if(abs(odchylka)<EPS) return true;
	else return false;
}

