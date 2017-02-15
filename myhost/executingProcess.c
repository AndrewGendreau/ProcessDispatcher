/*NAME: Andrew Gendreau
DATE: Nov 3 2015
VERSION: 1.0
FILENAME: executingProcess.c
PURPOSE: This will be a module to represent an executing process.
	This will be represented by a struct that will keep track 
	of the following information:
	service time, arrival time, time executed, priority, held resources.
	This module uses threads to emulate process execution. Each process struct will
	contain a pthread that will start initally waiting for a signal to a condtion
	variable and will begin execution when that condition variable is signaled.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#ifndef _EXECUTINGPROCESS_H_
#define _EXECUTINGPROCESS_H_
#include "executingProcess.h"
#endif

#ifndef _DATAOUTPUT_H_
#define _DATAOUTPUT_H_
#include "dataOutput.h"
#endif

#ifndef _RESOURCE_H_
#define _RESOURCE_H_
#include "resources.h"
#endif

#ifndef _DISPATCHER_H_
#define _DISPATCHER_H_
#include "dispatcher.h"
#endif

extern int systemTime;							//int to keep track of elapsed system time
pthread_mutex_t executeMuxex;						//mutex to control process execution
extern int processRunning;						//global variable to define if a thread is running right now
extern pthread_mutex_t runningMutex;					//mutex to protect the currently running process
extern struct process *runningProcess;					//currently running process struct
extern pthread_mutex_t schedulerMutex;					//mutex to protect the scheduler condition variable
extern pthread_cond_t schedulerCondtion;				//condition variable used to signal the next scheduler invocation
extern pthread_mutex_t resourceMutex;					//mutex to protect the resource struct

/**
*Name: createProcess
*Purpose: Process creation function, this is to be used to form a process struct from a job struct.
	This function will take the arrival time, service time and priority from the job, set the has
	executed, myTurn and timeServiced fields to 0. It will then allocate the needed resources to the 
	process. Finally, it'll initalize the condition variable in the struct and call the begin execution
	function to begin its thread.
*Parameters: 
*	struct job *job --job to change to a process
	
*Returns:
*	struct process newProcess --pointer to the new process struct
*Author: Andrew Gendreau
**/
struct process *createProcess(struct job *job)
{
	struct process *newProcess;					//new process struct to be returned

	newProcess = malloc(sizeof(struct process));

	if(newProcess == NULL)
	{
		printf("Malloc error!\n");
		exit(1);
	}

	newProcess->arrivalTime = job->arrivalTime;
	newProcess->serviceTime = job->serviceTime;
	newProcess->priority = job->priority;
	newProcess->hasExecuted = 0;
	newProcess->myTurn = 0;
	newProcess->timeServiced = 0;

	resourceAllocation(newProcess, job);
	
	pthread_cond_init(&newProcess->yourTurnCondition, NULL);

	beginExecution(newProcess);

	return newProcess;
}

/**
*Name: beginExecution
*Purpose: This function will create the thread for the process structs. 
	If the priority of the process is 0, marking it as real time
	the thread will be passed the realTimeExecute function. If not,
	it will simply be passed the NormalProcessExecute function.
*Parameters: 
*	struct process *servicedProcess --process to spawn a thread for
	
*Returns:
*	void
*Author: Andrew Gendreau
**/
void beginExecution(struct process *newProcess)
{
	if(newProcess->priority == 0)
	{
		pthread_create(&newProcess->thread, NULL, &RealTimeExecute, newProcess);
	}
	else
	{
		pthread_create(&newProcess->thread, NULL, &NormalProcessExecute, newProcess);
	}
}

/**
*Name: interrupt
*Purpose: Will enact process interruption, this will work by first locking the runningMutex mutex,
	 It will then issue SIGUSR1 to the process to force it to stop. it will then make the 
	interruptedProcess variable a shallow copy of the currently running process. If the process
	has run to completion this will return null. It will then lock the executeMutex to change the 
	myTurn variable and the global processRunning int to 0, to signify it is not the process' turn
	and no process is running. It will then return a pointer to this process.
*Parameters: 
*	void
*Returns:
*	struct process *interruptedProcess --pointer to the interrupted process
*Author: Andrew Gendreau
**/
struct process *interrupt(void)
{
	struct process *interruptedProcess;

	pthread_mutex_lock(&runningMutex);

	pthread_kill(runningProcess->thread, SIGUSR1);

	interruptedProcess = runningProcess;

	if(interruptedProcess->timeServiced >= interruptedProcess->serviceTime)
	{
		pthread_mutex_unlock(&runningMutex);
		pthread_mutex_unlock(&executeMuxex);
		return NULL;
	}
	pthread_mutex_lock(&executeMuxex);
	interruptedProcess->myTurn = 0;
	processRunning = 0;
	pthread_mutex_unlock(&runningMutex);
	pthread_mutex_unlock(&executeMuxex);

	return interruptedProcess;
}

/**
*Name: resume
*Purpose: Will enact process resumption, this function will first lock the execute mutex and change
	the processRunning variable to 1 to signify a process is running now. It will then issue
	SIGUSR2	to the thread in the given struct to make it resume. It will then signal the condtion
	variable in the struct and unlock the mutex.
*Parameters: 
*	struct process *executingProcess --A pointer to the process to resume
*Returns:
*	void
*Author: Andrew Gendreau
**/
void resume(struct process *executingProcess)
{
	pthread_mutex_lock(&executeMuxex);
	processRunning = 1;
	pthread_kill(executingProcess->thread, SIGUSR2);
	pthread_cond_signal(&executingProcess->yourTurnCondition);
	pthread_mutex_unlock(&executeMuxex);
}

/**
*Name: RealTimeExecute
*Purpose: This will the thread function for real time processes. This function will
	start by locking the executeMutex mutex and waiting on the your turn condition
	in the struct. This is to prevent the thread from executing as soon as it's spawned.
	While it has not run to completion it will increment the time it has and the
	elapsed system time. After it has run to completion it will release its resources 
	and make a call to outputData, which will handle the rest of the cleanup. It will then
	signal the global schedulerCondtion to signal it is time to invoke the scheduler, before the
	thread is terminated.
*Parameters: 
*	void *executing process --void pointer to the process to execute
*Returns:
*	void
*Author: Andrew Gendreau
**/
void *RealTimeExecute(void *executingProcess)
{
	struct process *processPointer;					//struct pointer variable to assign the void pointer to
	int timeExecuted;						//time the process has executed

	processPointer = executingProcess;				//cast the void pointer to a struct pointer

	
	pthread_mutex_lock(&executeMuxex);
	pthread_cond_wait(&processPointer->yourTurnCondition, &executeMuxex);
	pthread_mutex_unlock(&executeMuxex);

	while(processPointer->timeServiced < processPointer->serviceTime)
	{
		printf("Currently executing process priority: %d\n", processPointer->priority);
		timeExecuted = processPointer->timeServiced + 1;
		processPointer->timeServiced = timeExecuted;
		systemTime++;
	}

	printf("arrival time %d Thread completed!\n", processPointer->arrivalTime);
	processPointer->finishTime = systemTime;
	pthread_mutex_lock(&resourceMutex);
	deallocateResources(processPointer);
	pthread_mutex_unlock(&resourceMutex);
	
	outputData(processPointer);

	pthread_mutex_lock(&schedulerMutex);
	pthread_cond_signal(&schedulerCondtion);
	pthread_mutex_unlock(&schedulerMutex);

	pthread_exit(NULL);
}

/**
*Name: NormalProcessExecute
*Purpose: This will the thread function for non real time processes. This function will
	start by locking the executeMutex mutex and waiting on the your turn condition
	in the struct. This is to prevent the thread from executing as soon as it's spawned.
	While it has not run to completion it will increment the time it has and the
	elapsed system time. The difference between this and the real time execution,
	is that it will wait to be signaled after every increment. This is meant to make it simpler
	to implement the time quantum contraint. After it has run to completion 
	it will release its resources and make a call to outputData,
	 which will handle the rest of the cleanup. It will then signal the global schedulerCondtion
	to signal it is time to invoke the scheduler, before the thread is terminated.
*Parameters: 
*	void *executing process --void pointer to the process to execute
*Returns:
*	void
*Author: Andrew Gendreau
**/
void *NormalProcessExecute(void *executingProcess)
{
	struct process *processPointer;
	processPointer = executingProcess;
	int timeExecuted;

	timeExecuted = 0;

	pthread_mutex_lock(&executeMuxex);
	pthread_cond_wait(&processPointer->yourTurnCondition, &executeMuxex);
	pthread_mutex_unlock(&executeMuxex);

	while(processPointer->timeServiced < processPointer->serviceTime)
	{
		printf("Currently executing process priority: %d\n", processPointer->priority);
		timeExecuted = processPointer->timeServiced + 1;
		processPointer->timeServiced = timeExecuted;
		systemTime++;
		pthread_mutex_lock(&executeMuxex);
		pthread_cond_wait(&processPointer->yourTurnCondition, &executeMuxex);
		pthread_mutex_unlock(&executeMuxex);
	}
	printf("arrival time %d Thread completed!\n", processPointer->arrivalTime);
	
	processPointer->finishTime = systemTime;
	pthread_mutex_lock(&resourceMutex);
	deallocateResources(processPointer);
	pthread_mutex_unlock(&resourceMutex);

	outputData(processPointer);

	pthread_mutex_lock(&executeMuxex);
	processRunning = 0;
	pthread_mutex_unlock(&executeMuxex);

	pthread_mutex_lock(&schedulerMutex);
	pthread_cond_signal(&schedulerCondtion);
	pthread_mutex_unlock(&schedulerMutex);
	pthread_exit(NULL);
}
