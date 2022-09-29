#include <stdio.h>

void fce(double *x, double y){
    double tmp;
    tmp = *x;
    *x = y;
    y=tmp;
    return;
}

int main(){
    double a = -6.0, b=21.0;
    fce(&b,a);
    printf("%lf,%lf",a,b);
    return 0;
}
