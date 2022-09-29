#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define CONTACTS_ROWS 84
#define CONTACTS_COLUMNS 103
#define NUMBERTWO "ABCabc"
#define NUMBERTHREE "DEFdef"
#define NUMBERFOUR "GHIghi"
#define NUMBERFIVE "JKLjkl"
#define NUMBERSIX "MNOmno"
#define NUMBERSEVEN "PQRSpqrs"
#define NUMBEREIGHT "TUVtuv"
#define NUMBERNINE "WXYZwxyz"
#define NUMBERZERO '+'



/**
 * @author xtomal02 (Tomáš Tomala)
 * @file   proj1.c
 */

/************************************************
 *@functionName:loadAllContacts
 *@param Function requires one parameter - two dimensional array with given dimensions.
 *Function reads standard input and if there is a line, it adds it to the array.
 *Returns integer - number of lines in the array.
 ***********************************************/
int loadAllContacts(char contacts[CONTACTS_ROWS][CONTACTS_COLUMNS]);
/************************************************
 *@functionName:printArray
 *@param Function requires two parameters - two dimensional array with given dimensions and number of rows of the array.
 *Function concatenates name of contact and his number onto one line and prints it on standard output.
 *Function doesnt return anything.
 ***********************************************/
void printArray(char contacts[CONTACTS_ROWS][CONTACTS_COLUMNS], int rowCount);
/************************************************
 *@functionName:upperToLowercase
 *@params Function requires one parameter - two dimensional array with given dimensions.
 *Function goes through the array, checks every character and if the character is upper-case, character is converted to lower-case.
 *Function doesnt return anything.
 ***********************************************/
void upperToLowercase(char contacts[CONTACTS_ROWS][CONTACTS_COLUMNS]);
/************************************************
 *@functionName:charsToNumber
 *@param Function requires two parameters - two dimensional array with given dimensions and number of rows of the array.
 *Function goes through the array, checks every character. If one of the given strings contains the character, the character 
    is converted to the number associated with the given string.
 *Function doesnt return anything.
 ***********************************************/
void charsToNumber(char contacts[CONTACTS_ROWS][CONTACTS_COLUMNS], int rowCount);
/************************************************
 *@functionName:searchContactStandard
 *@params Function requires four parameters - array of chars that represent the contact to search,
    two dimensional array with given dimensions with all contacts,  two dimensional array with given dimensions for found contacts and count of all contacts.
 *Function creates an array and stores contacts as a number converted by function "charsToNumber", then compares contacts with given contact as parameter. If contact is found, it is 
    stored into an array.
 *Returns integer - how many contacts were found
 ***********************************************/
int searchContactStandard(char contact[CONTACTS_COLUMNS],char contacts[CONTACTS_ROWS][CONTACTS_COLUMNS], 
                        char foundContacts[CONTACTS_ROWS][CONTACTS_COLUMNS] ,int rowCount);
/************************************************
 *@functionName:searchContactPremium
 *@params Function requires four parameters - array of chars that represent the contact to search,
    two dimensional array with given dimensions with all contacts,  two dimensional array with given dimensions for found contacts and count of all contacts.
 *Function creates an array and stores contacts as a number converted by function "charsToNumber", then compares contacts with given contact as parameter. 
    Comparing contacts is done by function "hasContactChars" If contact is found, it is stored into an array.
 *Returns integer - how many contacts were found
 ***********************************************/
int searchContactPremium(char contact[CONTACTS_COLUMNS],char contacts[CONTACTS_ROWS][CONTACTS_COLUMNS], 
                        char foundContacts[CONTACTS_ROWS][CONTACTS_COLUMNS] ,int rowCount);

/************************************************
 *@functionName:hasContactChars
 *@params Function requires three parameters - three arrays of characters for name, number and searched characters
 *Function concatenates number to name. Characters from third array are then searched from first two character arrays, which are joined.
 *Function searches characters one by one and in order. If first character is never found, function ends .
 *Returns bool - true - contact had all characters and they were in order, false - character didnt have all characters or they werent in order
 ***********************************************/
bool hasContactChars(char *name,char *number,char *chars);


int main(int argc, char* argv[]){
    char contacts[CONTACTS_ROWS][CONTACTS_COLUMNS];
    char foundContacts[CONTACTS_ROWS][CONTACTS_COLUMNS];
    int numberOfContacts=loadAllContacts(contacts);
    if (numberOfContacts==-1)
    {
        return 1;
    }
    upperToLowercase(contacts);
/************************************************
 *Check if there is more than one argument. If not, all contacts are written out.
 *If there is more than one argument, searching contacts is done with second argument. If second argument contains character, error report is printed out.
 *If there isnt any contact found, report is printed out on standard error output. Otherwise, contacts that have been found are printed by function "printArray"
 ***********************************************/
    if(argc>1){
        int argLength=strlen(argv[1]);
        for(int i=0; i<argLength; i++){
            if(argv[1][i]>65 && argv[1][i]<122){
                fprintf(stderr, "Please search numbers, not characters\n");
                return 1;
            }
        }

        int foundContactsCount=searchContactPremium(argv[1],contacts,foundContacts,numberOfContacts);
        if(foundContactsCount==0){fprintf(stderr, "Not found\n");}
        else{printArray(foundContacts,foundContactsCount*2);
        }
    }
    else{
        printArray(contacts, numberOfContacts);
    }
    (void) argc;
    (void) argv;
    return 0;
}
int loadAllContacts(char contacts[CONTACTS_ROWS][CONTACTS_COLUMNS]){
    char line[CONTACTS_COLUMNS];
    int lineNumber = 0;
    int failure=0;
    while(fgets(line,sizeof line,stdin)!=NULL){
        if (strlen(line)>101)
        {
            failure=1;
            fprintf(stderr, "Invalid data:contact can have maximum of 100 characters\n");
            
        }
        strcpy(contacts[lineNumber],line);
        lineNumber++;
    }
    if(failure==0) return lineNumber;
    else return -1;
}

void printArray(char array[CONTACTS_ROWS][CONTACTS_COLUMNS], int rowCount){

 for (int i = 0; i < rowCount; i+=2) {
    array[i][strlen(array[i])-1]=',';
    strcat(array[i]," ");
    strcat(array[i],array[i+1]);
    fprintf(stdout, "%s",array[i]);

 }
}

void upperToLowercase(char contacts[CONTACTS_ROWS][CONTACTS_COLUMNS]){
    for (int i = 0; i < CONTACTS_ROWS; i++) {
        for (int j = 0; j < CONTACTS_COLUMNS; j++) {
            if(contacts[i][j]>=65 && contacts[i][j]<=90){
                contacts[i][j] = contacts[i][j] + 32;
            }
        }
    }
}

void charsToNumber(char contacts[CONTACTS_ROWS][CONTACTS_COLUMNS],int rowCount){
 for (int i = 0; i < rowCount; i++) {
        int lineLength = strlen(contacts[i]);
    for (int j = 0; j < lineLength; j++) {
        int number=0;
        if(strchr(NUMBERTWO,contacts[i][j])){number=1;}
        else if(strchr(NUMBERTHREE,contacts[i][j])){number=2;}
        else if(strchr(NUMBERFOUR,contacts[i][j])){number=3;}
        else if(strchr(NUMBERFIVE,contacts[i][j])){number=4;}
        else if(strchr(NUMBERSIX,contacts[i][j])){number=5;}
        else if(strchr(NUMBERSEVEN,contacts[i][j])){number=6;}
        else if(strchr(NUMBEREIGHT,contacts[i][j])){number=7;}
        else if(strchr(NUMBERNINE,contacts[i][j])){number=8;}
        else if(contacts[i][j]==NUMBERZERO){number=9;}
        switch(number){
            case 0: contacts[i][j] =contacts[i][j]; break;
            case 1: contacts[i][j] ='2'; break;
            case 2: contacts[i][j] ='3'; break;
            case 3: contacts[i][j] ='4'; break;
            case 4: contacts[i][j] ='5'; break;
            case 5: contacts[i][j] ='6'; break;
            case 6: contacts[i][j] ='7'; break;
            case 7: contacts[i][j] ='8'; break;
            case 8: contacts[i][j] ='9'; break;
            case 9: contacts[i][j] ='0'; break;
            default: contacts[i][j] = contacts[i][j]; break;
        }
    }
}
}
int searchContactStandard(char contact[CONTACTS_COLUMNS],char contacts[CONTACTS_ROWS][CONTACTS_COLUMNS], char foundContacts[CONTACTS_ROWS][CONTACTS_COLUMNS] ,int rowCount){
    char contactsConverted[CONTACTS_ROWS][CONTACTS_COLUMNS];
    int arraySize = sizeof(char) * CONTACTS_ROWS * CONTACTS_COLUMNS;
    int foundContactsCount=0;
    memcpy(contactsConverted, contacts, arraySize);
    charsToNumber(contactsConverted,rowCount);
   for (int i = 0; i <rowCount ; i=i+2)
    {
        if(strstr(contactsConverted[i], contact) != NULL || strstr(contactsConverted[i+1], contact) != NULL) {
            strcpy(foundContacts[foundContactsCount*2],contacts[i]);
            strcpy(foundContacts[(foundContactsCount*2)+1],contacts[i+1]);
            foundContactsCount++;
        } 
    }
    return foundContactsCount;
}

bool hasContactChars(char *name,char *number,char *chars){
    strcat(name,number);
    int charsCount=strlen(chars);
    int nameLength = strlen(name);
    int positionOfSearchedChar=0;
    for(int i=0;i<nameLength;i++){
        if(name[i]==chars[positionOfSearchedChar]){
            if(positionOfSearchedChar==charsCount-1){
                return true;
            }
                else{positionOfSearchedChar++;
                }
        }   
    }
    return false;
}

int searchContactPremium(char contact[CONTACTS_COLUMNS],char contacts[CONTACTS_ROWS][CONTACTS_COLUMNS], char foundContacts[CONTACTS_ROWS][CONTACTS_COLUMNS] ,int rowCount){
    char contactsConverted[CONTACTS_ROWS][CONTACTS_COLUMNS];
    int arraySize = sizeof(char) * CONTACTS_ROWS * CONTACTS_COLUMNS;
    int foundContactsCount=0;
    memcpy(contactsConverted, contacts, arraySize);
    charsToNumber(contactsConverted,rowCount);

   for (int i = 0; i <rowCount ; i=i+2)
    {
        if(hasContactChars(contactsConverted[i],contactsConverted[i+1], contact)){
            strcpy(foundContacts[foundContactsCount*2],contacts[i]);
            strcpy(foundContacts[(foundContactsCount*2)+1],contacts[i+1]);
            foundContactsCount++;
        } 
    }
    return foundContactsCount;
}    