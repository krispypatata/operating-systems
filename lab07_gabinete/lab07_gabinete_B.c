/**
	Author: Gabinete, Keith Ginoel S.
	Section: CMSC 125 - CD-7L
    Lab Exercise 07 - B
	Program Description: 
		This is a simple program that simulates sharing of memory with another program to compute for the matrix product of two matrices..
 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>	//for new types declared
#include<sys/ipc.h>		//for interprocess communication
#include<sys/shm.h>		//for shared memory
#define MAT_A_KEY 0x64
#define MAT_B_KEY 0xC8
#define MAT_RESULT 0x12C
#define MAT_SIZE 0x190

int main(){
    // for creating key-typed objects
	key_t matA_key = MAT_A_KEY;			
    key_t matB_key = MAT_B_KEY;
    key_t matResult_key = MAT_RESULT;
    key_t matSize_key = MAT_SIZE;

    // shared mem identifier of the shared-memory locations
	int matA_shmid;	
    int matB_shmid;	
    int matResult_shmid;	
    int matSize_shmid;		  

    // pointers to the shared memories
	int *shared_matA;	    
    int *shared_matB;       
	int *shared_matResult;
    int *shared_matSize; 

	// for matrices' dimensions
	int matA_nRow, matA_nCol, matA_size;
	int matB_nRow, matB_nCol, matB_size;
	int matResult_nRow, matResult_nCol, matResult_size;

	// ===============================================================================================================================
	// STEP 1 & STEP 2:
	// 1a.
	// create a shared memory segment using shmget for the shared memory that contains the dimensions of matrices A and B
	matSize_shmid = shmget(matSize_key, 4*sizeof(int), 0666);

	// check for errors
	if(matSize_shmid < 0){
		printf("shmget error\n");
		exit(1);
	}

	// 2a.
	// attach the shared memory to this process
	shared_matSize = shmat(matSize_shmid, NULL, 0);

	//check for errors
	if(shared_matSize == (int *) -1) {
		printf("shmat error\n");
		exit(1);
	}

	// ===============================================================================================================================
	// extract the contents of the shared memory
	// matrix B dimensions
	matA_nRow = shared_matSize[0];
	matA_nCol = shared_matSize[2];

	// matrix B dimensions
	matB_nRow = shared_matSize[1];
	matB_nCol = shared_matSize[3];

	// resulting matrix dimensions
	matResult_nRow = matA_nRow;
	matResult_nCol = matB_nCol;

	// sizes of the three matrices
	matA_size = matA_nRow*matA_nCol;
	matB_size = matB_nRow*matB_nCol;
	matResult_size = matResult_nRow*matResult_nCol;

	// ===============================================================================================================================
	//	1b.
	// create shared memory segments for the other shared memories (given the extracted contents)
	puts("\n=============================================================================="); 
	puts("Creating Memory Segments for the shared memories created in lab07_gabinete_A.c");
	usleep(500000); // 0.5 seconds
	matA_shmid = shmget(matA_key, matA_size*sizeof(int), 0666);
	// check for errors
	if (matA_shmid < 0){
		printf("shmget error: matA_shmid\n");
		exit(1);
	}

    matB_shmid = shmget(matB_key, matB_size*sizeof(int), 0666);
	// check for errors
	if (matB_shmid < 0){
		printf("shmget error: matB_shmid\n");
		exit(1);
	}

    matResult_shmid = shmget(matResult_key, matResult_size*sizeof(int), 0666);
    // check for errors
	if (matResult_shmid < 0){
		printf("shmget error: matResult_shmid\n");
		exit(1);
	}
	
	// 2b.
	//attach the shared memories to this process
	shared_matA = shmat(matA_shmid, NULL, 0);
    shared_matB = shmat(matB_shmid, NULL, 0);
    shared_matResult = shmat(matResult_shmid, NULL, 0);
    
	//check for errors
	if(shared_matA == (int *) -1 || shared_matB == (int *) -1 || shared_matResult == (int *) -1) {
		printf("shmat error\n");
		exit(1);
	}

	puts("Done.");
	usleep(500000); // 0.5 seconds
	// ===============================================================================================================================
	// actual code for program's logic
	// ===============================================================================================================================
	// check if the two matrices can be multiplied or not
	// if they can't be multiplied, then prompt an error message and terminate the program
	if (matA_nCol != matB_nRow) {
		printf("\n=====================================");
		printf("\nERROR: incompatible matrix sizes\n");
		printf("Cannot perform MULTIPLICATION...\n");
		printf("=====================================");
		usleep(500000); // 0.5 seconds
		printf("\n\nTerminating the program...\n");
		printf("Bye...\n\n");
		usleep(500000); // 0.5 seconds
		exit(1);
	}
	
	// ===============================================================================================================================
	// // for checking
	// printf("\nShared Memory (Matrix A)\n");
	// for (int row=0; row<matA_nRow; row++){
	// 	for (int col=0; col<matA_nCol; col++){
	// 		printf("[%d%d]:\t%d\t", row, col, shared_matA[row*matA_nCol+col]);
    //         if (col==matA_nCol-1) printf("\n");
	// 	}
	// }

	// // for checking
	// printf("\nShared Memory (Matrix B)\n");
	// for (int row=0; row<matB_nRow; row++){
	// 	for (int col=0; col<matB_nCol; col++){
	// 		printf("[%d%d]:\t%d\t", row, col, shared_matB[row*matB_nCol+col]);
    //         if (col==matB_nCol-1) printf("\n");
	// 	}
	// }

	// // for checking
	// printf("\nShared Memory (Resulting Matrix)\n");
	// for (int row=0; row<matResult_nRow; row++){
	// 	for (int col=0; col<matResult_nCol; col++){
	// 		printf("[%d%d]:\t%d\t", row, col, shared_matResult[row*matB_nCol+col]);
    //         if (col==matB_nCol-1) printf("\n");
	// 	}
	// }
	// =====================================================================================================================
	// RESULTING MATRIX
	// =====================================================================================================================
	// determine how many columns of the resulting matrix should be filled in by process b
	// lab07_surname_B.c should compute the right half of the resulting matrix
	// lab07_surname_B.c should start filling in values on the next column after the last column lab07_surname_A.c has filled in
	// and should stop filling in values, of course, at the last column of the resulting matrix
	int range_a;
	if (matResult_nCol%2 == 0) {
		range_a = matResult_nCol/2;
	} else {
		range_a = matResult_nCol/2 + 1;
	}
	
	int range_b_start, range_b_end;
	range_b_start = range_a;
	range_b_end = matResult_nCol;

	// performing multiplication on the two matrices (right half)
	puts("\nPerforming Multiplication on two Matrices (A & B) - right half.");
	printf("Last %d column/s.\n", range_b_end-range_b_start);
	usleep(500000); // 0.5 seconds
	int sumOfProducts;
	for (int row = 0; row<matResult_nRow; row++) {
		// start at range_b_start and ends with range_b_end or matResult_nCol
		for (int col=range_b_start; col<range_b_end; col++) {
			// reset the sum of products to zero to avoid inaccuracy in the computation
			sumOfProducts = 0;
			
			// access the contents of each row in matrix A and each column in matrix B
			for (int index = 0; index < matA_nCol; index++) {
				// multiply the contents sharing the same index (index in current row of mat A and index in current column of Mat B)
				// note that matA_nCol == matB_nRow (number of columns of matrix a is equal to the number of rows in matrix b)
				sumOfProducts += shared_matA[row*matA_nCol+index] * shared_matB[index*matB_nCol+col];
			}

			// update contents of the shared memory for the resulting matrix
			shared_matResult[row*matResult_nCol+col] = sumOfProducts;
		}
	}

	puts("Done.");
	usleep(500000); // 0.5 seconds
	puts("Displaying the results...");
	usleep(500000); // 0.5 seconds
	// ==========================================================================================================================
	// Display the results
	printf("\nShared Memory (Resulting Matrix)\n");
	for (int row=0; row<matResult_nRow; row++){
		for (int col=0; col<matResult_nCol; col++){
			printf("[%d,%d]:\t%d\t", row+1, col+1, shared_matResult[row*matB_nCol+col]);
            if (col==matB_nCol-1) printf("\n");
		}
	}
	usleep(500000); // 0.5 seconds
	// ==========================================================================================================================
	// prompt a message before cleaning up shared mem
    puts("\nPlease [Enter] to clean up shared mem.");
    getchar();

	// use sleep (before clearing)
	puts("=========================================");
	puts("Please wait while we clean up shared mem.");
	puts("=========================================");
    sleep(3);

    // step 3: dettach
    shmdt(shared_matA);
    shmdt(shared_matB);
    shmdt(shared_matResult);
    shmdt(shared_matSize);

    // step 4: un-allocate,			        
    shmctl(matA_shmid, IPC_RMID, NULL);
    shmctl(matB_shmid, IPC_RMID, NULL);	
    shmctl(matResult_shmid, IPC_RMID, NULL);
    shmctl(matSize_shmid, IPC_RMID, NULL);	 
   
	// display an exit message
    puts("\nCleanup Done.");
	usleep(500000); // 0.5 seconds
	puts("Terminating the program...");
	usleep(500000); // 0.5 seconds
	puts("Bye...\n");
	usleep(500000); // 0.5 seconds
}
