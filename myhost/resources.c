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

#define SYSTEMPRINTERS 2
#define SYSTEMSCANNERS 1
#define SYSTEMMODEMS 1
#define SYSTEMCDDRIVES 2
#define SYSTEMMEMORY 1024

#include "resources.h"
#include <stdio.h>
#include <stdlib.h>

struct resources *availableResources;						//struct of available system resources

struct resources totalSystemResources =						//total system resources struct, set at the beginning
{
	SYSTEMPRINTERS, 
	SYSTEMSCANNERS,
	SYSTEMMODEMS,
	SYSTEMCDDRIVES,
	SYSTEMMEMORY
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
void resourceInitalization(void)
{
	availableResources = malloc(sizeof(struct resources));

	if(availableResources == NULL)
	{
		fprintf(stderr, "Malloc failure!\n");
		exit(1);
	}

	availableResources->printers = totalSystemResources.printers;
	availableResources->scanners = totalSystemResources.scanners;
	availableResources->modems = totalSystemResources.modems;
	availableResources->cdDrives = totalSystemResources.cdDrives;
	availableResources->memory = totalSystemResources.memory;

}

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
int jobPossible(struct job *job)
{
	int resourceNeedMet;							//int to act like a boolean
	int enoughPrinters;							//int to check for printer availability
	int enoughScanners;							//int to check for scanner availability
	int enoughModems;							//int to check for modem availability
	int enoughCDDrives;							//int to check for cd drive availability
	int enoughMemory;							//int to check for memory availability
				
	resourceNeedMet = 1;							//initally 1, assuming worst at first

	enoughPrinters = availableResources->printers - job->numberOfPrinters;

	if(enoughPrinters < 0)
	{
		return resourceNeedMet;
	}

	enoughScanners = availableResources->scanners - job->numberOfScanners;
	
	if(enoughScanners < 0)
	{
		return resourceNeedMet;
	}

	enoughModems = availableResources->modems - job->numberOfModems;
	
	if(enoughModems < 0)
	{
		return resourceNeedMet;
	}

	enoughCDDrives = availableResources->cdDrives - job->numberOfCDs;

	if(enoughCDDrives < 0)
	{
		return resourceNeedMet;
	}

	enoughMemory = availableResources->memory - job->memoryRequested;

	if(enoughMemory < 0)
	{
		return resourceNeedMet;
	}

	resourceNeedMet = 0;
	return resourceNeedMet;
}

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
struct process *resourceAllocation(struct process *process, struct job *job)
{
	int printersRemaining;
	int scannersRemaining;
	int modemsRemaining;
	int cdDrivesremaining;
	int memoryRemaining;

	printersRemaining =  availableResources->printers - job->numberOfPrinters;

	scannersRemaining =  availableResources->scanners - job->numberOfScanners;

	modemsRemaining = availableResources->modems - job->numberOfModems;

	cdDrivesremaining = availableResources->cdDrives - job->numberOfCDs;

	memoryRemaining = availableResources->memory - job->memoryRequested;

	process->printersHeld = job->numberOfPrinters;
	availableResources->printers = printersRemaining;

	process->scannersHeld = job->numberOfScanners;
	availableResources->scanners = scannersRemaining;

	process->modemsHeld = job->numberOfModems;
	availableResources->modems = modemsRemaining;

	process->cdDrivesHeld = job->numberOfCDs;
	availableResources->cdDrives = cdDrivesremaining;

	process->MBMemoryHeld = job->memoryRequested;
	availableResources->memory = memoryRemaining;

	return process;
}

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
void deallocateResources(struct process *process)
{
	availableResources->printers =  availableResources->printers + process->printersHeld;

	availableResources->scanners =  availableResources->scanners + process->scannersHeld;

	availableResources->modems =  availableResources->modems + process->modemsHeld;

	availableResources->cdDrives =  availableResources->cdDrives + process->cdDrivesHeld;

	availableResources->memory =  availableResources->memory + process->MBMemoryHeld;
}
