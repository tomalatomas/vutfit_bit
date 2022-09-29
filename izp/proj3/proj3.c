#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define ARGHELP "--help"
#define ARGTEST "--test"
#define HELP "Argument options:\n 1. --test file.txt (file.txt contains map of maze)\n  Program checks if map is valid.\n 2. --lpath/--rpath R C file.txt\n  Choose one of the variants of path finding. R is number of rows and C is number of columns. file.txt is file with map of maze.\n"
#define MAXBUFFER 100
#define RIGHTLEFTBORDER 1
#define DOWNUPBORDER 2
#define MAXBITS 3
#define LEFTBORDER 2
#define RIGHTBORDER 1
#define HORIZONTBORDER 0


typedef struct {
	int rows;
	int cols;
	unsigned char *cells;
} Map;

/*************************
*@author: xtomal02(Tomáš Tomala)
*@project:proj3
*************************/

void arguments(int argc,char* argv[]);
/************************************************
 *@functionName:arguments
 *@param argc-number of arguments for the program
 *@param argv-arguments for the program
 *Function loads arguments from stdin. 
 *If there are two arguments and second is "--help", help is printed out.
 *If there are three arguments and second is "--test", function checkMap with third argument as parameter is called.
   If function checkMap returns false, "Invalid" is printed out. If function checkMap returns true, "Valid" is printed out. 
 ***********************************************/
bool isValidNumber(char c);
/************************************************
 *@functionName:isValidNumber
 *@param c-character
 *Function returns logical value. True is returned if char is valid number for the definition of the map. 
 *If character isnt valid, false is returned.
 ***********************************************/
bool isValidNumberFL(char c);
/************************************************
 *@functionName:isValidNumberFL
 *@param c-character
 *Function returns logical value. True is returned if char is valid number for the number of rows and columns in the first line. 
 *If character isnt valid, false is returned.
 ***********************************************/
bool areAllowedChars(bool firstLine,char string[]);
/************************************************
 *@functionName:areAllowedChars
 *@param c-character
 *Function checks if array of chararacters contains only allowed characters.
 *True is returned if string contains only allowed characters, otherwise false is returned;
 ***********************************************/
bool checkMap(char* argv[]);
/************************************************
 *@functionName:checkMap
 *@param argv-array of characters - arguments for the program
 *@param cellBinary - array of integers to store binary value
 *Function checks number of rows and columns. (Functions isValidNumber, isValidNumberFL called)
   Checks if every number is valid and if cells, that are next to each other have same type of border. (Function checkBorders called)
   If one of checks is false, false is returned. If every check is sucessful, true is returned.
 ***********************************************/
bool checkBorders(Map maptest);
/************************************************
 *@functionName:checkBorders
 *@param maptest - structure Map 
 *Calls function checkBorder if two cell have mutual border. If one border is not the same type as other one, false is returned.
   If border that cells are sharing is same type in both cells, true is returned.
 ***********************************************/
bool checkBorder(int firstCell, int border, int secondCell);
/************************************************
 *@functionName: checkBorder
 *@param firstCell-integer in decimal value, value of cell
 *@param border- integer, which borders to check, uses constants "DOWNUPBORDER" and "RIGHTLEFTBORDER"
 *@param secondCell-integer in decimal value, value of cell
 *Function converts decimal values of cells into binary number,that represents bordes of cell. Checks if shared border is the same value in both cells.
 If border is same in both cells true is returned, otherwise false is returned.
 ***********************************************/
void cellToBinary(unsigned number, int* cellBinary);
/************************************************
 *@functionName:cellToBinary
 *@param number - unsingned integer - number to convert
 *@param cellBinary - array of integers to store binary value
 *Function converts decimal number into binary
 ***********************************************/
bool checkFirstLine(char buffer[], int *numberCount, Map *maptest);
/************************************************
 *@functionName:checkFirstLine
 *@param buffer - line to check
 *@param numberCount - pointer to integer, counter of numbers
 *@param maptest - pointer to struct Map
 *Checks first line for invalid characters
 ***********************************************/


int main(int argc, char* argv[]){
	arguments(argc,argv);
	(void) argv;
}
void arguments(int argc,char* argv[]){
	if(argc==2 && strcmp( argv[1], ARGHELP) == 0){
		fprintf(stdout, "%s\n", HELP);
	}
	else if(argc==3 && strcmp( argv[1], ARGTEST) == 0){
		if(checkMap(argv)){
			fprintf(stdout, "Valid\n");
		} else fprintf(stderr, "Invalid\n");
	} 
}

bool isValidNumberFL(char c){
	if(c>='0' && c<='9') return true;
	else return false;
}

bool isValidNumber(char c){
	if(c>='0' && c<='7') return true;
	else return false;
}

bool areAllowedChars(bool firstLine,char string[]){
	int length = strlen(string);
	for(int i=0;i<length;i++){
		char c=string[i];
		if(firstLine){
			if( !isValidNumberFL(c) && c!='\n'&& c!='\0' && c!=' '){return false;}
		} else{
			if( !isValidNumber(c) && c!='\n'&& c!='\0' && c!=' '){return false;}
		}
	}
	return true; 
}

bool checkMap(char* argv[]){
	Map maptest;
	FILE* map = NULL; 
	map=fopen(argv[2],"r");
	int line=0;
	int cellsCounter=0;
	if(map!=NULL){
		char buffer[MAXBUFFER];
		while(fgets(buffer,MAXBUFFER,map)){
			int numberCount=0;
			if(line==0){
				if (!checkFirstLine(buffer, &numberCount, &maptest))
				{
					return false;
				}
			}
			else{
				if(!areAllowedChars(false,buffer)){ return false;};
				int linelen=strlen(buffer);
				for(int c=0;c<linelen;c++){
					if(isValidNumber(buffer[c])){
						numberCount++;
						maptest.cells[cellsCounter]=buffer[c];
						cellsCounter++;
						//printf("%c\n",buffer[c]);
					}
				}
				if(numberCount!=maptest.cols){ return false;}	
			}
			line++;

		}
		if(maptest.rows!=line-1){return false;}
	} else {
		fprintf(stderr, "Text file opening failed. ");
		return false;
	}
	if(!checkBorders(maptest)){return false;}
	free(maptest.cells);
	return true;
}

bool checkBorders(Map map){
	for(int row=0;row<map.rows;row++){
		for(int cell=0+(row*map.cols);cell<map.cols*(row+1);cell++){
			if(cell<map.cols*(row+1)-1){
				if(!checkBorder(map.cells[cell]-'0',RIGHTLEFTBORDER,map.cells[cell+1]-'0')){
					return false;
				}
			}
			if(row<map.rows-1){
				if ((cell & 1) != 0){
					if(!checkBorder(map.cells[cell]-'0',DOWNUPBORDER,map.cells[cell+map.cols]-'0')){
						return false;
					}
				} 
			}
		}
	}
	return true;
}

bool checkBorder(int firstCell, int border, int secondCell){
	//printf("První:%d, Druhá:%d, Pořadí bitu k porovnání:%d\n",firstCell,secondCell,border);
	int firstCellBinary[MAXBITS+1];
	cellToBinary(firstCell,firstCellBinary);
	int secondCellBinary[MAXBITS+1];
	cellToBinary(secondCell,secondCellBinary);		


	if(border==RIGHTLEFTBORDER){
		//printf("%d==%d\n",firstCellBinary[RIGHTBORDER]-'0',secondCellBinary[LEFTBORDER]-'0');
		if(firstCellBinary[RIGHTBORDER]-'0'!=secondCellBinary[LEFTBORDER]-'0'){
			return false;
		} 
	}

	else if(border==DOWNUPBORDER){
		//printf("%d==%d\n",firstCellBinary[HORIZONTBORDER],secondCellBinary[HORIZONTBORDER]);
		if(firstCellBinary[HORIZONTBORDER]-'0'!=secondCellBinary[HORIZONTBORDER]-'0'){
			return false;
		}

	}

	//printf("%d-%d%d%d\n",firstCell,firstCellBinary[0]-'0',firstCellBinary[1]-'0',firstCellBinary[2]-'0');
	//printf("%d-%d%d%d\n",secondCell,secondCellBinary[0]-'0',secondCellBinary[1]-'0',secondCellBinary[2]-'0');
	//printf("___\n");
	return true;
}

void cellToBinary(unsigned number, int* cellBinary) 
{   
	for (int i=0;i<MAXBITS;i++) { 
		cellBinary[i] = (number & (int)1<<(MAXBITS-i-1)) ? '1' : '0';
		cellBinary[MAXBITS]='\0';
	}
} 
bool checkFirstLine(char buffer[], int *numberCount, Map *maptest){
	if(!areAllowedChars(true,buffer)){ return false;};
				// Finds first two numbers and sets them as values of expectedC and expected R.
	int linelen=strlen(buffer);
	for(int c=0;c<linelen;c++){
		if(isValidNumberFL(buffer[c])){
			int number=buffer[c]-'0';
			if(isValidNumberFL(buffer[c+1])){
				number=number*10+(buffer[c+1]-'0');
				c++;
			}
						//printf("%d\n",number);
			++*numberCount;
			if(*numberCount==1){maptest->rows=number;}
			else if(*numberCount==2){
				maptest->cols=number;
				maptest->cells = malloc(sizeof(char)*maptest->cols*maptest->rows);
			}
			else {
				printf("%d\n",*numberCount);
			 return false;}
		}
	}
	return true;
}


