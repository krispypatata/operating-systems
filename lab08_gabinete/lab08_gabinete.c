/**
	Author: Gabinete, Keith Ginoel S.
	Section: CMSC 125 - CD-7L
    Lab Exercise 08
	Program Description: 
		This is a simple program that demonstrates Matrix Multiplication (with Multi-threading).
 */

#include<stdio.h>
#include<stdlib.h> //for malloc
#include<pthread.h> //for threads

int **A;
int **B;

int numRowA, numRowB;
int numColA, numColB;
int numOperations;


typedef struct ARG{
	int Arow;
	int Bcol;
	int ans;
}args;


// function for the thread that takes two or more parameters
// performs multiplication on a row in Matrix A and a column in Matrix B
void * multiplyRowAColB(void *arguments){
	args * temp;

    // typecast back to args data type
	temp = (args *) arguments;

    int * result = (int *) malloc(sizeof(int));	// this is where we will store the return value

    // multiply each element in row A to its corresponding element in column B
    for (int i = 0; i < numColA; i++) {
        temp->ans += A[temp->Arow][i] * B[i][temp->Bcol];
    }

    *result = temp->ans;

    // printf("\nresult: %d", *result);
    // printf("\n");

    // return the final sum of the products
	pthread_exit(result);
}


int main(){
	args *arguments;    // dynamic number of arguments since the number of threads is unknown;
    pthread_t *tid;     // will hold the threads that are gonna be created
    int **C;            // is the resulting matrix from matrix multiplication
    int numRowC, numColC;

	FILE *fp;

	fp = fopen("matrix.in", "r");
	// fp = fopen("matrixSample.in", "r");
	if(fp!=NULL){
        // ========================================================================================================================

		/*
                read file here
        */

        // ========================================================================================================================
        // Extracting info about the task
        // extracting the the number of operations to be performed
        char fgets_numOperations[128];	// for storing the number of operations to perform

        fgets(fgets_numOperations, sizeof(fgets_numOperations), fp);
	    sscanf(fgets_numOperations, " %d ", &numOperations);			// store it in the number_of_operations variable

        // ------------------------------------------------------------------------------------------------------------------------
        printf("\n=============================================================");
        printf("\nNumber of Operations: %d\n", numOperations);
        printf("=============================================================\n");
        // ------------------------------------------------------------------------------------------------------------------------
        
        for (int nOperation=0; nOperation<numOperations; nOperation++) {            
            printf("\nMatrix Multiplication #%d\n", nOperation+1);
            

            // ========================================================================================================================
            // dimensions of matrix A
            char fgets_dim_matrixA[128];

            fgets(fgets_dim_matrixA, sizeof(fgets_dim_matrixA), fp);
            sscanf(fgets_dim_matrixA, " %d %d ", &numRowA, &numColA);			// store it in the num_of_addresses int variable 


            // ------------------------------------------------------------------------------------------------------------------------
            // for checking
            // printf("\nNumber of rows of Mat A: %d", numRowA);
            // printf("\nNumber of columns of Mat A: %d\n", numColA);
            // ------------------------------------------------------------------------------------------------------------------------
            
            // create a 2d array for storing the elements of matrix A

            // for the rows
            A = (int **) malloc( numRowA * sizeof(int *) );

            // for the columns on each row
            for (int i = 0; i < numRowA; i++) {
                A[i] = (int *) malloc( numColA * sizeof(int) );
            }
            
            // ========================================================================================================================

            // read the elements of matrix A from the input file and store them properly in the matrix
            int matElement = 0;

            for (int i = 0; i < numRowA; i++) {
                for (int j = 0; j < numColA; j++) {
                    fscanf(fp, " %d\t", &matElement);
                    A[i][j] = matElement;
                }
            }

            // ========================================================================================================================
            // dimensions of matrix B
            char fgets_dim_matrixB[128];

            fgets(fgets_dim_matrixB, sizeof(fgets_dim_matrixB), fp);
            sscanf(fgets_dim_matrixB, " %d %d ", &numRowB, &numColB);			// store it in the num_of_addresses int variable 


            // ------------------------------------------------------------------------------------------------------------------------
            // for checking
            // printf("\nNumber of rows of Mat B: %d", numRowB);
            // printf("\nNumber of columns of Mat B: %d\n", numColB);
            // ------------------------------------------------------------------------------------------------------------------------
            
            // create a 2d array for storing the elements of matrix B

            // for the rows
            B = (int **) malloc( numRowB * sizeof(int *) );

            // for the columns on each row
            for (int i = 0; i < numRowB; i++) {
                B[i] = (int *) malloc(numColB * sizeof(int) );
            }
            
            // ========================================================================================================================
            
            // read the elements of matrix B from the input file and store them properly in the matrix
            matElement = 0;

            for (int i = 0; i < numRowB; i++) {
                for (int j = 0; j < numColB; j++) {
                    fscanf(fp, " %d\t", &matElement);
                    B[i][j] = matElement;
                }
            }

            // ========================================================================================================================

            /*
                    check if the size is invalid, i.e. colA != rowB            
            */

            // ========================================================================================================================

            if (numColA != numRowB) {
                // deallocate memories for matrix A and matrix B
                for (int i = 0; i < numRowA; i++) {
                    free(A[i]);
                }
                free(A);

                // deallocate memory for matrix B
                for (int i = 0; i < numRowB; i++) {
                    free(B[i]);
                }
                free(B);

                if (nOperation == numOperations-1) {
                    printf("\n=============================================================");
                    printf("\nERROR: incompatible matrix sizes\n");
                    printf("Cannot perform MULTIPLICATION...\n");
                    printf("=============================================================");
                    printf("\n\nTerminating the program...\n");
                    printf("Bye...\n\n");

                    return 0;
                } else {
                    printf("\n=============================================================");
                    printf("\nERROR: incompatible matrix sizes\n");
                    printf("Cannot perform MULTIPLICATION...\n");
                    printf("\nWill Proceed to the next operation...\n");
                    printf("=============================================================\n");
                    continue;
                }
            }

            // ========================================================================================================================

            /*
                    print Matrix A           
            */

            // ========================================================================================================================

            printf("\nMatrix A:\n");
            for (int i = 0; i < numRowA; i++) {
                for (int j = 0; j < numColA; j++) {
                    printf("[%d,%d]:\t%d\t", i+1, j+1, A[i][j]);
                }
                printf("\n");
            }
            
            // ========================================================================================================================

            /*
                    print Matrix B           
            */
            
            // ========================================================================================================================

            printf("\nMatrix B:\n");
            for (int i = 0; i < numRowB; i++) {
                for (int j = 0; j < numColB; j++) {
                    printf("[%d,%d]:\t%d\t", i+1, j+1, B[i][j]);
                }
                printf("\n");
            }

            // ========================================================================================================================

            /*
                    create your threads here. Pass to the thread the row of A and the column of B they need to check.
            */

            // ========================================================================================================================
            numRowC = numRowA;
            numColC = numColB;

            // create a 2d array for the resulting matrix of the matrix multiplication (A x B)

            // for the rows
            C = (int **) malloc( numRowC * sizeof(int *) );

            // for the columns on each row
            for (int i = 0; i < numRowC; i++) {
                C[i] = (int *) malloc( numColC * sizeof(int) );
            }

            // ========================================================================================================================

            // create an array for the m x n threads needed for the matrix multiplication
            tid = (pthread_t *) malloc( numRowC*numColC * sizeof(pthread_t) );

            // ========================================================================================================================

            // perform matrix multiplication
            for (int i = 0; i < numRowC; i++) {
                for (int j = 0; j < numColC; j++) {
                    // allocate memory for the arguments needed in a function call
                    arguments = (args *) malloc(sizeof(args));

                    // initialize the members of the arguments structure
                    arguments->Arow = i;
                    arguments->Bcol = j;
                    arguments->ans = 0;  // initialize to zero (Since we are just about to solve the product in this multiplication)

                    // create a thread for multiplying a certain row of matrix A and a certain column of matrix B
                    pthread_create(&tid[ i*numColC + j ], NULL, multiplyRowAColB, (void *) arguments);
                }
            }

            // ========================================================================================================================

            /*
                    join your threads here
            */

            // ========================================================================================================================
            for (int i=0; i<numRowC*numColC; i++) {
                pthread_join(tid[i], (void  **) &C[i] );
            }
            // ========================================================================================================================

            /*
                    manage the return values of the threads here
                    print the solution here
            */

            // ========================================================================================================================
            //accessing the return values
            printf("\nResulting Matrix C (After Matrix Multiplication A x B):\n");
            for(int i=0; i<numRowC*numColC; i++){
                // for proper indexing (in printing)
                int row, col;
                row = i / numColC;
                col = i % numColC;

                // printing the returned value stored in C
                printf("[%d,%d]:\t%d\t", row+1, col+1, *((int *)C[i]) );

                // new line at the end of each row
                if ( (i+1) % (numColC) == 0 ) printf("\n");
            }

            // ========================================================================================================================

            /*
                    matrix multiplication done...
                    now, cleaning up
            */

            // ========================================================================================================================
            // deallocate memory for matrix A
            for (int i = 0; i < numRowA; i++) {
                free(A[i]);
            }
            free(A);

            // deallocate memory for matrix B
            for (int i = 0; i < numRowB; i++) {
                free(B[i]);
            }
            free(B);

            // deallocate memory for matrix C
            for (int i = 0; i < numRowC; i++) {
                free(C[i]);
            }
            free(C);

            // deallocate memory for threads
            free(tid);

            printf("\n=============================================================\n");
        }

	}else{
		printf("File not found!\n");
	}
}
