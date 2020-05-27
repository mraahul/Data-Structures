/*******************************************************************************
* File Name          : Hashing.cpp
* Description        : Implemenation of a hash skelleton
*
* Author			 : Rahul Mishra
* Date				 : Nov 21, 2018
*******************************************************************************/

#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int putIntoHashTable(char *ptrInputData, unsigned int bufferLength);    // function to add to hash table
unsigned int getFromHashTable(char *ptrOutputData, unsigned int bufferLength);   // function to retrieve data from hash table

#define INPUT_BUFFER_SIZE         200             // local buffer used for adding data to the hash table (there is no reason in this assignment to change this value)

#define HASH_SIZE    10                           // size of hash table to be used (for testing I suggest making this number much lower)

												  // data structure used to keep track of hashed data
struct myHashStruct {
	char *ptrBuffer = NULL;                       // pointer to data stored in hash (you will need to malloc space for string to be stored)
	struct myHashStruct *ptrNextHashData = NULL;  // pointer to next item in this hash bucket (or NULL if no more)
};

struct myHashStruct *myHash[HASH_SIZE];           // create an empty hash table structure (note this is basically an arrary of linked list heads)

int main()
{
	char    inputBuffer[INPUT_BUFFER_SIZE];

	// initialize the hash table to empty one
	for (int i = 0; i < HASH_SIZE; i++)
	{
		if ((myHash[i] = (struct myHashStruct *)calloc(1, sizeof(struct myHashStruct))) == NULL)
		{
			printf("calloc failed!\n");
			return(-1);
		}
	}

	// add to hash table loop
	while (1)
	{
		printf("\nEnter data to be added to hash table or exit when done\n");

		// get strings from the console and place in hash until nothing entered
		scanf_s("%s", inputBuffer, INPUT_BUFFER_SIZE);
		printf("Entered String: %s \t", inputBuffer);

		// stop adding data into hash table when "exit" is entered
		if (strcmp(inputBuffer, "exit") == 0)
			break;

		if (putIntoHashTable(inputBuffer, strlen(inputBuffer)) == HASH_SIZE)
			printf("Error putting into hash table\n");
	}

	// check if data is in hash table
	while (1)
	{
		unsigned int hashIndexNumber = 0;

		printf("\nEnter data to find, done when complete\n");

		// get strings from the console and check if in hash table
		scanf_s("%s", inputBuffer, INPUT_BUFFER_SIZE);

		// stop adding data into hash table when "done" is entered
		if (strcmp(inputBuffer, "done") == 0)
			break;

		if ((hashIndexNumber = getFromHashTable(inputBuffer, strlen(inputBuffer))) == HASH_SIZE)
			printf("%s not found in hash table\n", inputBuffer);
		else
			printf("%s found in hash table at %u\n", inputBuffer, hashIndexNumber);
	}
	return 0;
}

// FUNCTION      : myHashFunction
// DESCRIPTION   : Calculate a hash value to use in storing the data into the hash table
// PARAMETERS    : ptrInputBuffer - a pointer to the buffer to be put into the hash table
// RETURNS       : Hash value calculated ( HASH_SIZE on failure)

unsigned int myHashFunction(char *ptrInputBuffer)
{
	unsigned long hash = 5381;
	int c;
	while (c = *ptrInputBuffer++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	return hash % HASH_SIZE;
}

// FUNCTION      : putIntoHashTable
// DESCRIPTION   : Put the supplied data into a hash table
// PARAMETERS    : ptrInputBuffer - a pointer to the buffer to be put into the hash table
//				   bufferLength   - how many characters are in the buffer being put into the hash table
// RETURNS       : Hash value used ( HASH_SIZE on failure)

unsigned int putIntoHashTable(char *ptrInputData, unsigned int bufferLength)
{
	unsigned int hashValue = myHashFunction(ptrInputData);
	printf("Hash Value:%ld \n", hashValue);
	if (myHash[hashValue]->ptrBuffer == NULL) {
		myHash[hashValue] = (myHashStruct*)malloc(sizeof(myHashStruct));
		myHash[hashValue]->ptrBuffer = (char *)malloc(sizeof(bufferLength + 1));
		strcpy_s(myHash[hashValue]->ptrBuffer, bufferLength + 1, ptrInputData);
		myHash[hashValue]->ptrNextHashData = NULL;
	}
	else {
		struct myHashStruct* link = (myHashStruct*)malloc(sizeof(myHashStruct));
		link->ptrBuffer = (char *)malloc(sizeof(bufferLength + 1));
		link->ptrNextHashData = NULL;
		strcpy_s(link->ptrBuffer, (bufferLength + 1), ptrInputData);
		link->ptrNextHashData = myHash[hashValue];
		myHash[hashValue] = link;
	}
	return hashValue;
}



// FUNCTION      : getFromHashTable
// DESCRIPTION   : Read as much data as there is room for from the hash table
// PARAMETERS    : ptrOutputBuffer - a pointer to the buffer to place the data read from hash table
//				   bufferLength    - maxiumum number of characters that can be read
// RETURNS       : Hash value used ( return HASH_SIZE value on failure)

unsigned int getFromHashTable(char *ptrOutputData, unsigned int bufferLength)
{
	unsigned int hashValue = myHashFunction(ptrOutputData);
	if (myHash[hashValue]->ptrBuffer != NULL) {
		struct myHashStruct* temp = (myHashStruct*)malloc(sizeof(myHashStruct));
		temp = myHash[hashValue];
		while (temp->ptrBuffer != NULL) {
			if (strcmp(temp->ptrBuffer, ptrOutputData) == 0) {
				printf("Data Present.\n");
				return hashValue;
			}
			temp = temp->ptrNextHashData;
		}
	}
	else {
		return HASH_SIZE;
	}
}
// end code *    *     *   
