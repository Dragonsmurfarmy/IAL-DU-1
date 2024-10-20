/*
 *  Předmět: Algoritmy (IAL) - FIT VUT v Brně
 *  Rozšíření pro příklad c206.c (Dvousměrně vázaný lineární seznam)
 *  Vytvořil: Daniel Dolejška, září 2024
 */

#include "c206-ext.h"
void CreateDLLElementList(DLLElementPtr list, DLList* PacketList);
void InsertPacket(DLList* list, long packet);
void InsertPriority(QosPacketListPtr list, PacketPtr packet);
void createDLList(DLList* list);

bool error_flag;
bool solved;


/**
 * Tato metoda simuluje příjem síťových paketů s určenou úrovní priority.
 * Přijaté pakety jsou zařazeny do odpovídajících front dle jejich priorit.
 * "Fronty" jsou v tomto cvičení reprezentovány dvousměrně vázanými seznamy
 * - ty totiž umožňují snazší úpravy pro již zařazené položky.
 * 
 * Parametr `packetLists` obsahuje jednotlivé seznamy paketů (`QosPacketListPtr`).
 * Pokud fronta s odpovídající prioritou neexistuje, tato metoda ji alokuje
 * a inicializuje. Za jejich korektní uvolnení odpovídá volající.
 * 
 * V případě, že by po zařazení paketu do seznamu počet prvků v cílovém seznamu
 * překročil stanovený MAX_PACKET_COUNT, dojde nejdříve k promazání položek seznamu.
 * V takovémto případě bude každá druhá položka ze seznamu zahozena nehledě
 * na její vlastní prioritu ovšem v pořadí přijetí.
 * 
 * @param packetLists Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param packet Ukazatel na strukturu přijatého paketu
 */
void receive_packet( DLList *packetLists, PacketPtr packet ){
	//checking whether there are elements in packetLists
	if(packetLists == NULL){
		return;
	}
	//checking whether packet priority doesnt already exists, in list, 
	//if it does, we insert the packet
	DLLElementPtr current = packetLists->firstElement;
	while(current != NULL){
		QosPacketListPtr currList = (QosPacketListPtr)(current->data);
		if(current->data == packet->priority){
			DLL_InsertAfter(currList->list,(long)packet->id);
			return;
		}
		else{
			current = current->nextElement;
		}
	}
	//no matching list exists, so we create a new one
	CreateDLLElementList(current,packetLists);
	
	//if packetLists is empty, we insert NewList as a new element, otherwise we insert it as last element
	
	DLList* dataList = NULL;
	createDLList(dataList);
	current->data = (long)dataList;
	((QosPacketListPtr)current->data)->list = dataList;

	//inserting packet into the NewList and setting the data back to the original long value
	InsertPacket(dataList,packet->id);
	InsertPriority(((QosPacketListPtr)current->data),packet);
	current->data=(long)dataList;

	return;

}

/**
 * Tato metoda simuluje výběr síťových paketů k odeslání. Výběr respektuje
 * relativní priority paketů mezi sebou, kde pakety s nejvyšší prioritou
 * jsou vždy odeslány nejdříve. Odesílání dále respektuje pořadí, ve kterém
 * byly pakety přijaty metodou `receive_packet`.
 * 
 * Odeslané pakety jsou ze zdrojového seznamu při odeslání odstraněny.
 * 
 * Parametr `packetLists` obsahuje ukazatele na jednotlivé seznamy paketů (`QosPacketListPtr`).
 * Parametr `outputPacketList` obsahuje ukazatele na odeslané pakety (`PacketPtr`).
 * 
 * @param packetLists Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param outputPacketList Ukazatel na seznam paketů k odeslání
 * @param maxPacketCount Maximální počet paketů k odeslání
 */
void send_packets( DLList *packetLists, DLList *outputPacketList, int maxPacketCount ){
	solved = false; /* V případě řešení, smažte tento řádek! */
}

void CreateDLLElementList(DLLElementPtr list, DLList* packetLists){
		//malloc check
	list = malloc(sizeof(DLList));
	if(list == NULL){
		return;
	}
	//init list
	list->data = 0;
	list->nextElement = NULL;
	list->previousElement = NULL;

	//case list of lists is empty
	if(packetLists->firstElement == NULL){
		packetLists->activeElement = list;
		packetLists->firstElement = list;
		packetLists->lastElement = list;
	}
	//case list of lists is not empty
	else{
		packetLists->lastElement->nextElement = list;
		list->previousElement = packetLists->lastElement;
		packetLists->lastElement = list;
	}
	list->data = (long)&packetLists->lastElement;
}


void InsertPacket(DLList* list, long packet){
	if(list->firstElement == NULL){
		DLL_InsertFirst(list, packet);
	}
	else if(!DLL_IsActive(list)){
		list->activeElement = list->lastElement;
	}
	DLL_InsertAfter(list, packet);
	return;
}

void InsertPriority(QosPacketListPtr list, PacketPtr packet){
	list->priority = packet->priority;
}

void createDLList(DLList* list){
	//malloc check
	list = malloc(sizeof(DLList));
	if(list == NULL){
		return;
	}
	//initialization
	list->currentLength = 0;
	list->activeElement = NULL;
	list->firstElement = NULL;
	list->lastElement = NULL;
}

