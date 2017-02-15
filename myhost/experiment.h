/*NAME: Andrew Gendreau
DATE: Nov 7 2015
VERSION: 1.0
FILENAME: experiment.c
PURPOSE: This will be a thread to syncronize the other threads on. This thread will
	call feedback scheduler endlessly. It will be terminated when myhost recieves the completion signal and terminates.
*/

/*Name: runExperiment
*Purpose: This function will simply begin the thread to execute simulation
*Parameters:
	 void
Returns:
	void
Author: Andrew Gendreau
*/
void runExperiment(void);

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
void *simulation(void *);
