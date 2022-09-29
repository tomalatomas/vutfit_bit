#include <stdio.h>
#include <stdbool.h>
typedef struct Struktura{
    int result;
    int remainder;
    bool error;
} myStruct;

myStruct add(int* a, int* b){
    myStruct vys;
    vys.result =*a+ *b;
    vys.remainder =0;
    vys.error=false;
    return vys;
}

myStruct div(int* a, int* b){
    myStruct vys;
    if(*b){
        vys.result = *a/*b;
        vys.remainder - *a % *b;
        vys.error =fault;
    } else {
        vys.error=true
   }
    return vys;
}
int main(){
    int a;
    int b;
    char operator;
    myStruct vysledek;
    printf("Zadej vyraz: ");
    scanf("%d %c %d", &a, &operator,  &b);
    if(operator=='+'){
        vysledek = add(&a, &b);
    } else if(operator=='/'){
        vysledek = div(&a, &b);
    }
    if(vysledek.error==false){
        printf("Vysledek je: %d, %d", vysledek.result, vys.remainder);
        if(vysledek.remainder){
            printf("Zbytek: %d\n", vysledek.remainder)
        }
    } else{
        printf("Nastala chyba!");
    }
}


