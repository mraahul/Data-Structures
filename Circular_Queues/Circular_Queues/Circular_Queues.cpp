/*******************************************************************************
* File Name          : circular_Queues.cpp
* Description        : Circular Queue assignment skeleton starting point
* Author			 :
* Date				 : June 27, 2017
* Updated            : March 29, 2019
******************************************************************************
*/
#include "pch.h"
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <strsafe.h>

DWORD WINAPI myReadThreadFunction(LPVOID lpParam);                                   // function used by read from queue thread
unsigned int putToCircularQueue(char *ptrInputBuffer, unsigned int bufferLength);    // circular queue function to add data to queue
unsigned int getFromCircularQueue(char *ptrOutputBuffer, unsigned int bufferLength); // circular queue function to remove data from queue

#define BUFFER_SIZE         20                 // local buffer used for sending or receiving data to/from queue in main and worker thread

#define INPUT_TIMEOUT_MS    5000               // dequeue every 5 seconds

#define CIRCULAR_QUEUE_SIZE 20                 // size of the circular queue

CRITICAL_SECTION Queue;							//define object of critical section

// data structure used to keep track of circular queue (DO NOT CHANGE THIS STRUCTURE)
struct myQueueStruct {
	char *ptrBuffer = NULL;                    // pointer to start of the circular queue buffer
	volatile unsigned int queueCount = 0;      // number of characters currently in cirucular queue
	volatile char *ptrCircularHead = NULL;     // location where data is added to queue
	volatile char *ptrCircularTail = NULL;     // loation where data is removed from queue
};

struct myQueueStruct myQueue;                  // create an instance of the circular queue data structure

// DO NOT CHANGE THE CODE IN MAIN
int main()
{
	HANDLE  hThread;            // handle to thread
	char    inputBuffer[BUFFER_SIZE];

	myQueue.ptrBuffer = (char *)calloc(CIRCULAR_QUEUE_SIZE, sizeof(char));
	if (myQueue.ptrBuffer == NULL)
	{
		printf("Error unable to allocate memory for buffer\n");
		exit(-1);
	}

	myQueue.ptrCircularHead = myQueue.ptrCircularTail = myQueue.ptrBuffer;

	InitializeCriticalSection(&Queue); //initialize critical section

	// create a thread that will consume the data we type in to demonstrate dequeing the data
	hThread = CreateThread(
		NULL,                   // default security attributes
		0,                      // use default stack size  
		myReadThreadFunction,   // thread function name
		NULL,                   // argument to thread function (not being used)
		0,                      // use default creation flags 
		NULL);                  // returns the thread identifier (not being used)
	
	EnterCriticalSection(&Queue);	

	// get a string from the console and queue it to circular queue
	while (1)
	{
		scanf_s("%s", inputBuffer, BUFFER_SIZE);    // get data from console

		// put the data into the circular queue but check if an error (marked by queue function returning 0) occurred
		if (putToCircularQueue(inputBuffer, (unsigned int)strlen(inputBuffer)) == 0)
			printf("Error queuing data\n");
	}
	return 0;
}

// FUNCTION      : putToCircularQueue
// DESCRIPTION   : Put the supplied data into a circular queue
// PARAMETERS    : ptrInputBuffer - a pointer to the buffer to be put into the queue
//				   bufferLength   - how many characters are in the buffer being put into the queue
// RETURNS       : Number of characters successfully queued (0 on failure)

unsigned int putToCircularQueue(char *ptrInputBuffer, unsigned int bufferLength)
{
	
	unsigned int numberOfCharsQueued = 0;  // number of characters placed in circular queue
	while (numberOfCharsQueued < bufferLength) {
		if ((myQueue.ptrCircularHead) == (myQueue.ptrBuffer + CIRCULAR_QUEUE_SIZE)) {
			myQueue.ptrCircularHead = myQueue.ptrBuffer;
		}

		if (myQueue.queueCount == CIRCULAR_QUEUE_SIZE) {
			printf("Queue Full. Lost data is \"%s.\"\n", ptrInputBuffer);
			break;
		}

		*myQueue.ptrCircularHead = *ptrInputBuffer;
		numberOfCharsQueued++;
		ptrInputBuffer++;

		myQueue.ptrCircularHead++;
		myQueue.queueCount++; 
		LeaveCriticalSection(&Queue);
	}
	return numberOfCharsQueued;
}

// FUNCTION      : getFromCircularQueue
// DESCRIPTION   : Read as much data as there is room for from the circular queue
// PARAMETERS    : ptrOutputBuffer - a pointer to the buffer to place the data read from queue
// 				   bufferLength    - maxiumum number of characters that can be read (IE the size of the output buffer)
// RETURNS       : Number of characters successfully queued (0 if none are read)
unsigned int getFromCircularQueue(char *ptrOutputBuffer, unsigned int bufferLength)
{
	static unsigned int readCount = 0;
	while (myQueue.queueCount > 0) {
		if ((myQueue.ptrCircularTail) == (myQueue.ptrBuffer + CIRCULAR_QUEUE_SIZE)) {
			myQueue.ptrCircularTail = myQueue.ptrBuffer;
		}
		*ptrOutputBuffer = *(myQueue.ptrCircularTail);
		readCount++;
		ptrOutputBuffer++;

		myQueue.ptrCircularTail++;
		myQueue.queueCount--;
		LeaveCriticalSection(&Queue);
	}
	*ptrOutputBuffer = '\0';

	return readCount;
}

// FUNCTION      : myReadThreadFunction
// DESCRIPTION   : A seperate thread from the main program that will independently check for data in queue and print it out
//  					 DO NOT CHANGE THIS CODE!
// PARAMETERS    : lpParam - arguement passed into thread (not used in this example)
// RETURNS       : Will never return so no effective return value
DWORD WINAPI myReadThreadFunction(LPVOID lpParam)
{
	char readBuffer[BUFFER_SIZE];     // local buffer to put the data into when reading from queue and print
	unsigned int  readCount = 0;      // the number of characters read from queue
	EnterCriticalSection(&Queue);
	while (1)
	{
		Sleep(INPUT_TIMEOUT_MS);      // wait for some data to be received

		// check if data is available and if so print it out
		readCount = getFromCircularQueue(readBuffer, BUFFER_SIZE);
		if (readCount != 0)           // check for reads that did not get any data
			printf("\nUNQUEUE: %s\n", readBuffer);
	}
	
	return 0;    // will never reach here
}