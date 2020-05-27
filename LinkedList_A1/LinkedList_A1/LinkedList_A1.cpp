/*******************************************************************************************
	* File Name          : LinkedList_A1.c
	* Description        : Prog8130 - Assignment 1: Linked Lists
	*					   This program uses linked lists to hold and print a list of words
	*					   organized by index number assosciated with each word. Each word can
	*					   consist of maximum 30 characters.
	* Author:              Rahul Mishra
	* Date:                28th Jan 2019
	********************************************************************************************/

#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_OF_DATA_ITEMS   10					// the number of data items to be used in list
#define NAME_DATA_SIZE	30

// structure/typedef used for holding data
typedef struct {
	char name[NAME_DATA_SIZE];
	int index;
} inputData;

// definition of linked list data NUMBER_OF_DATA_ITEMS
typedef struct LinkedListNode {
	struct LinkedListNode		*ptrNextNode;		// pointer to next node in list (NULL if the end)
	char						NameData[NAME_DATA_SIZE];		// the name to be stored (no need to store index)
} LinkedListNodeDef;

inputData iData[10] = {
		{ "fish", 7 },
		{ "allan", 1 },
		{ "kiwi", 5 },
		{ "butter", 5 },
		{ "carrot", 8 },
		{ "pickels", 3 },
		{ "orange", 5 },
		{ "car", 6 },
		{ "chocolate", 8 },
		{ "milk", 5 }
};
/* Correct order after inserting into list is:
1. allan
2. fish
3. pickels
4. kiwi
5. milk
6. orange
7. car
8. butter
9. chocolate
10. carrot
*/

// prototypes
LinkedListNodeDef *AddToLinkedList(LinkedListNodeDef *ptrHead, char *NameData, int DataIndex);
void PrintLinkedList(LinkedListNodeDef *ptrHead);

int main(void) {
	int i;

	// define linked list head
	struct LinkedListNode		*ptrHead = NULL;

	// Print out the input data
	printf("Data before Linked List is:\n\n");
	for (i = 0; i < NUMBER_OF_DATA_ITEMS; i++)
		printf("%s , %d\n", iData[i].name, iData[i].index);

	// insert the data into Linked List
	for (i = 0; i < NUMBER_OF_DATA_ITEMS; i++)
		ptrHead = AddToLinkedList(ptrHead, iData[i].name, iData[i].index);

	printf("\nPrint Data in Linked List.\n");
	// now print out the list in order it is stored starting at head
	PrintLinkedList(ptrHead);
}

// FUNCTION      : AddToLinkedList()
// DESCRIPTION   : This function takes an input of Data of 30 Char word and an index and 
//				   places them in a linked list organized by priority of index number. 
// PARAMETERS    : LinkedListNodeDef *ptrHead	- variable to store address of ptrHead
//				   char *NameData				- Array of Characters with data 
//		           int DataIndex				- Index of data
// RETURNS       : possibly updated pointer to the head of the list
LinkedListNodeDef *AddToLinkedList(LinkedListNodeDef *ptrHead, char *NameData, int DataIndex) {
	
	// put code in here to add the item(s) to the linked list
	struct LinkedListNode* newNode = (LinkedListNode*)malloc(sizeof(LinkedListNode));
	struct LinkedListNode* pIterative = NULL;

	newNode->ptrNextNode = NULL;

	strcpy_s(newNode->NameData, NAME_DATA_SIZE, NameData);

	if (ptrHead == NULL)  //if list is empty, then newNode becomes the head
	{
		// empty list so put at front
		ptrHead = newNode;
	}
	else {
		if (DataIndex <= 1)
		{
			// index is 0 or 1 then it is at the start
			newNode->ptrNextNode = ptrHead;
			ptrHead = newNode;
		}
		else
		{
			int currentIndex = 1;	
			struct LinkedListNode* pLast = ptrHead;
			pIterative = ptrHead;

			// find the spot to put it in or the end of list
			while (pIterative->ptrNextNode != NULL)
			{
				if (currentIndex >= DataIndex)
				{
					break;
				}
				pLast = pIterative;
				pIterative = pIterative->ptrNextNode;
				currentIndex++;
			}
			if (currentIndex >= DataIndex)
			{
				// data will be in middle of linked list
				newNode->ptrNextNode = pIterative;
				pLast->ptrNextNode = newNode;
			}
			else
			{
				// data will be at end of linked list
				pIterative->ptrNextNode = newNode;
			}
		}
	}
	return ptrHead;
}

// FUNCTION      : PrintLinkedList()
// DESCRIPTION   : This function takes an input of the head of the linked list, then loops through
//				   and prints each word in the linked list until the ptrNextNode is NULL.
// PARAMETERS    : LinkedListNodeDef *ptrHead 
// RETURNS       : void
void PrintLinkedList(LinkedListNode *ptrHead) {

	// put code in here to print the linked list out
	LinkedListNode *pIterative = ptrHead;
	while (pIterative != NULL)
	{
		printf("%s\n", pIterative->NameData);
		pIterative = pIterative->ptrNextNode;
	}
}
