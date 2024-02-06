/*
 * Author: Gabinete, Keith Ginoel S.
 * Section: CMSC 125 - CD-7L
 * Program Description: This is a simple program that simulates the Shortest Remaining Processing Time (SRPT) algorithm
 * in scheduling a given list of processes. 
 */


#include <stdio.h>
#include <stdlib.h>
#define MAX 10


/* Process Control Block (PCB) */
typedef struct PCB {
	int process_id;
	int arrival_time;
	int job_length;
	int wait_time;
	int run_time;
}PCB;


/* Singly-linked list of PCB */
typedef struct linked_list{
	PCB pcb;
	struct linked_list *next;
}linked_list;


/* Stores the processes */
linked_list *ready_queue = NULL;


/* Prints the details of the process */
void PrintPCB(PCB pcb){
	printf("\tProcess ID: %d, Arrival Time: %d, Job Length: %d, Wait Time: %d, Run Time: %d\n", pcb.process_id, pcb.arrival_time, pcb.job_length, pcb.wait_time, pcb.run_time);
}


/* Prints the processes in the ready queue */
void PrintReadyQueue(){
	linked_list *ptr = ready_queue;
	
	printf("\n\tPrinting Ready Queue:\n");

	while(ptr!=NULL){
		PrintPCB(ptr->pcb);
		ptr = ptr->next;
	}
}


/* 
 * Reads from process.txt
   * First line of the file is the process count
   * Succeeding lines are the <arrival_time> <job_length> values
   * Assume that the processes are sorted in ascending order by arrival time
 * Stores <arrival_time> to arrival_time array
 * Stores <job_length> to job_length array
 * Returns the process count
 */
int ReadProcesses(int *arrival_time, int *job_length){
	int arrival, length, pcnt;
	FILE *fp;

	// open a file for reading and writing
	fp = fopen("process.txt", "rw");

	// for reading inputs from a file
	// will store the unformatted fethed inputs
	char fgets_pcnt[128];
	char fgets_process[128];
	
	if(fp != NULL){
		//get the process
		fgets(fgets_pcnt, sizeof(fgets_pcnt), fp);
		sscanf(fgets_pcnt, " %d ", &pcnt);			// store it in the pcnt int variable

		// read the each line from the input file
		// each line contains a pair of a process’ arrival time and job length
		// store the fetched data into arrays
		for (int i=0; i<pcnt; i++) {
			fgets(fgets_process, sizeof(fgets_process), fp);
			sscanf(fgets_process, " %d %d", arrival_time, job_length); // store in the array

			// update pointers (point to the next index of the array)
			arrival_time++;
			job_length++;
		}

		// close the file
		fclose(fp);
	}
	return pcnt;
}


/* 
 * Prints the terminated process
 * Removes the terminated process at the head of the queue
 */
void Dequeue(){	
	if (ready_queue==NULL) return; 		// for safety purposes

	printf("\tThe following process already ended:\n");
	PrintPCB(ready_queue->pcb);

	// delete at head
	linked_list *ptr = ready_queue;

	// change the head pointer
	// point ready_queue to its current second element (will serve as the new head of the linked list)
	ready_queue = ready_queue->next;

	// deallocate memory to avoid leaks
	free(ptr);
}


/* Inserts the process in the queue */
void Enqueue(int process_id, int arrival, int job_length){
	linked_list *ptr;

	// insert new process at tail
	// check if the ready queue is empty or not
	if (ready_queue==NULL) {
		// initialize the queue
		ready_queue = (linked_list *) malloc(sizeof(linked_list));
		
		// initialize members' values
		ready_queue->pcb.process_id = process_id;
		ready_queue->pcb.arrival_time = arrival;
		ready_queue->pcb.job_length = job_length;
		ready_queue->pcb.run_time = 0;
		ready_queue->pcb.wait_time = 0;

		ready_queue->next = NULL;

	} else {
		// poins the pointer to where ready_queue is pointing at
		ptr = ready_queue;

		// find the tail of the the linked list
		while (ptr->next!=NULL) {
			ptr = ptr->next;
		}

		// append a new item at the tail of the linked list
		ptr->next = (linked_list *) malloc(sizeof(linked_list));

		// initialize members' values
		ptr->next->pcb.process_id = process_id;
		ptr->next->pcb.arrival_time = arrival;
		ptr->next->pcb.job_length = job_length;
		ptr->next->pcb.run_time = 0;
		ptr->next->pcb.wait_time = 0;

		ptr->next->next = NULL;
	}
}


/*
 * Simulates processing of a single CPU cycle
 * Increases the run time of the currently executing process
   * Currently executing processs is the first process in the queue
 * Increases the wait time of the other processes in the queue
 * Checks if a process terminates (run time == job length)
 */
void Processing(){
	if (ready_queue==NULL) return; 		// for safety purposes

	// 1. apply the Shortest Remaining Processing Time (SRPT) algorithm
	// compute for the current process' remaining execution time
	int current_remaining_time = ready_queue->pcb.job_length-ready_queue->pcb.run_time;

	// find the tail of the list
	linked_list *tail = ready_queue;
	while (tail->next!=NULL ) {
		tail = tail->next;
	}
	
	if (tail!=NULL) {
		// compare the tail's (or new process) execution time with the computed remaining time being executed by the CPU
		// if the former is less than the latter, make the tail (or new process) be the new head of the queue
		if (tail->pcb.job_length < current_remaining_time) {
			// points the 'next' member of the tail node to the current head of the queue
			tail->next = ready_queue;

			// find the parent of the tail node (new process)
			// if found, just point it to nothing (NULL)
			linked_list *temp = ready_queue;
			while (temp != NULL) {
				if (temp->next->pcb.process_id == tail->pcb.process_id) {
					temp->next = NULL;
				}
				temp = temp->next;
			}

			// make the tail (new process) the new head of the queuq
			ready_queue = tail;
		}		
	}

	// 2. increment the run time of the currently executing process
	ready_queue->pcb.run_time++; 		
	
	// 3. increment the wait time of other processes in the queue
	linked_list *ptr = ready_queue->next;
	while (ptr!=NULL) {
		ptr->pcb.wait_time++;			
		ptr = ptr->next;				
	}

	// 4. check if the process is should be terminated already
	if (ready_queue->pcb.run_time == ready_queue->pcb.job_length) {
		Dequeue();
	}
}


/* 
 * Simulates the CPU scheduling
 * Checks the arrival of processes in a given cpu cycle
 * Updates the processes in the queue
 * Prints the processes in the ready queue
 */
void RunSimulation(int *arrival_time, int *job_length, int process_count){
	int i=0, cpu_cycles=1;
	
	while(1){
		printf("\nCPU Cycle: %d\n", cpu_cycles);
		
		// for checking
		// printf("Arrival Time: %d\n", *arrival_time);
		// printf("I: %d\n", i);

		// enqueue a process if arrival time is equivalent to the current cpu cycle
		// enqueue a process if arrival time is equivalent to the current cpu cycle
		for (i=0; i<process_count; i++) {
			if (arrival_time[i] == cpu_cycles) {
				Enqueue(i, arrival_time[i], job_length[i]);
			}
		}

		Processing();
		PrintReadyQueue();
		cpu_cycles++;

		if(ready_queue == NULL) break; 		// terminate the program when there's no more process to execute
		
	}
}


int main(){
	int arrival_time[MAX], job_length[MAX];

	// read processes' data from a file
	int process_count = ReadProcesses(arrival_time, job_length);

	// for checking
	// for (int i=0; i<process_count; i++) {
	// 	printf("Process' Arrival Time:\t%d\n", arrival_time[i]);
	// 	printf("Process' Job Length:\t%d\n", job_length[i]);
	// 	printf("\n");
	// }

	// for checking
	// printf("======================================================================================");
	// printf("\nProcess count: %d\n", process_count);
	// for (int i=0; i<process_count; i++) {
	// 	Enqueue(i, arrival_time[i], job_length[i]);
	// }
	// PrintReadyQueue();

	RunSimulation(arrival_time, job_length, process_count);

	return 0;
}
