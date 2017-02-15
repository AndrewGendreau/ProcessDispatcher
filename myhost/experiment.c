/*NAME: Andrew Gendreau
DATE: Nov 7 2015
VERSION: 1.0
FILENAME: experiment.c
PURPOSE: This will be a thread to syncronize the other threads on. This thread will
	call feedback scheduler endlessly. It will be terminated when myhost recieves the completion signal and terminates.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

#ifndef _JOBQUEUE_H_
#define _JOBQUEUE_H_
#include "jobQueue.h"
#endif

#ifndef _EXECUTINGPROCESS_H_
#define _EXECUTINGPROCESS_H_
#include "executingProcess.h"
#endif

#ifndef _FILEREADER_H_
#define _FILEREADER_H_
#include "fileReader.h"
#endif

#ifndef _DISPATCHER_H_
#define _DISPATCHER_H_
#include "dispatcher.h"
#endif

#ifndef _DISPATCH_H_
#define _DISPATCH_H_
#include "dispatchQueue.h"
#endif

#ifndef _RESOURCE_H_
#define _RESOURCE_H_
#include "resources.h"
#endif

#include "experiment.h"

pthread_t systemThread;						//thread ID of the system thread
int systemTime;							//total elapsed system time
int experimentCompletion;					//int to signify the experiment is complete.
struct dispatchQueue *q0Head;					//pointer to the head of the dispatch queue 0 (real time priority)
struct dispatchQueue *q1Head;					//pointer to the head of the dispatch queue 1 (first level normal)
struct dispatchQueue *q2Head;					//pointer to the head of the dispatch queue 2 (second level normal)
struct dispatchQueue *q3Head;					//pointer to the head of the dispatch queue 3 (third level normal)
extern struct job *head;					//pointer to the head of the job queue
int processRunning;						//global variable to define if a thread is running right now
extern pthread_mutex_t completeMutex;				//mutex to protect the completion condition variable
extern pthread_cond_t completeCondition;			//condition variable that will be signaled upon completion
pthread_mutex_t schedulerMutex;					//mutex to protect the completion condition variable
pthread_cond_t schedulerCondtion;				//condition to be scheduled when the scheduler is to be invoked
struct process *runningProcess;					//currently running process
pthread_mutex_t runningMutex;					//mutex to protect the currently running process
pthread_mutex_t resourceMutex;					//mutex to protect the resource struct

/*Name: newProcessPauseHandler
*Purpose: This function will be a signal handler for SIGUSR1. 
	Upon reciept, it will call pause, which will stop the thread and
	not return until another handled signal (SIGUSR2 in this case) is recieved and handled.
*Parameters:
	 int signum --signal number
Returns:
	void
Author: Andrew Gendreau
*/
void newProcessPauseHandler(int signum)
{
	printf("pause handler was called\n");
	pause();
	printf("Pause returned\n");
	signal(signum, newProcessPauseHandler);
}

/*Name: newProcessResumeHandler
*Purpose: This function will be a signal handler for SIGUSR2. 
	Upon reciept, it will call do nothing really. Its only purpose
	is to make pause return.
*Parameters:
	 int signum --signal number
Returns:
	void
Author: Andrew Gendreau
*/
void newProcessResumeHandler(int signum)
{
	signal(signum, newProcessResumeHandler);
}

/*Name: simulation
*Purpose: This function will be the thread function for the system thread.
	This function will create the condition variable and mutex related to the scheduler and
	the running mutex, set the signal handers, create the queues and spawn the job dispatcher thread.
	It will then sleep, giving the job dispatch thread some time to work. Then enter a purposefully infinite
	loop and invoke the scheduler every iteration, acting as if it does it every second.
*Parameters:
	 void *input --void pointer to thread input, won't be used.
Returns:
	void
Author: Andrew Gendreau
*/
void *simulation(void *input)
{	
	systemTime = 0;

	pthread_mutex_init(&schedulerMutex, NULL);

	pthread_mutex_init(&schedulerMutex, NULL);
	
	pthread_cond_init(&schedulerCondtion, NULL);

	pthread_mutex_init(&resourceMutex, NULL);

	runningProcess = NULL;

	signal(SIGUSR1, newProcessPauseHandler);

	signal(SIGUSR2, newProcessResumeHandler);

	processRunning = 0;

	q0Head = createQueue();

	q1Head = createQueue();

	q2Head = createQueue();

	q3Head = createQueue(); 
	
	resourceInitalization();

	head = readFile();

	jobDispatchSpawn();

	usleep(1000);

	while(1)
	{ 
		feedbackScheduler();
	}
	pthread_exit(NULL);
}

/*Name: runExperiment
*Purpose: This function will simply begin the thread to execute simulation
*Parameters:
	 void
Returns:
	void
Author: Andrew Gendreau
*/
void runExperiment(void)
{
 	pthread_create(&systemThread, NULL, &simulation, NULL);
}
