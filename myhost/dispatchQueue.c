/*NAME: Andrew Gendreau
DATE: Nov 3 2015
VERSION: 1.0
FILENAME: dispatchQueue.c
PURPOSE: This will be a module to represent a queue of processes in execution.
	This will make use of a dispatchQueue struct defined in the header.
	This is a generally defined queue, it can be used with any number of dispatch queues.
*/

#include <stdio.h>
#include <stdlib.h>

#ifndef _EXECUTINGPROCESS_H_
#define _EXECUTINGPROCESS_H_
#include "executingProcess.h"
#endif

#ifndef _DISPATCH_H_
#define _DISPATCH_H_
#include "dispatchQueue.h"
#endif

/*Name: dispatchEnqueue
*Purpose: This function will enqueue a given process into a given queue.
	Will just return nothing if a null process is attempted to be inserted.
	It will check if the queue is empty, if so the new node will be the head.
	If not, it will make a call to another function to locate the tail of the queue
	and insert there.
*Parameters:
	struct process *inputProcess --process to enqueue
	struct dispatchQueue *queue --queue to insert the process into
Returns:
	void
Author: Andrew Gendreau
*/
void dispatchEnqueue(struct process *inputProcess, struct dispatchQueue *queue)
{
	struct dispatchNode *newNode;						//newly created dispatch node
	struct dispatchNode *tail;						//tail of the queue, used if we're not inserting into an 
										//empty queue
	
	if(inputProcess == NULL)						//check for attempts to insert a null process.
	{
		printf("Trying to insert a null process?\n");
		return;
	}

	newNode = createDispatchNode(inputProcess);

	if(queue->head == NULL)
	{
		queue->head = newNode;
		newNode->nextNode = NULL;
	}
	else
	{
		tail = dispatchTail(queue);

		tail->nextNode = newNode;
		newNode->nextNode = NULL;
	}
}

/*Name: createQueue
*Purpose: This function will create a new empty queue. This is used to initalize the
	queue variable to be null to account for the possibility that uninitialized pointers
	in C are could be anything. It will allocate the nessicary memory using malloc,
	then it will set the head to null.
*Parameters:
	void
Returns:
	struct dispatchQueue *newQueue --newly created queue
Author: Andrew Gendreau
*/
struct dispatchQueue *createQueue(void)
{
	struct dispatchQueue *newQueue;						//queue variable to be returned

	newQueue = malloc(sizeof(struct dispatchQueue));

	if(newQueue == NULL)
	{
		fprintf(stderr, "malloc error\n");
		exit(1);
	}
	newQueue->head = NULL;

	return newQueue;
}

/*Name: dispatchNode
*Purpose: This function will create a dispatch queue node and return it.
	It will simply take a process pointer and allocate the space for the 
	node and set the node to contain the given process pointer.
*Parameters:
	struct process *inputProcess --process to be inserted into a node
Returns:
	struct dispatchNode *node --process to be inserted into a node
Author: Andrew Gendreau
*/
struct dispatchNode *createDispatchNode(struct process *inputProcess)
{
	struct dispatchNode *node;						//new dispatch node

	node = malloc(sizeof(struct dispatchNode));	

	if(node == NULL)
	{
		fprintf(stderr, "Malloc error!\n");
		exit(1);
	}
	
	node->processPointer = inputProcess;
	
	return node;
}

/*Name: dispatchTail
*Purpose: This function will take a queue and locate its tail. 
	It will move through the nodes until it hits the one with the next
	node set to NULL. It will return this node.
*Parameters:
	struct dispatchQueue *queue --queue we want the tail of
Returns:
	struct dispatchNode *currentNode --tail of the queue
Author: Andrew Gendreau
*/
struct dispatchNode *dispatchTail(struct dispatchQueue *queue)
{
	struct dispatchNode *currentNode;						//current node in the queue
	struct dispatchNode *nextNode;							//next node in the queue

	currentNode = queue->head;

	while(currentNode->nextNode != NULL)
	{
		nextNode = currentNode->nextNode;
		currentNode = nextNode;
	}
	return currentNode;
}

/*Name: dispatchDequeue
*Purpose: This function will take a queue and remove and return the 
	head of it. It will check if the head is null, then it 
	will move the head and return the process in the queue head.
*Parameters:
	 struct dispatchQueue *queue --queue we want to draw from
Returns:
	struct process *processReturn --process that was in the head
Author: Andrew Gendreau
*/
struct process *dispatchDequeue(struct dispatchQueue *queue)
{
	struct dispatchNode *temp;							//temporary variable to hold the node to return
	struct process *processReturn;							//process in the node to return

	if(queue->head == NULL)
	{
		printf("Null head?\n");
	}
	temp = queue->head;

	processReturn = temp->processPointer;

	queue->head = temp->nextNode;

	return processReturn;
}

/*Name: empty
*Purpose: This function will take a queue and check if it is empty.
	It will simply check if the head is NULL.
*Parameters:
	 struct dispatchQueue *queue --queue we want to check
Returns:
	int isEmpty --int that will be 0 if it is empty
Author: Andrew Gendreau
*/
int empty(struct dispatchQueue *queue)
{
	int isEmpty;									//int value to check if the given queue is empty
	
	if(queue->head == NULL)								
	{
		isEmpty = 0;
		return isEmpty;
	}
	else
	{
		isEmpty = 1;
		return isEmpty;
	}
	return isEmpty;
}
