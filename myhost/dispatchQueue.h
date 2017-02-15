/*NAME: Andrew Gendreau
DATE: Nov 3 2015
VERSION: 1.0
FILENAME: dispatchQueue.c
PURPOSE: This will be a module to represent a queue of processes in execution.
	This will make use of a dispatchQueue struct to hold the head and another that 
	represents nodes This is a generally defined queue,
	 it can be used with any number of dispatch queues.
*/

#include <stdio.h>

/*Name: dispatchQueue
*Purpose: This is a struct to hold the head of the queue.
Author: Andrew Gendreau
*/
struct dispatchQueue 
{
	struct dispatchNode *head;			//pointer to the head of the queue
};

/*Name: dispatchNode
*Purpose: This is a struct to represent a node in the queue
Author: Andrew Gendreau
*/
struct dispatchNode 
{
	struct process *processPointer;			//pointer to the process in the node
	struct dispatchNode *nextNode;			//pointer to the next dispatch node
};

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
struct dispatchNode *createDispatchNode(struct process *);

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
void dispatchEnqueue(struct process *, struct dispatchQueue *);

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
struct dispatchNode *dispatchTail(struct dispatchQueue *);

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
struct dispatchQueue *createQueue(void);

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
struct process *dispatchDequeue(struct dispatchQueue *);

/*Name: empty
*Purpose: This function will take a queue and check if it is empty.
	It will simply check if the head is NULL.
*Parameters:
	 struct dispatchQueue *queue --queue we want to check
Returns:
	int isEmpty --int that will be 0 if it is empty
Author: Andrew Gendreau
*/
int empty(struct dispatchQueue *);
