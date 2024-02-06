/**
	Author: Gabinete, Keith Ginoel S.
	Section: CMSC 125 - CD-7L
    Lab Exercise 07 - A
	Program Description: 
		This is a simple program that simulates sharing of memory with another program to compute for the matrix product of two matrices..
 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>	//for new types declared
#include<sys/ipc.h>		//for interprocess communication
#include<sys/shm.h>		//for shared memory
#define MAT_A_KEY 0x64		// 100
#define MAT_B_KEY 0xC8		// 200
#define MAT_RESULT 0x12C	// 300
#define MAT_SIZE 0x190		// 400

// ===============================================================================================================================
void readInputFile(int **main_mat_sizes, int **main_matA, int **main_matB) {
	FILE *fp = fopen("input.txt","r"); // input file

	// variables for file reading
	// for the dimensions of matrix A
	char fgets_matAdim[128];	// storing the dimensions of matrix A
	int matA_size = 0, matA_nRow = 0, matA_nCol = 0;

	// for the dimensions of matrix B
	char fgets_matBdim[128];	// storing the dimensions of matrix B
	int matB_size = 0, matB_nRow = 0, matB_nCol = 0;

	// for reading of matrix contents
	int content = 0; 

	// ===========================================================================================================================
	puts("\nSuccessfully fetched input data.");
	usleep(500000); // 0.5 seconds
	// Creating matrix A
	puts("Creating Matrix A.");
	usleep(500000); // 0.5 seconds
	// ===========================================================================================================================
	// get the dimension of matrix a
	fgets(fgets_matAdim, sizeof(fgets_matAdim), fp);
	sscanf(fgets_matAdim, " %d %d", &matA_nRow, &matA_nCol);			// store it in the num_of_addresses int variable

	// ===========================================================================================================================
	// // for checking
	// printf("\nNumber of rows of Mat A: %d", matA_nRow);
	// printf("\nNumber of columns of Mat A: %d\n", matA_nCol);
	// ===========================================================================================================================
	// create an array for storing the contents of matrix A from input.txt
	matA_size = matA_nRow*matA_nCol;
	int matA[matA_size];

	// get the contents of matrix A
	content = 0;
	for (int i =0; i<matA_size; i++ ) {
		fscanf(fp, " %d\t", &content);
		matA[i] = content;
	}

	// // ===========================================================================================================================
	// // for checking
	// for (int row=0; row<matA_nRow; row++){
	// 	for (int col=0; col<matA_nCol; col++){
	// 		printf("[%d]:\t%d\t", row*matA_nCol+col, matA[row*matA_nCol+col]);
    //         if (col==matA_nCol-1) printf("\n");
	// 	}
	// }
	// ===========================================================================================================================
	// Creating matrix B
	puts("Creating Matrix B.");
	usleep(500000); // 0.5 seconds
	// ===========================================================================================================================
	// get the dimension of matrix B
	fgets(fgets_matBdim, sizeof(fgets_matBdim), fp);
	sscanf(fgets_matBdim, " %d %d", &matB_nRow, &matB_nCol);			// store it in the num_of_addresses int variable
	
	// ===========================================================================================================================
	// // for checking
	// printf("\nNumber of rows of Mat B: %d", matB_nRow);
	// printf("\nNumber of columns of Mat B: %d\n", matB_nCol);
	// ===========================================================================================================================
	// create an array for storing the contents of matrix B from input.txt
	matB_size = matB_nRow*matB_nCol;
	int matB[matB_size];

	// get the contents of matrix B
	content = 0;
	for (int i =0; i<matB_size; i++ ) {
		fscanf(fp, " %d", &content);
		matB[i] = content;
	}

	// ===========================================================================================================================
	// // for checking
	// for (int row=0; row<matB_nRow; row++){
	// 	for (int col=0; col<matB_nCol; col++){
	// 		printf("[%d]:\t%d\t", row*matB_nCol+col, matB[row*matB_nCol+col]);
    //         if (col==matB_nCol-1) printf("\n");
	// 	}
	// }
    // ===========================================================================================================================
	// store the dimensions of the 2 matrices in an array
	int mat_sizes[4];
	mat_sizes[0] = matA_nRow;
	mat_sizes[1] = matB_nRow;
	mat_sizes[2] = matA_nCol;
	mat_sizes[3] = matB_nCol;

	// modifying the values of the passed pointers
	*main_mat_sizes = mat_sizes;
	*main_matA = matA;
	*main_matB = matB;

 	// ===========================================================================================================================
	// close the file
	fclose(fp);
	puts("Done.");
	usleep(500000); // 0.5 seconds
}
// ===============================================================================================================================

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

    // other variables
    // for matrix 
    int *mat_sizes;
	int *matA;
	int *matB;

	// for matrices' dimensions
	int matA_nRow, matA_nCol, matA_size;
	int matB_nRow, matB_nCol, matB_size;
	int matResult_nRow, matResult_nCol, matResult_size;

	// ===============================================================================================================================
	// read input.txt
	puts("\n========================================================================"); 
	puts("Reading input.txt");
	usleep(500000); // 0.5 seconds
    readInputFile(&mat_sizes, &matA, &matB);

	// ===============================================================================================================================
	// for code simplicity/clarity
	matA_nRow = mat_sizes[0];
	matB_nRow = mat_sizes[1];
	matA_nCol = mat_sizes[2];
	matB_nCol = mat_sizes[3];

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
	// (cont.) for code simplicity/clarity 
	matA_size = matA_nRow*matA_nCol;
	matB_size = matB_nRow*matB_nCol;

	matResult_nRow = matA_nRow;
	matResult_nCol = matB_nCol;
	matResult_size = matResult_nRow*matResult_nCol;

	// ===============================================================================================================================
	// STEP 1:
	// Create shared memories using shmget
	matA_shmid = shmget(matA_key, matA_size*sizeof(int), IPC_CREAT | 0666);
	// check for errors
	if (matA_shmid < 0){
		printf("shmget error: matA_shmid\n");
		exit(1);
	}

    matB_shmid = shmget(matB_key, matB_size*sizeof(int), IPC_CREAT | 0666);
	// check for errors
	if (matB_shmid < 0){
		printf("shmget error: matB_shmid\n");
		exit(1);
	}

    matResult_shmid = shmget(matResult_key, matResult_size*sizeof(int), IPC_CREAT | 0666);
	// check for errors
	if (matResult_shmid < 0){
		printf("shmget error: matResult_shmid\n");
		exit(1);
	}

    matSize_shmid = shmget(matSize_key, 4*sizeof(int), IPC_CREAT | 0666);
	// check for errors
	if (matSize_shmid < 0){
		printf("shmget error:matSize_shmid\n");
		exit(1);
	}
    
	// check for errors
	if(matA_shmid < 0 || matB_shmid < 0 || matResult_shmid < 0 || matSize_shmid < 0){
		printf("shmget error\n");
		exit(1);
	}

	// ===============================================================================================================================
	//STEP 2:
	//Attach the shared memory to this process
	shared_matA = shmat(matA_shmid, NULL, 0);
    shared_matB = shmat(matB_shmid, NULL, 0);
    shared_matResult = shmat(matResult_shmid, NULL, 0);
    shared_matSize = shmat(matSize_shmid, NULL, 0);

	//check for errors
	if(shared_matA == (int *) -1 || shared_matB == (int *) -1 || shared_matResult == (int *) -1 || shared_matSize == (int *) -1) {
		printf("shmat error\n");
		exit(1);
	}

	// ===============================================================================================================================
	// actual code for program's logic
	// ===============================================================================================================================
	// copy the contents of mat_sizes to the shared memory shared_matSize
	// can't do assignment statement directly (shared_matSize = mat_sizes) as mat_sizes is just a pointer to an array exclusive to this process only
	for (int i = 0; i<4; i++) {
		shared_matSize[0] = mat_sizes[0];
		shared_matSize[1] = mat_sizes[1];
		shared_matSize[2] = mat_sizes[2];
		shared_matSize[3] = mat_sizes[3];
	}

	// do the same for the contents of matrix A and matrix B
	for (int i=0; i<matA_size; i++) {
		shared_matA[i] = matA[i];
	}

	for (int i=0; i<matB_size; i++) {
		shared_matB[i] = matB[i];
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
	// ===============================================================================================================================
	// RESULTING MATRIX
	// a. initialize the contents of the shared memory for the resulting matrix to zero
	for (int row=0; row<matResult_nRow; row++){
		for (int col=0; col<matResult_nCol; col++){
			shared_matResult[row*matB_nCol+col] = 0;
		}
	}

	// b. determine how many columns of the resulting matrix should be filled in by process A
	// lab07_surname_A.c should compute the left half of the resulting matrix
	// If the number of columns for the resulting matrix is odd, lab07_gabinete_A.c shall compute for the values in the middle column. 
	int range_a;
	if (matResult_nCol%2 == 0) {
		range_a = matResult_nCol/2;
	} else {
		range_a = matResult_nCol/2 + 1;
	}

	// c. performing multiplication on the two matrices
	puts("\nPerforming Multiplication on two Matrices (A & B) - left half.");
	printf("First %d column/s only.\n", range_a);
	usleep(500000); // 0.5 seconds
	int sumOfProducts;
	for (int row = 0; row<matResult_nRow; row++) {
		for (int col=0; col<range_a; col++) {
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
	// ===============================================================================================================================
	// // for checking
	// printf("\nShared Memory (Resulting Matrix)\n");
	// for (int row=0; row<matResult_nRow; row++){
	// 	for (int col=0; col<matResult_nCol; col++){
	// 		printf("[%d%d]:\t%d\t", row, col, shared_matResult[row*matB_nCol+col]);
    //         if (col==matB_nCol-1) printf("\n");
	// 	}
	// }
	// ===============================================================================================================================
	// remind the user to run the part B of the program before continuing
    puts("\nRun lab07_gabinete_B now.");
    puts("Press [Enter] key after running lab07_gabinete_B to clean up shared mem.");
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
