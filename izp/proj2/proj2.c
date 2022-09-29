#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define I0 1e-12
#define UT 0.0258563
#define MINEPS 1e-16


/*************************
*@author: xtomal02(Tomáš Tomala)
*@project:proj2
*************************/

/************************************************
 *@functionName:loadValues
 *@param argc-number of arguments for the program
 *@param argv-arguments for the program
 *@param u0-pointer on value of voltage 
 *@param r-pointer on value of resistance
 *@param eps-pointer on epsilon-precision of found Up
 *Function loads arguments from stdin. If arguments dont contain text and are larger than 0, values are converted to double and stored on address of pointer. 
 *If epsilon is larger than constant MINEPS, MINEPS value is loaded into epsilon.
 *Function returns logical value. If true is returned, values were loaded without an error. If false is returned, arguments were invalid.
 ***********************************************/
bool loadValues(int argc, char *argv[], double *u0, double *r, double *eps);
/************************************************
 *@functionName:diode
 *@param u0-value of voltage 
 *@param r-value of resistance
 *@param eps-epsilon-precision of found Up
 *Function  contains loop. Loop calculates Up by bisection method. Method takes interval(min;max). Min at start is 0, max is U0.
 *Up is middle of the interval. If equation Ip-Ir is larger than 0, Up is new max in interval.  If equation Ip-Ir is lesser than 0, Up is new min in interval.
 *Loop ends if Up is infinite, max-min is lesser than epsilon or function deviation returns true.
 *Function returns value of Up represented as double.
 ***********************************************/
double diode(double u0, double r, double eps);
/************************************************
 *@functionName:deviation
 *@param ip-Current on diode (calculated in function "diode")
 *@param ir-Current on resistor (calculated in function "diode")
 *@param eps-value of epsilon - precision of calculation
 *Function returns logical value. True is returned if difference between ir and ip is lesser then epsilon. In any other case, false is returned.
 ***********************************************/
bool deviation(double ip,double ir, double eps);

int main(int argc, char *argv[]){
	double u0=0.0;
	double r=0.0;
	double eps=0.0;
	if(loadValues(argc,argv,&u0,&r,&eps)==false){
		fprintf(stderr, "Invalid arguments.\n");
		return 1;
	} 
	double up=diode(u0,r,eps);
	double ip=I0*(exp(up/UT)-1);
	fprintf(stdout, "Up=%lg V\nIp=%lg A\n",up,ip);
	return 0;
}

bool loadValues(int argc, char *argv[], double *u0, double *r, double *eps){
	if(argc==4){
		char *test = NULL;
		*u0=strtod(argv[1],&test);
		if (*test != '\0'||*u0<=0) return false;
		test=NULL;
		*r=strtod(argv[2],&test);
		if (*test!='\0'||*r<=0) return false;
		test=NULL;
		*eps=strtod(argv[3],&test);
		if (*test!='\0'||*eps<=0) return false;
		test=NULL;
		if (*eps<MINEPS)*eps=MINEPS;
		//printf("%g\n",*eps);
		return true;
	} else{
		return false;
	}
}


double diode(double u0, double r, double eps){
	double min=0, max=u0, ip, ur, ir, up;
	do{
		up =(max+min)/2;
		ip=I0*(exp(up/UT)-1);
		ur=u0-up;
		ir=ur/r;
		if(ip-ir>0)max=up;
		else min=up;
		//printf("%g\n",max-min );
		if(fabs(max-min)<eps) break;
		if(isinf(up)) break;
	}
	while(!deviation(ip,ir,eps));
	return up;
}

bool deviation(double ip,double ir, double eps){
	//printf("%g<%g\n",fabs(ip-ir),eps);
	if(fabs(ip-ir)<eps) return true;
	else return false;
}




