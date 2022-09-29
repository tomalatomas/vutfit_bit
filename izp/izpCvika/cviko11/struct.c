/**
Projekt:    Kostra 9. cviceni IZP 2015
Autor:      Marek Zak <izakmarek@fit.vutbr.cz>
Datum:      28. 11. 2015
*/

#include "struct.h"

/**
 * Inizializace objektu. Název objektu kopíruje. Objekt bude mít název
 * roven NULL, pokud se inicializace nezdařila.
 */
void object_ctor(Object *o, int id, char *name)
{
	o->name = malloc(sizeof(name)+1);
	if(name!=NULL){
		strcpy(o->name,name);
		o->id=id;}
	}


/**
 * Záměna dat dvou objektù.
 */
void object_swap(Object *i1, Object *i2)
{
	Object tmp= *i2;
	*i2=*i1;
	*i1=tmp;
	
}
 
/**./
 * Hluboká kopie objektu src na dst. Vrací dst, pokud se operace povedla,
 * jinak NULL.
 */
Object *object_cpy(Object *dst, Object *src)
{	
	if(src->name != NULL){
	int namesize = strlen(src->name)+1;
	dst->name=malloc(sizeof(char)*namesize);
	strcpy(dst->name,src->name);
	dst->id=src->id;
	return dst;
	/*if(dst->id==src->id && strcmp(src->name,dst->name)){
		return dst;
	} else {
		return NULL;
	}*/
}
}
/**
 * Uvolní objekt (resp. jeho jméno) z paměti. 
 * Nastavi jmeno objektu na NULL.
 */
void object_dtor(Object *o)
{
	free(o->name);
}
/**
 * Vytiske hodnoty daneho objektu.
 */
void print_object(Object *o)
{
    printf("ID: %d, NAME: %s\n", o->id, o->name);
}
