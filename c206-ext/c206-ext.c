/*
 *  Předmět: Algoritmy (IAL) - FIT VUT v Brně
 *  Rozšíření pro příklad c206.c (Dvousměrně vázaný lineární seznam)
 *  Vytvořil: Daniel Dolejška, září 2024
 */

#include "c206-ext.h"

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
void receive_packet( DLList *packetLists, PacketPtr packet ) {
	if (packetLists != NULL && packet != NULL) {

		DLList * prioListPtr = NULL;

		//create new priority list 
		DLL_First(packetLists);

			//insert new packetListH in order so list remains sorted
		if (DLL_IsActive(packetLists) == false) {
			QosPacketListPtr newPacketListHPtr = (QosPacketListPtr)malloc(sizeof(QosPacketList));
			DLList *         newPrioListPtr     = (DLList *)malloc(sizeof(DLList));
			
			DLL_InsertFirst(packetLists, (long)newPacketListHPtr);

			//set packetListH parameters
			newPacketListHPtr->priority = packet->priority;
			newPacketListHPtr->list = newPrioListPtr;

			//initialize and bind 
			DLL_Init(newPrioListPtr);
			prioListPtr = newPrioListPtr;
		} else {
			while (DLL_IsActive(packetLists)) {
				QosPacketListPtr packetListHPtr = (QosPacketListPtr)packetLists->activeElement->data;

				if (packet->priority == packetListHPtr->priority) {
					prioListPtr = packetListHPtr->list;

					break;

				} else if (packet->priority > packetListHPtr->priority) {
					QosPacketListPtr newPacketListHPtr = (QosPacketListPtr)malloc(sizeof(QosPacketList));
					DLList *         newPrioListPtr     = (DLList *)malloc(sizeof(DLList));

					DLL_InsertBefore(packetLists, (long)newPacketListHPtr);

					//set packetListH parameters
					newPacketListHPtr->priority = packet->priority;
					newPacketListHPtr->list = newPrioListPtr;

					//initialize and connect
					DLL_Init(newPrioListPtr);
					prioListPtr = newPrioListPtr;

					break;
				}

				DLL_Next(packetLists);
				if (DLL_IsActive(packetLists) == false) {
					QosPacketListPtr newPacketListHPtr = (QosPacketListPtr)malloc(sizeof(QosPacketList));
					DLList *         newPrioListPtr     = (DLList *)malloc(sizeof(DLList));

					DLL_InsertLast(packetLists, (long)newPacketListHPtr);

					//set packetListH parameters
					newPacketListHPtr->priority = packet->priority;
					newPacketListHPtr->list = prioListPtr;

					//initialize and connect
					DLL_Init(newPrioListPtr);
					prioListPtr = newPrioListPtr;
				}
			}
		}

		//add packet to queue
		if (prioListPtr != NULL) {
			if (prioListPtr->currentLength >= MAX_PACKET_COUNT) {
				for (DLL_First(prioListPtr); DLL_IsActive(prioListPtr); DLL_Next(prioListPtr)) {
					DLL_DeleteAfter(prioListPtr);
				}
			}
			DLL_InsertLast(prioListPtr, (long)packet);
		}
	}
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
void send_packets( DLList *packetLists, DLList *outputPacketList, int maxPacketCount ) {
	int packetsInList = 0;
	DLL_First(packetLists);
	while(DLL_IsActive(packetLists)){
		QosPacketListPtr packetListH = (QosPacketListPtr)packetLists->activeElement->data;
		DLList *         prioList     = packetListH->list;

		DLL_First(prioList);
		while(DLL_IsActive(prioList)){
			PacketPtr packetElementPtr = (PacketPtr)prioList->activeElement->data;

			DLL_InsertLast(outputPacketList, (long)packetElementPtr);
			DLL_Next(prioList);
			if (DLL_IsActive(prioList)) {
				DLL_DeleteBefore(prioList);
			} else {
				//last remaining element is deleted
				DLL_DeleteLast(prioList);
			}
			packetsInList++;

			if (packetsInList >= maxPacketCount) {
				return;
			}
		}
		DLL_Next(packetLists);
	}
}

