/*NAME: Andrew Gendreau
DATE: Oct. 29th, 2015
VERSION: 1.0
FILENAME: process.h
*/

/**
*Purpose: Defines the data structure to represent a newly submitted job
*Author: Andrew Gendreau
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct job
{
	int arrivalTime;			
	int priority;
	int serviceTime;
	int memoryRequested;
	int numberOfPrinters;
	int numberOfScanners;
	int numberOfModems;
	int numberOfCDs;
	struct job *nextJob;
};

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
struct job *createJob(int, int, int, int, int, int, int, int);
