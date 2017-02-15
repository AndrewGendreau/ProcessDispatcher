/*NAME: Andrew Gendreau
DATE: Nov 4th 2015
VERSION: 1.0
FILENAME: resources.c
PURPOSE: This will be a module to keep track of both the total and currently available system resources.
	It will check if the job passed to it can be executed, and if it can it will allocate the needed
	resourcs to the process and will deallocate them when the process has completed. It will also possess
	a function to initalize the inital system resources when the experiment is first run. It will represent
	the total initally available by using the predefined constants.
*Author: Andrew Gendreau
*/
#ifndef _JOB_H_
#define _JOB_H_
#include "job.h"
#endif

#ifndef _EXECUTINGPROCESS_H_
#define _EXECUTINGPROCESS_H_
#include "executingProcess.h"
#endif

/**
*Name: resources
*Purpose: This is a struct to hold the resources, both available and total.
*Author: Andrew Gendreau
**/
struct resources
{
	int printers;							//printers in the system
	int scanners;							//scanners in the system
	int modems;							//modems in the system
	int cdDrives;							//CD drives in the system
	int memory;							//memory in the system (in MBs)
};

/**
*Name: resourceInitalization
*Purpose: Will initalize the available system resources. It will set the available resources
	struct values to be equal to the values in the total struct.
*Parameters: 
*	void
*Returns: 
	void
*Author: Andrew Gendreau
**/
void resourceInitalization(void);

/**
*Name: jobPossible
*Purpose: Will check the resource needs of the given job against the available resources.
	It will use an int to signal if it is possible, this int will be 1 if the job
	is not possible yet and 0 if it is. It will subtract the resource request of the 
	given job from the available resources and store that difference in the corresponding int.
	If that int is less than 0, we don't have the resources to run the job available.
*Parameters: 
*	struct job *job --job to check
*Returns: 
	int resourceNeedMet --int to signal if the job is possible right now.
*Author: Andrew Gendreau
**/
int jobPossible(struct job *);


/**
*Name: resourceAllocation
*Purpose: Will actually allocate the needed resources to the process.
	It will do this by simply setting the number of the corresponding resource
	that was requested to the corresponding held field in the process control block.
	It will update the available resources by setting the corresponding resource available
	field to the difference between what was initally available and the requested resource.
	int from the available resources.
*Parameters: 
*	struct process *process --process to allocate resources to
	struct job *job --job that was okayed to run.
*Returns: 
	struct process *process --the process that was passed in, now with resources allocated to it.
*Author: Andrew Gendreau
**/
struct process *resourceAllocation(struct process *, struct job *);

/**
*Name: deallocateResources
*Purpose: Will remove the allocated resources from the given process and update the 
	available struct. It will add the resource held field for each resource to the
	corresponding available field in the resources available struct.
*Parameters: 
*	struct process *process --process to remove resources from
*Returns: 
	void
*Author: Andrew Gendreau
**/
void deallocateResources(struct process *);
