/*NAME: Andrew Gendreau
DATE: Nov 3 2015
VERSION: 1.0
FILENAME: dispatcher.c
PURPOSE: This will be a module to perform the process dispatching. The functions of this
	module will be fed created processes and dispatch them to their respective queues.
	It will have defined constants to represent each queue's time quantum.
*/

#include <stdio.h>
#include <stdlib.h>
#include "dispatcher.h"

#ifndef _RESOURCE_H_
#define _RESOURCE_H_
#include "resources.h"
#endif

#ifndef _DISPATCH_H_
#define _DISPATCH_H_
#include "dispatchQueue.h"
#endif

#include <pthread.h>
#include <unistd.h>

#define PRIORITY1QUANTUM 1						//time quantum for priority level 1
#define PRIORITY2QUANTUM 2						//time quantum for priority level 2
#define PRIORITY3QUANTUM 4						//time quantum for priority level 3

extern struct dispatchQueue *q0Head;					//pointer to the head of the dispatch queue 0 (real time priority)
extern struct dispatchQueue *q1Head;					//pointer to the head of the dispatch queue 1 (first level normal)
extern struct dispatchQueue *q2Head;					//pointer to the head of the dispatch queue 2 (second level normal)
extern struct dispatchQueue *q3Head;					//pointer to the head of the dispatch queue 3 (third level normal)
extern int experimentCompletion;					//int to signify the experiment is complete.
int realTimeArrived;							//int to keep track of when a real time process arrives
extern int processRunning;						//global variable to define if a thread is running right now
extern pthread_mutex_t executeMuxex;					//mutex to protect the completion condition variable
extern int systemTime;							//int to represent total elapsed system time
extern pthread_mutex_t schedulerMutex;					//mutex to protect the completion condition variable
extern pthread_cond_t schedulerCondtion;				//condition variable to signal scheduler invocation
extern pthread_mutex_t completeMutex;					//mutex to protect the completion condition variable
extern pthread_cond_t completeCondition;				//condtion variable to be signaled when all processes complete
extern struct process *runningProcess;					//currently running process struct
extern pthread_mutex_t runningMutex;					//mutex to protect the currently running process struct
extern pthread_mutex_t resourceMutex;					//mutex to protect the resource struct

/*Name: feedbackScheduler
*Purpose: This will scan all the dispatch queues checking one by one which one has a process in it.
*	it will then grab a process from the highest available process queue and depending on priority will
*	either call dispatch real time or dispatch normal. The real time processes will just execute 
*	right away in a FCFS manner. The other 3 levels use a multilevel feedback scheduler. 
*	The quantum for the other 3 levels starts at 1 and increases by a power of 2
*	 with the lowest level having a quantum of 4. After all the processes have completed,
*	this function will signal completion of the experiment.
*Parameters:
	void
Returns:
	void
Author: Andrew Gendreau
*/
void feedbackScheduler(void)
{
	struct process *nextProcess;					//next process to schedule
	struct process *interruptedProcess;				//process that was interrupted if there was one
	
	usleep(1000);							//make it sleep for a little bit to give the job thread time
									//to work.

	if(empty(q0Head) == 1)
	{

		if(processRunning == 1)					//if a process is running
		{
			interruptedProcess = interrupt();
			
			if(interruptedProcess != NULL)
			{
				dispatchEnqueue(interruptedProcess, getQueue(interruptedProcess));
			}

			nextProcess = dispatchDequeue(q0Head);
			dispatchRealTime(nextProcess);
		}
		else
		{
			nextProcess = dispatchDequeue(q0Head);
			dispatchRealTime(nextProcess);
		}
	}
	else if(empty(q1Head) == 1)
	{
		pthread_mutex_lock(&executeMuxex);
		if(processRunning == 0)
		{
			pthread_mutex_unlock(&executeMuxex);
			nextProcess = dispatchDequeue(q1Head);
			dispatchNormal(nextProcess, PRIORITY1QUANTUM);
		}
	}
	else if(empty(q2Head) == 1)
	{
		pthread_mutex_lock(&executeMuxex);
		if(processRunning == 0)
		{
			pthread_mutex_unlock(&executeMuxex);
			nextProcess = dispatchDequeue(q2Head);
			dispatchNormal(nextProcess, PRIORITY2QUANTUM);
		}
	}
	else if(empty(q3Head) == 1)
	{
		pthread_mutex_lock(&executeMuxex);
		if(processRunning == 0)
		{
			pthread_mutex_unlock(&executeMuxex);
			nextProcess = dispatchDequeue(q3Head);
			dispatchNormal(nextProcess, PRIORITY3QUANTUM);
		}
	}
	else
	{
		pthread_mutex_lock(&completeMutex);
		experimentCompletion = 1;
		pthread_cond_signal(&completeCondition);
		pthread_mutex_unlock(&completeMutex);
	}
}

/*Name: getQueue
*Purpose: This will be used to get the queue to re enqueue the given process in.
	This will use a switch to based on the process priority to decide which queue to use.
*Parameters:
	struct process *interruptedProcess --process that was interrupted
Returns:
	struct dispatchQueue * --head of whatever queue we want to use
Author: Andrew Gendreau
*/
struct dispatchQueue *getQueue(struct process *interruptedProcess)
{
	switch(interruptedProcess->priority)
	{
		case 1: 
			return q1Head;
		case 2:
			return q2Head;
		case 3:
			return q3Head;
		default:								//we shouldn't get here
				return NULL;
	}
}

/*Name: dispatchRealTime
*Purpose: This function will dispatch real time processes. This function will take a pointer
	to the struct of the process to begin. It will first set the start time of the
	control block to be current system time. It will then lock the execute mutex to change
	the value of processRunning to 1 to signify a process is running, then unlock it.
	It will then call resume with the control block to run it. It will then join the executing
	thread with the struct's thread to wait for it to execute and make it impossible to
	stop it. 
*Parameters:
	struct process *realTimeProcess --real time process to run
Returns:
	void
Author: Andrew Gendreau
*/
void dispatchRealTime(struct process *realTimeProcess)
{	
	realTimeProcess->startTime = systemTime;

	pthread_mutex_lock(&executeMuxex);
	processRunning = 1;
	pthread_mutex_unlock(&executeMuxex);

	resume(realTimeProcess);

	pthread_join(realTimeProcess->thread, NULL);

	pthread_mutex_lock(&executeMuxex);
	processRunning = 0;
	pthread_mutex_unlock(&executeMuxex);
}

/*Name: dispatchNormal
*Purpose: This function will dispatch non real time processes. 
	This function will take a pointer to the struct of the process to begin.
	It will also take the time quantum to run.
	 It will first set the start time of the control block to be current system time
	if it has not executed. It will then lock the execute and running mutexes to change
	the value of processRunning to 1 to signify a process is running, then unlock them.
	It will then call resume with the control block to run it. It will use a for loop 
	to count up the time quantum, every iteration it will resignal the process that it is okay
	to keep running, it will then signal to have the scheduler invoked to check for the availbility
	of a higher priority process. Once the time quantum expires, the process will be interrupted
	and the scheduler invoked.
*Parameters:
	struct process *servicedProcess --process to run
	int quantum --time quantum to execute
Returns:
	void
Author: Andrew Gendreau
*/
void dispatchNormal(struct process *servicedProcess, int quantum)
{
	int quantumExecuted;						//time the process has executed in this quantum
	struct process *interruptedProcess;				//used to keep track of the process that was interrupted

	if(servicedProcess->hasExecuted == 0)
	{
		servicedProcess->startTime = systemTime;
		servicedProcess->hasExecuted = 1;
	}

	pthread_mutex_lock(&executeMuxex);
	pthread_mutex_lock(&runningMutex);

	processRunning = 1;
	runningProcess = servicedProcess;

	pthread_mutex_unlock(&executeMuxex);
	pthread_mutex_unlock(&runningMutex);

	resume(servicedProcess);

	for(quantumExecuted = 0; quantumExecuted < quantum; quantumExecuted++)
	{
		pthread_mutex_lock(&executeMuxex);
		pthread_cond_signal(&servicedProcess->yourTurnCondition);
		pthread_mutex_unlock(&executeMuxex);
		
		pthread_mutex_lock(&schedulerMutex);
		pthread_cond_signal(&schedulerCondtion);
		pthread_mutex_unlock(&schedulerMutex);
	}
	interruptedProcess = interrupt();
	
	if(interruptedProcess != NULL)					//used to check if the process that was interrupted has ended
	{
		dispatchEnqueue(interruptedProcess, getQueue(servicedProcess));
	}

	pthread_mutex_lock(&executeMuxex);
	processRunning = 0;
	pthread_mutex_unlock(&executeMuxex);
	
	pthread_mutex_lock(&schedulerMutex);
	pthread_cond_signal(&schedulerCondtion);
	pthread_mutex_unlock(&schedulerMutex);
}

/*Name: dispatchJob
*Purpose: This is the job dispatch function, this function is designed to form a process from a job
	once it can be assured that the system has the available resources. It will take a job from
	the job queue and make a call to jobPossible to determine if it is possible to run the job right now.
	If not, it will continue polling that function until it is possible to run the job. Afterward, it will
	enqueue it into the proper dispatch queue, using a switch based on process priority.
*Parameters:
	struct job *job --job to dispatch
Returns:
	void
Author: Andrew Gendreau
*/
void dispatchJob(struct job *job)
{
	struct process *newProcess;					//newly created process to be dispatched
	int processPriority;						//int used to keep track of process priority

	processPriority = job->priority;

	pthread_mutex_lock(&resourceMutex);
	newProcess = createProcess(job);
	pthread_mutex_unlock(&resourceMutex);
		
	switch(processPriority)
	{
		case 0:
			dispatchEnqueue(newProcess, q0Head);
			break;
		case 1:
			dispatchEnqueue(newProcess, q1Head);
			break;
		case 2:
			dispatchEnqueue(newProcess, q2Head);
			break;
		case 3:
			dispatchEnqueue(newProcess, q3Head);
			break;
		default:
			printf("We should never have gotten here!\n");
			exit(1);
	}
}
