/*NAME: Andrew Gendreau
DATE: Oct. 30, 2015
VERSION: 1.0
FILENAME: myhost.c
PURPOSE: This will be a centralized control module. This will coordinate the other threads and control
	the system thread. It will also set a signal mask to block SIGUSR1 and SIGUSR2 so pause cannot affect it.
	It will spawn the system thread and initialize the completion condition variable and corresponding mutex.
	Once it spawns the thread it will wait on the completion condition and once that is signaled, it will free the 
	queues, the resources struct and destroy the completion and exection mutexes and the complete condtion variable.
	At which point it will return and exectuion will end.
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

#ifndef _EXPERIMENT_H_
#define _EXPERIMENT_H_
#include "experiment.h"
#endif

pthread_mutex_t executeMuxex;						//mutex to control process execution
extern struct resources *availableResources;				//struct used to keep track of system resources
extern 	pthread_t jobThread;						//thread variable of the job dispatch thread
extern pthread_t systemThread;						//thread varialbe of the experiment thread
pthread_mutex_t completeMutex;						//mutex to protect the completion condition variable
pthread_cond_t completeCondition;					//condition variable that is signaled once the processes are done
int experimentCompletion;						//int to signify the experiment is complete.
extern struct dispatchQueue *q0Head;					//pointer to the head of the dispatch queue 0 (real time priority)
extern struct dispatchQueue *q1Head;					//pointer to the head of the dispatch queue 1 (first level normal)
extern struct dispatchQueue *q2Head;					//pointer to the head of the dispatch queue 2 (second level normal)
extern struct dispatchQueue *q3Head;					//pointer to the head of the dispatch queue 3 (third level normal)
extern struct job *head;						//pointer to the head of the job queue

/**
*Name: main
*Purpose: Will initalize experimentCompletion to be 0, it will use a sigmask to block SIGUSR1 and 2.
	It will then initialize the completion mutex and corresponding condition variable. It will
	then spawn the system thread and begin the experiment through the call to runExperiment.
	It will then wait on the experiment's completion and unlock the corresponding mutex after it is signaled.
	After the loop exits, myhost will perform some cleanup work and then return 0.
	
*Parameters: void
*Returns: void
*Author: Andrew Gendreau
**/

int main()
{

	sigset_t set;							//signal set mask to be used to block SIGUSR1 and SIGUSR2

	experimentCompletion = 0;
	
	sigemptyset(&set);
	
	sigaddset(&set, SIGUSR1);
	sigaddset(&set, SIGUSR2);

	pthread_sigmask(SIG_SETMASK, &set, NULL);

	pthread_mutex_init(&completeMutex, NULL);
	pthread_cond_init(&completeCondition, NULL);
	
	runExperiment();

	usleep(1000);

	printf("jobthread return value: %d\n", pthread_join(jobThread, NULL));

	while(experimentCompletion == 0)
	{
		pthread_cond_wait(&completeCondition, &completeMutex);
		pthread_mutex_unlock(&completeMutex);
	}
	printf("Experiment complete\n");
	free(q0Head);
	free(q1Head);
	free(q2Head);
	free(q3Head);
	free(head);
	free(availableResources);
	pthread_mutex_destroy(&completeMutex);
	pthread_mutex_destroy(&executeMuxex);
	pthread_cond_destroy(&completeCondition);
	return 0;
}
