/*******************************************************************************
* File Name			: circularQueue.cpp
* Description		: Circular Queue Final skeleton starting point
*
* Author			: Rahul Mishra
* Date				: April 8, 2019
* Updated			: April 12, 2019
******************************************************************************
*/

#include "stdafx.h"
#include <string.h>
#include <windows.h>
#include <strsafe.h>

DWORD WINAPI myReadThreadFunction(LPVOID lpParam);                             // function used by read from queue thread
unsigned int putToTheQueue(char *ptrInputBuffer, unsigned int bufferLength);    // circular queue function to add data to queue
unsigned int getFromTheQueue(char *ptrOutputBuffer, unsigned int bufferLength); // circular queue function to remove data from queue
int checkIfVowel(char characterToCheck);                                        // function that checks if char is a vowel

#define BUFFER_SIZE         200                // local buffer used for sending or receiving data to/from queue in main and worker thread

#define INPUT_TIMEOUT_MS    2000               // dequeue every 2 seconds

#define CIRCULAR_QUEUE_SIZE 10                 // size of the circular queue

																				// data used to keep track of circular queue (DO NOT CHANGE THESE DATA ITEMS)

char qBuffer[CIRCULAR_QUEUE_SIZE] = { 0 }; // circular queue buffer (note this is different than what you did in class)
volatile int queueCount = 0;               // number of characters currently in cirucular queue
volatile int indexHead = -1;             // index into qBuffer of location where data is added to queue
volatile int indexTail = -1;             // index into qBuffer of location where data is removed from queue

										   // note an index is different than a pointer like used in assignment 3.  For an index you can access the data like so:
										   //  qBuffer[indexHead]  instead of using a pointer.  This type of circular queue implementation is pretty commonly described
										   // on the internet but you will have to adapt it to this skelleton.


										   // DO NOT CHANGE THE CODE IN MAIN
int main()
{
	HANDLE  hThread;            // handle to thread
	char    inputBuffer[BUFFER_SIZE];

								// create a thread that will consume the data we type in to demonstrate dequeing the data
	hThread = CreateThread(
		NULL,                   // default security attributes
		0,                      // use default stack size  
		myReadThreadFunction,   // thread function name
		NULL,                   // argument to thread function (not being used)
		0,                      // use default creation flags 
		NULL);                  // returns the thread identifier (not being used)

								// get a string from the console and queue it to circular queue
	while (1)
	{
		scanf_s("%s", inputBuffer, BUFFER_SIZE);    // get data from console

													// put the data into the circular queue but check if an error (marked by queue function returning 0) occurred
		if (putToTheQueue(inputBuffer, (unsigned int)strlen(inputBuffer)) == 0)
			printf("Error queuing data\n");
	}
	return 0;
}

// FUNCTION      : putToTheQueue
// DESCRIPTION   : Put the supplied data into a circular queue
// PARAMETERS    : ptrInputBuffer - a pointer to the buffer to be put into the queue
//				   bufferLength   - how many characters are in the buffer being put into the queue
// RETURNS       : Number of characters successfully queued

unsigned int putToTheQueue(char *ptrInputBuffer, unsigned int bufferLength)
{
	unsigned int numberOfCharsQueued = 0;  // number of characters placed in circular queue
	while (numberOfCharsQueued < bufferLength) {
		if (indexHead == -1 && indexTail == -1) {
			printf("Queue Empty.\n");
			indexTail = indexHead = 0;
		}
		else if (queueCount == CIRCULAR_QUEUE_SIZE) {
			printf("Queue Full. Lost data is \"%s.\"\n", ptrInputBuffer);
			break;
		}
		else {
			indexTail = (indexTail + 1) % CIRCULAR_QUEUE_SIZE;
		}
		qBuffer[indexTail] = *ptrInputBuffer;
		numberOfCharsQueued++;
		ptrInputBuffer++;
		queueCount++;
	}
	return numberOfCharsQueued;
}



// FUNCTION      : getFromTheQueue
// DESCRIPTION   : Read as much data as there is room for from the circular queue
// PARAMETERS    : ptrOutputBuffer - a pointer to the buffer to place the data read from queue
//				   bufferLength    - maxiumum number of characters that can be read (IE the size of the output buffer)
// RETURNS       : Number of characters successfully queued (0 if none are read)

unsigned int getFromTheQueue(char *ptrOutputBuffer, unsigned int bufferLength)
{
	static unsigned int readCount = 0;
	if (queueCount >= 0) {
		*ptrOutputBuffer = qBuffer[indexHead];
		if (indexHead == -1) {
			printf("Queue Empty.\n");
			return (-1);
		}
		else {
			if (indexHead == indexTail) {
				indexHead = indexTail = -1;
			}
			else {
				indexHead = (indexHead + 1) % CIRCULAR_QUEUE_SIZE;
			}
			readCount++;
			ptrOutputBuffer++;
			queueCount--;
		}
	}
	return readCount;
}

// FUNCTION      : checkIfVowel
// DESCRIPTION   : This function will check a given character to see if it is a vowel
// PARAMETERS    : characterToCheck - a character to check for vowel
// RETURNS       : 0 if not a vowel / non zero if it is a vowel

int checkIfVowel(char characterToCheck)
{
	int itIsAVowel = 0;

	if (characterToCheck == 'a' || characterToCheck == 'e' || characterToCheck == 'i' || characterToCheck == '0' || characterToCheck == 'u' || characterToCheck == 'A' || characterToCheck == 'E' || characterToCheck == 'I' || characterToCheck == 'O' || characterToCheck == 'U')
		itIsAVowel = 1;
	else {
		itIsAVowel = 0;
	}
	return itIsAVowel;
}

// FUNCTION      : myReadThreadFunction
// DESCRIPTION   : A seperate thread from the main program that will independently check for data in queue and print it out
//   DO NOT CHANGE THIS CODE!
// PARAMETERS    : lpParam - arguement passed into thread (not used in this example)
// RETURNS       : Will never return so no effective return value

// DO NOT CHANGE THE CODE IN myReadThreadFunction

DWORD WINAPI myReadThreadFunction(LPVOID lpParam)
{
	char readBuffer[BUFFER_SIZE] = {0};     // local buffer to put the data into when reading from queue and print
	unsigned int  readCount = 0;      // the number of characters read from queue
	char vowelString[2][10] = { "VOWEL", "NOT VOWEL" };

	while (1)
	{
		Sleep(INPUT_TIMEOUT_MS);      // wait for some data to be received

									  // check if data is available and if so print it out
		readCount = getFromTheQueue(readBuffer, 1);   // note this will read each character out one at a time

		if (readCount != 0)           // check for reads that did not get any data
			printf("UNQUEUE: %c\t", *readBuffer);

		if (checkIfVowel(*readBuffer) != 0)
			printf("%s\n", vowelString[0]);
		else
			printf("%s\n", vowelString[1]);
	}
	return 0;    // will never reach here
}

// end code *     *   