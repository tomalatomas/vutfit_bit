//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - priority queue code
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     JMENO PRIJMENI <xlogin00@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author Tomas Tomala
 * 
 * @brief Implementace metod tridy prioritni fronty.
 */

#include <stdlib.h>
#include <stdio.h>

#include "tdd_code.h"

//============================================================================//
// ** ZDE DOPLNTE IMPLEMENTACI **
//
// Zde doplnte implementaci verejneho rozhrani prioritni fronty (Priority Queue)
// 1. Verejne rozhrani fronty specifikovane v: tdd_code.h (sekce "public:")
//    - Konstruktor (PriorityQueue()), Destruktor (~PriorityQueue())
//    - Metody Insert/Remove/Find a GetHead
//    - Pripadne vase metody definovane v tdd_code.h (sekce "protected:")
//
// Cilem je dosahnout plne funkcni implementace prioritni fronty implementovane
// pomoci tzv. "double-linked list", ktera bude splnovat dodane testy 
// (tdd_tests.cpp).
//============================================================================//

PriorityQueue::PriorityQueue()
{
	root=NULL;
}

PriorityQueue::~PriorityQueue()
{
	Element_t *current=GetHead();
	Element_t *next=NULL;
	while(current!=NULL){
		next=current->pNext;
		delete current;
		current=next;
	}
}

void PriorityQueue::Insert(int value)
{
	Element_t *newElement= new Element_t;
	newElement->pNext=NULL;
	newElement->pPrev=NULL;
	newElement->value=value;
	if(GetHead()==NULL){
		//Empty list
		root=newElement;	
	}
	else if(GetHead()->value>value){
		//Nonempty list, newElement is the new root
		newElement->pNext=GetHead();
		root=newElement;
		newElement->pNext->pPrev=root;
	}
	else{
		Element_t *current=GetHead();
		while(current!=NULL){
			//New element is the tail
				if(current->pNext==NULL){
					current->pNext=newElement;
					newElement->pPrev=current;
					break;
				}
				else if(current->value<value && value<current->pNext->value){
					//New element is between two other elements
						newElement->pNext=current->pNext;
						newElement->pPrev=current;
						current->pNext=newElement;
						newElement->pNext->pPrev=newElement;
						break;
				}
			current=current->pNext;
		}

	}
}

bool PriorityQueue::Remove(int value)
{
	Element_t *searched=Find(value);
	if(searched==NULL) return false;
	if(searched==GetHead()&& searched->pNext==NULL){
		//The queue consists of only one element (root)
		root=NULL;
	}
	else if(searched==GetHead()&& searched->pNext!=NULL){
		//The element to remove is the head and the queue consists of other elements also
		root=root->pNext;
		root->pPrev=NULL;
	}
	else if(searched->pNext==NULL){
		//The element to remove is the last one (tail)
		searched->pPrev->pNext=NULL;
	}
	else if(searched->pNext!=NULL && searched->pPrev!=NULL){
		//The element to remove is between two other elements
		Element_t *previous=searched->pPrev;
		Element_t *next=searched->pNext;
		next->pPrev=searched->pPrev;
		previous->pNext=searched->pNext;
		delete previous;
		delete next;
	}
	delete searched;
	return true;
}

PriorityQueue::Element_t *PriorityQueue::Find(int value)
{
	Element_t *searched=NULL;
    Element_t *current=GetHead();
    if(current!=NULL){
    	while(current->pNext!=NULL){
    		if(current->value==value){
    			searched=current;
    			return searched;
    		}
    		current=current->pNext;
    	}
    	if(current->value==value){
    			searched=current;
    		}
    }
    //delete current;
    return searched;

}

PriorityQueue::Element_t *PriorityQueue::GetHead()
{
    return root;
}

/*** Konec souboru tdd_code.cpp ***/
