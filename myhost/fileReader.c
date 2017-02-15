/*NAME: Andrew Gendreau
DATE: Oct 31st 2015
VERSION: 1.0
FILENAME: fileReader.c
PURPOSE: This program will read the input file in.
	It will then take the input and form process structs,
	then place them in the job queue for the dispatcher.
*/

#include <stdio.h>
#include <stdlib.h>
#include "job.h"
#include "jobQueue.h"

#define LINELEN 100							//constant for the length of the input line

void fileClose(FILE *);

extern struct job *head;

/**
*Name: fileOpen
*Purpose: Will open the file input in read mode. Input is assumed to exist.
*	if it does not exist, it will print an error and die.
*Parameters: 
*	void
*Returns:
Pointer to the opened file
*Author: Andrew Gendreau
**/
FILE *fileOpen(void)
{
	FILE *inputPointer;							//file pointer to the input file

	inputPointer = fopen("input.txt", "r");

	if(inputPointer == NULL)
	{
		fprintf(stderr, "File opening error!\n");
		exit(1);
	}
	else
	{
		return inputPointer;
	}
}

/**
*Name: readLine
*Purpose: Will read a line from input. will return null if the file is empty.
*Parameters: 
*	inputFile --open filePointer to input
*Returns:
*	newly formed job struct
*Author: Andrew Gendreau
**/
struct job *readLine(FILE *inputFile)
{
	struct job *jobOutput;							//process struct pointer to be returned from this function
	char lineBuffer[LINELEN];						//character array to hold the line
	int arrivalTime;							//process arrival time
	int priority;								//process priority
	int serviceTime;							//process service time
	int memoryRequested;							//process memory requested
	int numOfPrinters;							//number of printers the process needs
	int numOfScanners;							//number of scanners the process needs
	int numOfModems;							//number of modems the process needs
	int numOfCDs;								//number of CDs the process needs
	


	sscanf(lineBuffer, "%d, %d, %d, %d, %d, %d, %d, %d", &arrivalTime, &priority, &serviceTime, 
		&memoryRequested, &numOfPrinters, &numOfScanners, &numOfModems, &numOfCDs); 

	jobOutput = createJob(arrivalTime, priority, serviceTime, memoryRequested, numOfPrinters,
					numOfScanners, numOfModems, numOfCDs);

	return jobOutput;
}

/**
*Name: fileClose
*Purpose: Will close the file input
*Parameters: 
*	inputFile --open filePointer
*Returns:
*	void
*Author: Andrew Gendreau
**/
void fileClose(FILE *inputFile)
{
	fclose(inputFile);
}

/**
*Name: readFile
*Purpose: Will read the input file by using readLine and 
	will enqueue the jobs in the job queue until it hits the end of the file.
	Then, it will close the file.
*Parameters: 
*	void
*Returns:
*	head of the queue
*Author: Andrew Gendreau
**/
struct job *readFile(void)
{
	struct job *jobRead;							//last job read from input
	FILE *inputFile;							//file pointer to the input file
	char lineBuffer[LINELEN];						//character array to hold the line
	int arrivalTime;							//process arrival time
	int priority;								//process priority
	int serviceTime;							//process service time
	int memoryRequested;							//process memory requested
	int numOfPrinters;							//number of printers the process needs
	int numOfScanners;							//number of scanners the process needs
	int numOfModems;							//number of modems the process needs
	int numOfCDs;								//number of CDs the process needs

	inputFile = fileOpen();							//call to fileOpen to open the file

	while(fgets(lineBuffer, sizeof(lineBuffer), inputFile) != NULL)
	{
		sscanf(lineBuffer, "%d, %d, %d, %d, %d, %d, %d, %d", &arrivalTime, &priority, &serviceTime, 
		&memoryRequested, &numOfPrinters, &numOfScanners, &numOfModems, &numOfCDs); 


		jobRead = createJob(arrivalTime, priority, serviceTime, memoryRequested, numOfPrinters,
					numOfScanners, numOfModems, numOfCDs);

		enqueue(jobRead);
	}

	fileClose(inputFile);	

	return head;
}
