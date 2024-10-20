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

		DLList * priorityListPtr = NULL;

		// create new priority list or 
		DLL_First(packetLists);

			// insert new packetListHeader in order (list remains sorted)
		if (DLL_IsActive(packetLists) == false) {
			QosPacketListPtr newPacketListHeaderPtr = (QosPacketListPtr)malloc(sizeof(QosPacketList));
			DLList *         newPriorityListPtr     = (DLList *)malloc(sizeof(DLList));
			
			DLL_InsertFirst(packetLists, (long)newPacketListHeaderPtr);

			// set packetListHeader parameters
			newPacketListHeaderPtr->priority = packet->priority;
			newPacketListHeaderPtr->list = newPriorityListPtr;

			// initialize and connect 
			DLL_Init(newPriorityListPtr);
			priorityListPtr = newPriorityListPtr;
		} else {
			while (DLL_IsActive(packetLists)) {
				QosPacketListPtr packetListHeaderPtr = (QosPacketListPtr)packetLists->activeElement->data;

				if (packet->priority == packetListHeaderPtr->priority) {
					priorityListPtr = packetListHeaderPtr->list;

					break;

				} else if (packet->priority > packetListHeaderPtr->priority) {
					QosPacketListPtr newPacketListHeaderPtr = (QosPacketListPtr)malloc(sizeof(QosPacketList));
					DLList *         newPriorityListPtr     = (DLList *)malloc(sizeof(DLList));

					DLL_InsertBefore(packetLists, (long)newPacketListHeaderPtr);

					// set packetListHeader parameters
					newPacketListHeaderPtr->priority = packet->priority;
					newPacketListHeaderPtr->list = newPriorityListPtr;

					// initialize and connect
					DLL_Init(newPriorityListPtr);
					priorityListPtr = newPriorityListPtr;

					break;
				}

				DLL_Next(packetLists);
				if (DLL_IsActive(packetLists) == false) {
					QosPacketListPtr newPacketListHeaderPtr = (QosPacketListPtr)malloc(sizeof(QosPacketList));
					DLList *         newPriorityListPtr     = (DLList *)malloc(sizeof(DLList));

					DLL_InsertLast(packetLists, (long)newPacketListHeaderPtr);

					// set packetListHeader parameters
					newPacketListHeaderPtr->priority = packet->priority;
					newPacketListHeaderPtr->list = priorityListPtr;

					// initialize and connect
					DLL_Init(newPriorityListPtr);
					priorityListPtr = newPriorityListPtr;
				}
			}
		}

		// add packet to this priority queue
		if (priorityListPtr != NULL) {
			if (priorityListPtr->currentLength >= MAX_PACKET_COUNT) {
				for (DLL_First(priorityListPtr); DLL_IsActive(priorityListPtr); DLL_Next(priorityListPtr)) {
					DLL_DeleteAfter(priorityListPtr);
				}
			}

			// PacketPtr newPacketPtr = (PacketPtr)malloc(sizeof(Packet));

			DLL_InsertLast(priorityListPtr, (long)packet);

			// newPacketPtr->id       = packet->id;
			// newPacketPtr->priority = packet->priority;
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
	int currentPacketCount = 0;

	for (DLL_First(packetLists); DLL_IsActive(packetLists); DLL_Next(packetLists)) {
		QosPacketListPtr packetListHeader = (QosPacketListPtr)packetLists->activeElement->data;
		DLList *         priorityList     = packetListHeader->list;

		for (DLL_First(priorityList); DLL_IsActive(priorityList); ) {
			PacketPtr packetElementPtr = (PacketPtr)priorityList->activeElement->data;

			DLL_InsertLast(outputPacketList, (long)packetElementPtr);
			DLL_Next(priorityList);
			if (DLL_IsActive(priorityList)) {
				DLL_DeleteBefore(priorityList);
			} else {
				// delete only remaining element
				DLL_DeleteLast(priorityList);
			}
			currentPacketCount++;

			if (currentPacketCount >= maxPacketCount) {
				return;
			}
		}
	}
}

