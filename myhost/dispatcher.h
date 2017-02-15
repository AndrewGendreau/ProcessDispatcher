/*NAME: Andrew Gendreau
DATE: Nov 3 2015
VERSION: 1.0
FILENAME: dispatcher.c
PURPOSE: This will be a module to perform the process dispatching. The functions of this
	module will be fed created processes and dispatch them to their respective queues.
	It will have defined constants to represent each queue's time quantum.
*/

#ifndef _JOB_H_
#define _JOB_H_
#include "job.h"
#endif

#ifndef _EXECUTINGPROCESS_H_
#define _EXECUTINGPROCESS_H_
#include "executingProcess.h"
#endif

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
void dispatchRealTime(struct process *);

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
void dispatchNormal(struct process *, int);

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
void dispatchJob(struct job *);

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
void feedbackScheduler(void);

/*Name: getQueue
*Purpose: This will be used to get the queue to re enqueue the given process in.
	This will use a switch to based on the process priority to decide which queue to use.
*Parameters:
	struct process *interruptedProcess --process that was interrupted
Returns:
	struct dispatchQueue * --head of whatever queue we want to use
Author: Andrew Gendreau
*/
struct dispatchQueue *getQueue(struct process *);
