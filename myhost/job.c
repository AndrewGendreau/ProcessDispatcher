/*NAME: Andrew Gendreau
DATE: Oct 29th 2015
VERSION: 1.0
FILENAME: job.c
PURPOSE: This will be a module to represent a newly submitted but not yet executed process/job
*/

#include <stdio.h>
#include <stdlib.h>
#include "job.h"

/**
*Name: createJob
*Purpose: Will create a job given the file input.
*Parameters: 
*	arrivalTime: process arrival time
*	priority: process priority
*	serviceTime: process serviceTime
*	memoryRequested: memory the process needs
*	numOfPrinters: number of printers needed
*	numOfScanners: number of scanners
*	numOfModems: number of modems
*	numOfCDs: number of CDs
*Returns:
Pointer to the new job
*Author: Andrew Gendreau
**/
struct job *createJob(int arrivalTime, int priority, int serviceTime, 
		int memoryRequested, int numOfPrinters, int numOfScanners,
		int numOfModems, int numOfCDs)
{
	struct job *newJob;					//struct pointer for the new job


	newJob = malloc(sizeof(struct job));

	if(newJob == NULL)
	{
		fprintf(stderr, "Malloc failure!\n");
		exit(1);
	}
	
	newJob->arrivalTime = arrivalTime;
	newJob->priority = priority;
	newJob->serviceTime = serviceTime;
	newJob->memoryRequested = memoryRequested;
	newJob->numberOfPrinters = numOfPrinters;
	newJob->numberOfScanners = numOfScanners;
	newJob->numberOfModems = numOfModems;
	newJob->numberOfCDs = numOfCDs;

	return newJob;
}
