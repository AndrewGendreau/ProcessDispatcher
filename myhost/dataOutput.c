/*NAME: Andrew Gendreau
DATE: Nov 3 2015
VERSION: 1.0
FILENAME: dataOutput.c
PURPOSE: This will be a module to output the data from the simulation into
	a text file for anaylsis. The file format will be as follows:
priority	service time	arrivalTime	start time	finish time  idle time	turnaround time		normalized turnaround time
*/
#include <pthread.h>
#include "dataOutput.h"

extern int systemTime;								//total elapsed system time

/*Name: idleTime
*Purpose: This function will calculate the idle time. 
	This wil be calculated by first storing in temp the process arrival time.
	It will then subtract temp from the service time
*Parameters:
	int serviceTime --time the process executed
	int arrivalTime --time the process arrived
Returns:
	int --the idle time
Author: Andrew Gendreau
*/
int idleTime(int serviceTime, int startTime)
{
	int temp;								//temporary variable for the calculation
	int idleTime;								//total idle time

	temp = systemTime - startTime;						//this gives us time elapsed since the process started

	idleTime = temp - serviceTime;						//subtracting temp from this gives us the time it was idle

	return idleTime;							
}

/*Name: turnaroundTime
*Purpose: This function will calculate the turnaround time. 
	This wil be calculated by adding the idle time to the service time.
*Parameters:
	int serviceTime --time the process executed
	int idleTime --time the process was idle
Returns:
	int --the turnaround time
Author: Andrew Gendreau
*/
int turnaroundTime(int serviceTime, int idleTime)
{
	int turnaroundTime;							//process turnaround time

	turnaroundTime = serviceTime + idleTime;

	return turnaroundTime;
}

/*Name: normalizedTurnaroundTime
*Purpose: This function will calculate the normalized turnaround time. 
	This wil be calculated by dividing the turnaround time by the service time.
*Parameters:
	int turnaroundTime --process turnaround time
	int serviceTime --time the process was serviced
Returns:
	double --the normalized turnaround time
Author: Andrew Gendreau
*/
double normalizedTurnaroundTime(int turnaroundTime, int serviceTime)
{
	double normalizedTurnaround;						//normalized turnaround time

	normalizedTurnaround = (double) turnaroundTime / (double) serviceTime;

	return normalizedTurnaround;
}

/*Name: outputData
*Purpose: This function will print the data into an output text file.
	It will invoke all the previous functions to calculate the numbers,
	then make use of a struct to store them and send them to the output file.
	It will then free the process, the output struct and close the file.
*Parameters:
	int turnaroundTime --process turnaround time
	int serviceTime --time the process was serviced
Returns:
	double --the normalized turnaround time
Author: Andrew Gendreau
*/
void outputData(struct process *finishedProcess)
{
	FILE *outputFile;							//experiment output file
	struct outputData *output;						//struct to hold the output to be appened to the file 
	
	int arrival;
	int start;
	int finish;
	int idle;
	int turnaround;
	double normalTurnaround;

	outputFile = fopen("output.txt", "a+");

	if(outputFile == NULL)
	{
		fprintf(stderr, "Output file could not be opened!\n");
		exit(1);
	}

	output = malloc(sizeof(struct outputData));

	if(output == NULL)
	{
		fprintf(stderr, "Malloc error!\n");
		exit(1);
	}

	arrival = finishedProcess->arrivalTime;
	output->arrivalTime = arrival;

	start = finishedProcess->startTime;
	output->startTime = start;

	finish = finishedProcess->finishTime;
	output->finishTime = finish;

	idle = idleTime(finishedProcess->serviceTime, finishedProcess->startTime);
	output->idleTime = idle;

	turnaround = turnaroundTime(finishedProcess->serviceTime, idle);
	output->turnaroundTime = turnaround;

	normalTurnaround = normalizedTurnaroundTime(turnaround, finishedProcess->serviceTime);
	output->normalizedTurnaround = normalTurnaround;

	fprintf(outputFile, "%-10d %-10d %-10d %-10d %-10d %-10d %-10d %-10lf\n", finishedProcess->priority, finishedProcess->serviceTime, arrival, start, finishedProcess->finishTime, idle, turnaround, normalTurnaround);


  	pthread_cond_destroy(&finishedProcess->yourTurnCondition);
	free(finishedProcess);
	free(output);

	fclose(outputFile);
	outputFile = NULL;
}
