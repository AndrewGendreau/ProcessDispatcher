/*NAME: Andrew Gendreau
DATE: Nov 1st 2015
VERSION: 1.0
FILENAME: jobQueue.h
PURPOSE: This program will be the header for the job queue module
*/

/**
*Name: deQueue
*Purpose: Will remove the current head of the queue from the queue.
*	Will return a pointer to this process so we can check completion.
*	if it has not completed, it will be added to the tail of the queue.
*	It it has, it will be freed.
*Parameters: 
*	void
*Returns:
Pointer to the dequeued process
*Author: Andrew Gendreau
**/
struct job *deQueue(void);

/**
*Name: traverseToTail
*Purpose: Will seek out the last node in the queue.
*	it will do this by going node by node until the 
*	nextProcess field is null, the current node is the tail
*Parameters: 
*	void
*Returns:
Pointer to the tail of the queue
*Author: Andrew Gendreau
**/

struct job *traverseToTail(void);

/**
*Name: createNode
*Purpose: Will create a node to be inserted into the queue
*Parameters: 
*	pointer to the process struct to be inserted.
*Returns:
Pointer to the newly created node
*Author: Andrew Gendreau
**/

struct job *createNode(struct job *);

/**
*Name: isEmpty
*Purpose: Will check if the queue is empty
*Parameters: 
*	void
*Returns:
0 if full, 1 if not
*Author: Andrew Gendreau
**/

int isEmpty(void);

/**
*Name: enqueue
*Purpose: Will enqueue the given process struct in this queue.
	If the head is empty the node will become the new head.
	Otherwise, it will be added at the end of the queue.
*Parameters: 
*	pointer to the process struct to insert
*Returns:
Pointer to the head of the queue
*Author: Andrew Gendreau
**/
struct job *enqueue(struct job *);

/**
*Name: jobDispatchWork
*Purpose: Will be the work of the job dispatching thread.
	This thread will remove jobs from the job queue, one at a time
	until it hits the end. Once it has a job, it will compare the system time 
	to the arrival time of the job and hold it until it is time
*Parameters: 
*	void *jobQueueHead --head of the job queue
*Returns:
	void
*Author: Andrew Gendreau
**/
void *jobDispatchWork(void *jobQueueHead);

/**
*Name: jobDispatchSpawn
*Purpose: Will spawn the job dispatch thread
*Parameters: 
*	void
*Returns:
	void
*Author: Andrew Gendreau
**/
void jobDispatchSpawn(void);

struct job *blockedEnqueue(struct job *, struct job *);

struct job *blockedDequeue(struct job *);


