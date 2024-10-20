/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/*           Daniel Dolejška, září 2022                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
** uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

bool error_flag;
bool solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error(void) {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = true;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {
	//initializes an empty list
	list->activeElement = NULL;
	list->currentLength = 0;
	list->firstElement = NULL;
	list->lastElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {
	//will keep deleting 1st element of the list as long as the element exists
	while(list->firstElement != NULL){
			DLL_DeleteFirst(list);
	}
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, long data ) {
	//allocating memory for new node and checking if malloc was succesfull
	struct DLLElement *NewNode = malloc(sizeof(struct DLLElement));
	if(NewNode == NULL){
		DLL_Error();
		return;
	}
	
	NewNode->data = data;
	//case when list was empty
	if(list->currentLength == 0){
		list->firstElement = NewNode;
		list->lastElement = list->firstElement;
		NewNode->previousElement = NULL;
		NewNode->nextElement = NULL;
	}
	//case when list wasnt empty
	else{
		NewNode->nextElement = list->firstElement;
		list->firstElement->previousElement = NewNode;
		list->firstElement = NewNode;
	
	}
	//no matter which case, we increment list lenght and set previous element to NULL (Node is at 1st position)
	NewNode->previousElement = NULL;
	list->currentLength++;
	if((list->currentLength == 1)&&(list->firstElement != list->lastElement)){
		list->lastElement=list->firstElement;
	}	
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, long data ) {
	//allocating memory for new node and checking if malloc was succesfull
	struct DLLElement *NewNode = malloc(sizeof(struct DLLElement));
	if(NewNode == NULL){
		DLL_Error();
		return;
	}
	
	
	NewNode->data = data;
	//case when list was empty
	if(list->currentLength == 0){
		list->lastElement = NewNode;
		list->firstElement = NewNode;
		NewNode->previousElement = NULL;
	}
	//case when list wasnt empty;
	else{
		list->lastElement->nextElement = NewNode;
		NewNode->previousElement=list->lastElement;
		list->lastElement = NewNode;
		
	}
	//no matter which case, we increment list lenght and set next element to NULL (Node is at last position)
	NewNode->nextElement = NULL;
	list->currentLength++;
	if((list->currentLength == 1)&&(list->firstElement != list->lastElement)){
		list->lastElement=list->firstElement;
	}
	
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {
	//setting first element as active element
	list->activeElement = list->firstElement;


}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {
	//setting last element as active element
	list->activeElement = list->lastElement;


}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, long *dataPtr ) {
	//case when list is empty
	if(list->firstElement == NULL){
		DLL_Error();
	}
	//standard case
	else{
		*dataPtr=list->firstElement->data;
	}
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, long *dataPtr ) {
	//case when list is empty
	if(list->lastElement == NULL){
		DLL_Error();
	}
	//standard case
	else{
		*dataPtr=list->lastElement->data;
	}
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
	//case list is empty
	if(list->firstElement == NULL){
		return;
	}
	else{
		struct DLLElement *ToDelete = list->firstElement;
		//case list has only one element
		if(list->firstElement == list->lastElement){
			list->firstElement = NULL;
			list->activeElement = NULL;
			list->lastElement = NULL;
		}
		/*	standard case, we store node we want to delete in special node, 
			then we set the follower of the node as the 1st node,
			then the node can be deleted, without loosing all nodes after the node which will be deleted
		*/
		else{
			list->firstElement = list->firstElement->nextElement;
			list->firstElement->previousElement = NULL;
			
		}
		free(ToDelete);
		list->currentLength--;
		if((list->currentLength == 1)&&(list->firstElement != list->lastElement)){
			list->lastElement=list->firstElement;
		}
	}
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {
	//case list is empty
	if(list->firstElement == NULL){
		return;
	}
	else{ 
			struct DLLElement *ToDelete = list->lastElement;
			//case list has only one element
			if(list->lastElement == list->firstElement){
				list->lastElement = NULL;
				list->activeElement = NULL;
				list->firstElement = NULL;
			}
		
			/*	standard case, we store node we want to delete in special node, 
			then we set the follower of the node as the 1st node,
			then the node can be deleted, without loosing all nodes after the node which will be deleted
			*/
			else{
				/*if(list->activeElement == list->lastElement){
					list->activeElement = NULL;
				}
				if(list->lastElement->previousElement != NULL)
				*/
				list->lastElement = list->lastElement->previousElement;
				list->lastElement->nextElement = NULL;
			}
			free(ToDelete);
			list->currentLength--;
			if((list->currentLength == 1)&&(list->firstElement != list->lastElement)){
				list->lastElement=list->firstElement;
			}
		}
	}
	

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {

	//case list is not active, or 1st element is active element, we do nothing
	if((list->activeElement == NULL) || list->activeElement == list->firstElement){
		return;
	}
	//case active element is 2nd element, we free the 1st element and set active as 1st element
	else if(list->activeElement->previousElement == list->firstElement){
		free(list->firstElement);
		list->firstElement = list->activeElement;
		list->activeElement->previousElement = NULL;
	}
	else{
		//standard case, we create temporary element, which holds the element we want to delete
		//we then move the pointers accordingly, so list works without the todelete element
		//then we free the element and set temporary element to NULL, so no shenanigans can happen
		DLLElementPtr ToDelete = list->activeElement->previousElement;
		list->activeElement->previousElement = ToDelete->previousElement;
		ToDelete->previousElement->nextElement = list->activeElement;
		free(ToDelete);
		ToDelete = NULL;
	}
	//no matter the case we decrease lenght and check if the array hasnt got single element now
	list->currentLength--;
	if(list->currentLength == 1){
		list->lastElement = list->firstElement;
	}
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {
	
	//case list is not active, or last element is active element, we do nothing
	if(list->activeElement == list->lastElement || list->activeElement == NULL){
		return;
	}
	//case active element is pre-last element, we free the last element and set active as last element
	else if(list->activeElement->nextElement == list->lastElement){
		free(list->lastElement);
		list->lastElement = list->activeElement;
		list->activeElement->nextElement = NULL;
	}
		//standard case, we create temporary element, which holds the element we want to delete
		//we then move the pointers accordingly, so list works without the todelete element
		//then we free the element and set temporary element to NULL, so no shenanigans can happen
	
	else{
		DLLElementPtr ToDelete = list->activeElement->nextElement;
		list->activeElement->nextElement = ToDelete->nextElement;
		ToDelete->nextElement->previousElement = list->activeElement;
		free(ToDelete);
		ToDelete = NULL;
	}
	//no matter the case we decrease lenght and check if the array hasnt got single element now
	list->currentLength--;
	if(list->currentLength == 1){
		list->lastElement = list->firstElement;
	}
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, long data ) {
	//nothing happens if list is not active
	if(list->activeElement == NULL){
		return;
	}
	//allocating memory for new node and checking whether memory allocation was succesfull
	struct DLLElement *NewNode = malloc(sizeof(struct DLLElement));
	if(NewNode == NULL){
		DLL_Error();
		return;
	}
	//storing data in new node
	else{
		NewNode->data = data;
	}

	//case when node after active element is NULL
	if(list->activeElement->nextElement == NULL){
		list->activeElement->nextElement = NewNode;
		list->lastElement = NewNode;
		NewNode->previousElement = list->activeElement;
		NewNode->nextElement = NULL;
	}
	//standard case, we insert the new node between active element and element it points to
	else{
		NewNode->nextElement = list->activeElement->nextElement;
		NewNode->previousElement = list->activeElement;
		list->activeElement->nextElement = NewNode;
	}
	//no matter which case, we always increase list lenght  and check whether list hasnt got single element
	list->currentLength++;
	if((list->currentLength == 1)&&(list->firstElement != list->lastElement)){
		list->lastElement=list->firstElement;
	}
	

}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, long data ) {
	//nothing happens if list is not active
	if(list->activeElement == NULL){
		return;
	}
	//allocating memory for new node and checking whether memory allocation was succesfull
	struct DLLElement *NewNode = malloc(sizeof(struct DLLElement));
	if(NewNode == NULL){
		DLL_Error();
		return;
	}
	else{
		NewNode->data = data;
	}
	//case when node after active element is NULL
	if(list->activeElement->previousElement == NULL){
		list->activeElement->previousElement = NewNode;
		list->firstElement = NewNode;
		NewNode->nextElement = list->activeElement;
		NewNode->previousElement = NULL;
	}
	//standard case, we insert the new node between active element and element it points to
	else{
		NewNode->previousElement = list->activeElement->previousElement;
		list->activeElement->previousElement = NewNode;
	}
	//no matter which case, we always increase list lenght  and check whether list hasnt got single element
	list->currentLength++;
	if((list->currentLength == 1)&&(list->firstElement != list->lastElement)){
		list->lastElement=list->firstElement;
	}
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, long *dataPtr ) {
	//if lisnt is active, stores value of active element in dataPtr, if it isnt, error occures
	if(DLL_IsActive(list)){
		*dataPtr=list->activeElement->data;
	}
	else{
		DLL_Error();
	}
	return;
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, long data ) {
	//sets value of active element to value provided in data variable
	if(DLL_IsActive(list)){
		list->activeElement->data=data;
	}
	return;
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
	//if list is active, sets element after active element as the new active element
	if(DLL_IsActive(list)){
		list->activeElement = list->activeElement->nextElement;
	}
	return;
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
	//if list is active, sets element before active element as the new active element
	if(DLL_IsActive(list)){
		list->activeElement = list->activeElement->previousElement;
	}
	return;
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
bool DLL_IsActive( DLList *list ) {
	//returns 1 if list is active, 0 if it isnt
	return (list->activeElement != NULL);
}

/* Konec c206.c */
