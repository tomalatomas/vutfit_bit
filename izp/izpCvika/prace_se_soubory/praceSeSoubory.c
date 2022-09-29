#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void slovaZeSouboru();
void kartoteka();
void max(int lhs, int rhs);
void vypisArgumenty(int pocetArgumentu, char* argumenty[]);
void standartVstup();
void vypisArgumentyChar(int pocetArgumetu, char* argumenty[]);
void pocetArgumentu(int pocetArgumentu, char* argumenty[]);
void zjistiCharArgumentu(int pocetArgumentu, char* argumenty[]);
void swap(char a[], char b[]);
char *stringChar(char *str, char c);
int *sum(int *a, int *b, int size);
struct cp{
	char* zastavka;
	int h;
	int m;
	int m_to_n;
}
nactiCestPoradek();
int main(){
	//slovaZeSouboru();
	//kartoteka();
	//int cislo = max(22,11);
	//vypisArgumenty(argc,argv);
	/*printf("Načtěte charakter\n");
	int charakter=getchar();
	while(charakter!=EOF){
		if(charakter!=10){
			printf("%d %c\n",charakter,charakter);
		}
		charakter = getchar();
	}
	//vypisArgumentyChar(argc, argv);
    //pocetArgumentu(argc, argv);
    // zjistiCharArgumentu(argc, argv);
    char a[] = "Jedn";
    char b[] = "icka";
    swap(a,b);
    printf("%s %s",a,b);
    */
    nactiCestPoradek();

	return 0;
}

void nactiCestPoradek(){

	FILE* soubor = fopen(argv[1],"r");
	if(soubor != NULL){
		char infoCP[100];
		while(fscanf(soubor, "%99s", infoCP)==1){
			printf("%s\n",infoCP);
		}
	}
	fclose(soubor);

}
/*

int *sum(int *a, int *b, int size){
	//Jak funkci spustit - screen na discordu private
	int i=0;
	int *c = malloc(size*sizeof(int));
	while(i<size){
		c[i] = a[i]+b[i];
		i++;
	}
	return c;
}

char *stringChar(char *str, char c){
    //char *str = "abcd";
    //printf("%s\n",stringChar(str,'c'););
    //printf("%s\n",stringChar(str,'e'));

  		int delka = strlen(str);
	int i=0;
	while(i<delka){
			if(str[i]==c){
				return &str[i];
			}
		i++;
	}

	return "";

}



void swap (char a[], char b[]){
    char *tmp = a;
    a = b;
    b = tmp;
}
void zjistiCharArgumentu(int pocetArgumentu, char* argumenty[]){
    int iterace = 0;
    while(pocetArgumentu>iterace){
        for(int pismeno=0;pismeno<strlen(argumenty[iterace]);pismeno++){
            if(argumenty[iterace][pismeno]=='h'){
                printf("Bylo nalezeno písmeno 'h' v %d. argumentu  a %d. písmenu", iterace, pismeno+1);
                exit(0);
            }
        }
        iterace++;
    }
}

void pocetArgumentu(int pocetArgumentu, char* argumenty[]){
	int iterace=0;
    int pocetHelp=0;
    while(pocetArgumentu>iterace){
        if(strcmp(argumenty[iterace],"--help")==0){
            pocetHelp++;
        }
        iterace++;
    }
    printf("Help byl zavolán %dx\n",pocetHelp);
}

void vypisArgumentyChar(int pocetArgumentu, char* argumenty[]){
	int i = 0;
	for(i,pocetArgumentu>i, i++){
		j=0;
		while(argumenty[i][j] != '\0'){
			printf("%c",argumenty[i][j++]);
		}
	}
}

void slovaZeSouboru(){
	FILE* soubor = fopen("praceSeSoubory.txt","r");
	if(soubor != NULL){
		char nacteni[99];
		while(fscanf(soubor, "%98s", nacteni)==1){
		printf("%s \n", nacteni);
		}
	}
	fclose(soubor);
}
void kartoteka(){
	printf("Zadejte rodné číslo:");
	int rodneCislo;
	scanf("%d",&rodneCislo);
	printf("Zadejte vaše jméno a příjmení:");
	char jmeno[50];
		printf("Zadejte vaši výšku v desetinném čísle pomocí tečky\n");
	double vyska;
	scanf("%lf", &vyska);
	printf("%s %.2f\n", jmeno,vyska);
	//fprint(soubor,"ABC %d\n",1); - zapsání do souboru ABC a text
	printf("Byl jste zaveden do evidence\n");
}

void max(int lhs, int rhs){
	if(lhs>rhs){
		printf("%d/n",lhs);
	} else{
		printf("%d/n",rhs);
	}
}

void vypisArgumenty(int pocetArgumentu, char* argumenty[]){
	//zakomponovat boolean, aby se nevypisovalo vickrat 
	int i=0;
	while(i<pocetArgumentu){
		//printf("%s/n",argumenty[i]);
		if(strcmp(argumenty[i], "-h")==0 || strcmp(argumenty[i],"-help")==0){
			printf("help/n");
		}
		else{
			printf("Nápověda nenalezena/n");
		}
		i++;
	}
}

void standartVstup(
	//dokoncit
        char textBezMezer[256];
	int vstup = scanf("%255s",textBezMezer);
	while(vstup==1){
		printf("Nactene slovo: %s\n",textBezMezer)
	}
);
*/
