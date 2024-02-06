/**
	Author: Gabinete, Keith Ginoel S.
	Section: CMSC 125 - CD-7L
    Lab Exercise 07 - A
	Program Description: 
		This is a simple program that simulates sharing of memory with another program to compute for the matrix product of two matrices..
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>	//for new types declared
#include<sys/ipc.h>		//for interprocess communication
#include<sys/shm.h>		//for shared memory
#define MY_KEY 0xC0FFEE


void readInputFile() {
	FILE *fp = fopen("input.txt","r"); //addresses to convert

	// variables for file reading
	// for the dimensions of matrix A
	char fgets_matAdim[128];	// storing the dimensions of matrix A
	int matA_size = 0, matA_nRow = 0, matA_nCol = 0;

	// for the dimensions of matrix B
	char fgets_matBdim[128];	// storing the dimensions of matrix B
	int matB_size = 0, matB_nRow = 0, matB_nCol = 0;

	// for reading of matrix contents
	int content = 0; 

	// for dumping a character
	char fgets_dump[128];
	// ===========================================================================================================================
	// Creating matrix A
	// get the dimension of matrix a
	fgets(fgets_matAdim, sizeof(fgets_matAdim), fp);
	sscanf(fgets_matAdim, " %d %d", &matA_nRow, &matA_nCol);			// store it in the num_of_addresses int variable

	// for checking
	printf("\nNumber of rows of Mat A: %d", matA_nRow);
	printf("\nNumber of columns of Mat A: %d\n", matA_nCol);

	// create an array for storing the contents of matrix A from input.txt
	matA_size = matA_nRow*matA_nCol;
	int matA[matA_size];

	// get the contents of the matrix
	content = 0;
	for (int i =0; i<matA_size; i++ ) {
		fscanf(fp, " %d", &content);
		matA[i] = content;
	}

	// dump the end of the line from previous reading of fscanf
	fgets(fgets_dump, sizeof(fgets_dump), fp);

	// for checking
	for (int row=0; row<matA_nRow; row++){
		for (int col=0; col<matA_nCol; col++){

			printf("[%d]: %d\n", col, matA[row*matA_nCol+col]);
		}
	}



	// ===========================================================================================================================
	// Creating matrix B

	// get the dimension of matrix a
	fgets(fgets_matBdim, sizeof(fgets_matBdim), fp);
	sscanf(fgets_matBdim, " %d %d", &matB_nRow, &matB_nCol);			// store it in the num_of_addresses int variable
	
	// for checking
	printf("\nNumber of rows of Mat B: %d", matB_nRow);
	printf("\nNumber of columns of Mat B: %d\n", matB_nCol);

	// create an array for storing the contents of matrix B from input.txt
	matB_size = matB_nRow*matB_nCol;
	int matB[matB_size];

	// get the contents of the matrix
	content = 0;
	for (int i =0; i<matB_size; i++ ) {
		fscanf(fp, " %d", &content);
		matB[i] = content;
	}

	// dump the end of the line from previous reading of fscanf
	fgets(fgets_dump, sizeof(fgets_dump), fp);

	// for checking
	for (int row=0; row<matB_nRow; row++){
		for (int col=0; col<matB_nCol; col++){
			printf("[%d]: %d\n", col, matB[row*matB_nCol+col]);
		}
	}
}


int main(){
	key_t key = MY_KEY;			// for creating a key-typed object
	int shmid;		//shared mem identifier of the shared-memory
	int *shared_memory;	//pointer to the shared memory
						      //in this case, we want a shared memory of integers
	int i;

readInputFile();

}



