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

#ifndef _JOB_H_
#define _JOB_H_
#include "job.h"
#endif

/**
*Name: process
*Purpose: This is a struct designed to simulate a process control block. This will
	hold the needed fields as well as a thread ID to the thread to simulate execution.
*	
*Author: Andrew Gendreau
**/
struct process
{
	int arrivalTime;						//time the process arrived
	int serviceTime;						//time the process needs to execute
	int timeExecuted;						//time the process has spent executing
	int priority;							//process priority
	int printersHeld;						//printers held
	int scannersHeld;						//scanners held
	int modemsHeld;							//modemsHeld
	int cdDrivesHeld;						//cd drives held
	int MBMemoryHeld;						//mega bytes of memory held
	pthread_t thread;						//thread ID of the thread assocated with this struct
	int myTurn;							//int to act like a boolean to track if it is the process' turn
	int hasExecuted;						//int to keep track of whether this process has had processor time
	int startTime;							//time the process began execution
	int finishTime;							//time the process finished
	pthread_cond_t yourTurnCondition;				//condition variable to control process execution
	int timeServiced;						//time the process has been serviced
};

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
void *NormalProcessExecute(void *);

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
void *RealTimeExecute(void *);

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
struct process *createProcess(struct job *);

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
void beginExecution(struct process *);

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
struct process *interrupt(void);

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
void resume(struct process *);
