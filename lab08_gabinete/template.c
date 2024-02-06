#include<stdio.h>
#include<stdlib.h> //for malloc
#include<pthread.h> //for threads

typedef struct ARG{
	int Arow;
	int Bcol;
	int ans;
}args;

int **A;
int **B;
int numRowA, numRowB;
int numColA, numColB;

int main(){
	args *arguments; //dynamic number of arguments since the number of threads is unknown;
		//pwede nyo tong gawing 2D array, mahihirapan lang kayo mag-loop

	FILE *fp;

	fp = fopen("matrix.in", "r");
	if(fp!=NULL){
		//read file here
		//you can use fscanf for reading the first and second lines
		//check if the size is invalid, i.e. colA != rowB

		//read the matrix by repeated fscanf to an integer, then getchar until the matrix is filled or the end-of-file is reached

		//print Matrix A

		//print Matrix B

		//create your threads here. Pass to the thread the row of A and the column of B they need to check.

		//join your threads here

		//manage the return values of the threads here

		//print the solution here
	}else{
		printf("File not found!\n");
	}
}