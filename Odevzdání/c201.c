/* c201.c **********************************************************************
** Téma: Jednosměrný lineární seznam
**
**                     Návrh a referenční implementace: Petr Přikryl, říjen 1994
**                                          Úpravy: Andrea Němcová listopad 1996
**                                                   Petr Přikryl, listopad 1997
**                                Přepracované zadání: Petr Přikryl, březen 1998
**                                  Přepis do jazyka C: Martin Tuček, říjen 2004
**                                              Úpravy: Kamil Jeřábek, září 2020
**                                                    Daniel Dolejška, září 2021
**                                                    Daniel Dolejška, září 2022
**
** Implementujte abstraktní datový typ jednosměrný lineární seznam.
** Užitečným obsahem prvku seznamu je celé číslo typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou typu List.
** Definici konstant a typů naleznete v hlavičkovém souboru c201.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ List:
**
**      List_Dispose ....... zrušení všech prvků seznamu,
**      List_Init .......... inicializace seznamu před prvním použitím,
**      List_InsertFirst ... vložení prvku na začátek seznamu,
**      List_First ......... nastavení aktivity na první prvek,
**      List_GetFirst ...... vrací hodnotu prvního prvku,
**      List_DeleteFirst ... zruší první prvek seznamu,
**      List_DeleteAfter ... ruší prvek za aktivním prvkem,
**      List_InsertAfter ... vloží nový prvek za aktivní prvek seznamu,
**      List_GetValue ...... vrací hodnotu aktivního prvku,
**      List_SetValue ...... přepíše obsah aktivního prvku novou hodnotou,
**      List_Next .......... posune aktivitu na další prvek seznamu,
**      List_IsActive ...... zjišťuje aktivitu seznamu.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam předá
** někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**/

#include "c201.h"

#include <stdio.h> // printf
#include <stdlib.h> // malloc, free


bool error_flag;
bool solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě. Nastaví error_flag na logickou 1.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void List_Error(void) {

	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = true;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným
 * seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
 * že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu jednosměrně vázaného seznamu
 */
void List_Init( List *list ) {
	//set all list struct elements to 0 values
	list->activeElement=NULL;
	list->currentLength=0;
	list->firstElement=NULL;

}

/**
 * Zruší všechny prvky seznamu list a uvede seznam list do stavu, v jakém se nacházel
 * po inicializaci. Veškerá paměť používaná prvky seznamu list bude korektně
 * uvolněna voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu jednosměrně vázaného seznamu
 **/
void List_Dispose( List *list ) {
//delete first element as long as 1st element is not empty
	while(list->firstElement != NULL){
			List_DeleteFirst(list);
	}

}

/**
 * Vloží prvek s hodnotou data na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci List_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu jednosměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void List_InsertFirst( List *list, int data ) {
	//allocating memory for new node and checking if malloc was succesfull
	struct ListElement *NewNode = malloc(sizeof(struct ListElement));
	if(NewNode == NULL){
		List_Error();
		return;
	}
	
	NewNode->data = data;
	//case when list was empty
	if(list->firstElement == NULL){
		list->firstElement = NewNode;
		NewNode->nextElement = NULL;
	}
	//case when list wasnt empty
	else{
		NewNode->nextElement = list->firstElement;
		list->firstElement = NewNode;
		list->currentLength++;
	}
}

/**
 * Nastaví aktivitu seznamu list na jeho první prvek.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 * @param list Ukazatel na inicializovanou strukturu jednosměrně vázaného seznamu
 */
void List_First( List *list ) {
	//setting first element as active element
	list->activeElement = list->firstElement;

}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci List_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu jednosměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void List_GetFirst( List *list, int *dataPtr ) {
	//case when list is empty
	if(list->firstElement == NULL){
		List_Error();
	}
	//standard case
	else{
		*dataPtr=list->firstElement->data;
	}
}

/**
 * Zruší první prvek seznamu list a uvolní jím používanou paměť.
 * Pokud byl rušený prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu jednosměrně vázaného seznamu
 */
void List_DeleteFirst( List *list ) {
	//case list is empty
	if(list->firstElement == NULL){
		return;
	}
	//case list has only one element
	else if(list->firstElement->nextElement == NULL){
		free(list->firstElement);
		list->firstElement = NULL;
	}
	/*	standard case, we store node we want to delete in special node, 
		then we set the follower of the node as the 1st node,
		then the node can be deleted, without loosing all nodes after the node which will be deleted
	*/
	else{
		struct ListElement *ToDelete = list->firstElement;
		list->firstElement = list->firstElement->nextElement;
		free(ToDelete);
		ToDelete = NULL;
	}
	list->currentLength--;
	
}

/**
 * Zruší prvek seznamu list za aktivním prvkem a uvolní jím používanou paměť.
 * Pokud není seznam list aktivní nebo pokud je aktivní poslední prvek seznamu list,
 * nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu jednosměrně vázaného seznamu
 */
void List_DeleteAfter( List *list ) {
	//nothing happens when list is not active or last node of the list is active 
	if(!(List_IsActive(list))){
		return;
	}

	if(list->activeElement->nextElement == NULL){
		return;
	}
	/*	standard case, we store node we want to delete in special node, 
		then we set the follower of the node as the 1st node,
		then the node can be deleted, without loosing all nodes after the node which will be deleted
	*/
	struct ListElement *ToDelete = list->activeElement->nextElement;
	list->activeElement->nextElement = ToDelete->nextElement;
	list->currentLength--;
	free(ToDelete);
}

/**
 * Vloží prvek s hodnotou data za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje!
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * zavolá funkci List_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu jednosměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void List_InsertAfter( List *list, int data ) {
	//nothing happens if list is not active
	if(list->activeElement == NULL){
		return;
	}
	//allocating memory for new node and checking whether memory allocation was succesfull
	struct ListElement *NewNode = malloc(sizeof(struct ListElement));
	if(NewNode == NULL){
		List_Error();
		return;
	}

	NewNode->data = data;
	//case when node after active element is NULL
	if(list->activeElement->nextElement == NULL){
		list->activeElement->nextElement = NewNode;
		NewNode->nextElement = NULL;
		list->currentLength++;
	}
	//standard case, we insert the new node between active element and element it points to
	else{
		NewNode->nextElement = list->activeElement->nextElement;
		list->activeElement->nextElement = NewNode;
		list->currentLength++;
	}
	
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam není aktivní, zavolá funkci List_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu jednosměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void List_GetValue( List *list, int *dataPtr ) {
	//if List is not active, error occures, if it is, we extract value of active element
	if(List_IsActive(list)){
		*dataPtr=list->activeElement->data;
	}
	else{
		List_Error();
	}
	return;
}

/**
 * Přepíše data aktivního prvku seznamu list hodnotou data.
 * Pokud seznam list není aktivní, nedělá nic!
 *
 * @param list Ukazatel na inicializovanou strukturu jednosměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void List_SetValue( List *list, int data ) {
	//if list is active, changes value of active node to the "data" number
	if(List_IsActive(list)){
		list->activeElement->data=data;
	}
	return;
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Všimněte si, že touto operací se může aktivní seznam stát neaktivním.
 * Pokud není předaný seznam list aktivní, nedělá funkce nic.
 *
 * @param list Ukazatel na inicializovanou strukturu jednosměrně vázaného seznamu
 */
void List_Next( List *list ) {
	//if list is active, makes follower of active element new active element
	if(List_IsActive(list)){
		list->activeElement = list->activeElement->nextElement;
	}
	return;
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Tuto funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu jednosměrně vázaného seznamu
 */
int List_IsActive( List *list ) {
	//returns true when list is active, or false when list is not active
	return (list->activeElement != NULL);
}

/* Konec c201.c */
