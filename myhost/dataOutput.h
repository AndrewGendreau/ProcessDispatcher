/*NAME: Andrew Gendreau
DATE: Nov 3 2015
VERSION: 1.0
FILENAME: dataOutput.h
PURPOSE: This will be a module to output the data from the simulation into
	a text file for anaylsis. The file format will be as follows:
	idle Time	arrivalTime	start time	finish time	turnaround time		normalized turnaround time
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#ifndef _EXECUTINGPROCESS_H_
#define _EXECUTINGPROCESS_H_
#include "executingProcess.h"
#endif


extern int systemTime;								//total elapsed system time

/*Name: outputData
*Purpose: This is a struct for tempory storage of the output data. 
	It is simply used for ease of storage and output of the numbers.
Author: Andrew Gendreau
*/
struct outputData
{
	int idleTime;								//time the process was idle
	int arrivalTime;							//time the process arrived
	int startTime;								//time the process began
	int finishTime;								//time the process finished
	int turnaroundTime;							//time it took for the process to complete
	double normalizedTurnaround;						//normalized turn around time for comparison
};

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
void outputData(struct process *);
