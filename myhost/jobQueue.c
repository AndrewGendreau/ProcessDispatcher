/*NAME: Andrew Gendreau
DATE: Oct 30th 2015
VERSION: 1.0
FILENAME: jobQueue.c
PURPOSE: This program will be the job queue that will hold the jobs.
	It will also spawn a thread to read in jobs and dispatch them
	as the system time changes.
*Author: Andrew Gendreau
	
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#ifndef _JOB_H_
#define _JOB_H_
#include "job.h"
#endif

#ifndef _FILEREADER_H_
#define _FILEREADER_H_
#include "fileReader.h"
#endif

#ifndef _JOBQUEUE_H_
#define _JOBQUEUE_H_
#include "jobQueue.h"
#endif

#ifndef _DISPATCH_H_
#define _DISPATCH_H_
#include "dispatchQueue.h"
#endif

#ifndef _RESOURCE_H_
#define _RESOURCE_H_
#include "resources.h"
#endif

#ifndef _DISPATCHER_H_
#define _DISPATCHER_H_
#include "dispatcher.h"
#endif

struct job *head;							//pointer to the head of the queue
struct dispatchQueue *q0Head;						//pointer to the head of the dispatch queue 0 (real time priority)
struct dispatchQueue *q1Head;						//pointer to the head of the dispatch queue 1 (first level normal)
struct dispatchQueue *q2Head;						//pointer to the head of the dispatch queue 2 (second level normal)
struct dispatchQueue *q3Head;						//pointer to the head of the dispatch queue 3 (third level normal)
extern struct resources *availableResources;				//pointer to a resource struct to define the available resources
pthread_t jobThread;							//thread ID of the thread that will populate the queues
extern int systemTime;							//int to represent total elapsed system time
extern pthread_t systemThread;						//thread of the system execution
extern int processRunning;						//global variable to define if a thread is running right now
extern pthread_mutex_t executeMuxex;					//mutex to control process execution

/**
*Name: enqueue
*Purpose: Will enqueue the given job struct in this queue.
	If the head is empty the node will become the new head.
	Otherwise, it will be added at the end of the queue.
	NOTE: enqueue has the null check to protect from segmentation fault
	if a null pointer is fed to it, the function will attempt
	to insert it and that will cause the fault, the inital if
	will prevent that.
*Parameters: 
*	pointer to the job struct to insert
*Returns:
Pointer to the head of the queue
*Author: Andrew Gendreau
**/
struct job *enqueue(struct job *newJob)
{
	struct job *queueTail;						//tail of the queue, used if we're not inserting at the head
	int emptyCheck = 1;						//int to check if the queue is empty, will be 0 if it is
	
	if(newJob == NULL)
	{
		fprintf(stderr, "Tried to insert a null job\n");
		exit(1);
	}

	emptyCheck = isEmpty();		

	if(emptyCheck == 0)						//if statement to check if we are inserting into the head the queue
	{
		head = newJob;
		head->nextJob = NULL;
	
		return head;
	}
	else								//otherwise we're inserting to the end of the queue
	{
		queueTail = traverseToTail();
		
		queueTail->nextJob = newJob;

		newJob->nextJob = NULL;

		return head;
	}
}

/**
*Name: traverseToTail
*Purpose: Will seek out the last node in the queue.
*	it will do this by going node by node until the 
*	nextJob field is null, the current node is the tail
*Parameters: 
*	void
*Returns:
Pointer to the tail of the queue
*Author: Andrew Gendreau
**/
struct job *traverseToTail(void)
{
	struct job *currentNode;					//node we're currently looking at
	struct job *nextNode;						//the next node in the queue

	currentNode = head;						

	while(currentNode->nextJob != NULL)			
	{
		nextNode = currentNode->nextJob;
		currentNode = nextNode;
	}
	return currentNode;
}

/**
*Name: deQueue
*Purpose: Will remove the current head of the queue from the queue.
*	Will return a pointer to this job so we can form a process from it.
*Parameters: 
*	void
*Returns:
Pointer to the dequeued job
*Author: Andrew Gendreau
**/
struct job *deQueue(void)
{
	struct job *newJob;				//job struct to return upon function completion

	newJob = head;

	head = head->nextJob;
	
	return newJob;
}

/**
*Name: createNode
*Purpose: Will create a node to be inserted into the queue.
	Will check for malloc errors as well.
*Parameters: 
*	pointer to the job struct to be inserted.
*Returns:
Pointer to the newly created node
*Author: Andrew Gendreau
**/
struct job *createNode(struct job *newJob)
{
	struct job *newNode;					//new node to be inserted into the queue

	newNode = malloc(sizeof(struct job));

	if(newNode == NULL)
	{
		fprintf(stderr, "Malloc failure!\n");
		exit(1);
	}

	return newNode;
}

/**
*Name: isEmpty
*Purpose: Will check if the queue is empty
*Parameters: 
*	void
*Returns:
0 if empty, 1 if not
*Author: Andrew Gendreau
**/
int isEmpty(void)
{
	if(head == NULL)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

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
void *jobDispatchWork(void *jobQueueHead)
{
	struct job *currentJob;						//current job to dispatch
	int jobArrivalTime;						//time the job arrived
	int resourcesAvailable;						//int to track if we have the needed resources


	do
	{
		currentJob = deQueue();

		jobArrivalTime = currentJob->arrivalTime;

		while(systemTime < jobArrivalTime);			//just wait until the time is right for the job

		resourcesAvailable = jobPossible(currentJob);

		while(resourcesAvailable != 0)
		{
			resourcesAvailable = jobPossible(currentJob);
		}
	
		dispatchJob(currentJob);

	} while(head != NULL);
	free(currentJob);
	printf("ALL JOBS ENQUEUED!\n");
	pthread_exit(NULL);						//terminate the thread
}

/**
*Name: jobDispatchSpawn
*Purpose: Will spawn the job dispatch thread
*Parameters: 
*	void
*Returns:
	void
*Author: Andrew Gendreau
**/
void jobDispatchSpawn(void)
{	
	pthread_create(&jobThread, NULL, &jobDispatchWork, head);
}
