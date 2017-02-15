#NAME: Andrew Gendreau
#DATE: Nov 1st 2015
#VERSION: 1.0
#FILENAME: makefile
#PURPOSE: This is the makefile for myhost 
#AUTHOR: Andrew Gendreau

SOURCES = job.c jobQueue.c fileReader.c executingProcess.c myhost.c dispatchQueue.c dataOutput.c resources.c dispatcher.c experiment.c
OBJECTS = job.o jobQueue.o fileReader.o executingProcess.o myhost.o dispatchQueue.o dataOutput.o resources.o dispatcher.o experiment.o
CC = gcc
CFLAGS = -g -Wall
LIBRARIES = -pthread 

#all rule, says to create an executable from all the compiled object files called myhost.
#AUTHOR: Andrew Gendreau
all: myhost

myhost: $(OBJECTS)
	$(CC) $(CFLAGS) -o myhost $(OBJECTS) $(LIBRARIES)

#rule for creation of the job.o object file
#AUTHOR: Andrew Gendreau
job.o: job.c job.h
	$(CC) $(CFLAGS)  -c job.c

#rule for the creation of the jobQueue.o object file
#AUTHOR: Andrew Gendreau
jobQueue.o: jobQueue.c jobQueue.h
	$(CC) $(CFLAGS) -c jobQueue.c

#rule for the creation of the fileReader.o object file
#AUTHOR: Andrew Gendreau
fileReader.o: fileReader.c fileReader.h
	$(CC) $(CFLAGS)  -c fileReader.c

##rule for the creation of the executingProcess.o object file
#AUTHOR: Andrew Gendreau
executingProcess.o: executingProcess.c executingProcess.h
	$(CC) $(CFLAGS) -c executingProcess.c

#rule for the creation of the dispatchQueue.o object file
#AUTHOR: Andrew Gendreau
dispatchQueue.o: dispatchQueue.c dispatchQueue.h
	$(CC) $(CFLAGS) -c dispatchQueue.c

#rule for the creation of the dataOutput.o object file
#AUTHOR: Andrew Gendreau
dataOutput.o: dataOutput.c dataOutput.h
	$(CC) $(CFLAGS) -c dataOutput.c

#rule for the creation of the resources.o object file
#AUTHOR: Andrew Gendreau
resources.o: resources.c resources.h
	$(CC) $(CFLAGS) -c resources.c

#rule for the creation of the myhost.o object file
#AUTHOR: Andrew Gendreau
myhost.o: myhost.c
	$(CC) $(CFLAGS) -c myhost.c

#rule for the creation of the dispatcher.o object file
#AUTHOR: Andrew Gendreau
dispatcher.o: dispatcher.c dispatcher.h
	$(CC) $(CFLAGS) -c dispatcher.c

#rule for the creation of the experiment.o object file
#AUTHOR: Andrew Gendreau
experiment.o: experiment.c experiment.h
	$(CC) $(CFLAGS) -c experiment.c

#clean rule
#AUTHOR: Andrew Gendreau
clean:
	-rm myhost $(OBJECTS) 
