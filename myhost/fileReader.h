/*NAME: Andrew Gendreau
DATE: Oct 31st 2015
VERSION: 1.0
FILENAME: fileReader.c
PURPOSE: This program will read the input file in.
	It will then take the input and form process structs,
	then place them in the job queue for the dispatcher.
*/

/**
*Name: readLine
*Purpose: Will read a line from input. will return null if the file is empty.
*Parameters: 
*	inputFile --open filePointer to input
*Returns:
*	newly formed job struct
*Author: Andrew Gendreau
**/
struct job *readLine(void);

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
struct job *readFile(void);
