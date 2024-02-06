/**
	Author
	Description

 */

#include<stdio.h>

/*
// TLB data structure
typedef struct TLBnode{
	int pageNum;
	int frameNumber;
	struct TLBnode *next;
}TLB;

TLB *tlb_head = NULL;
*/

//initialize an empty page table
//page table is always size 256
void initializePageTable(int pageTable[]){
	int i;
	for(i=0;i<256;i+=1){
		pageTable[i] = -1;
	}
}

//main
int main(){
	int num,i,temp,pnum,poff,val,next_free_frame=0; //Declared here are all the variables
	FILE *fp, *bp, *fpp;				 //you need. You can add other variables
	char physicalMemory[256][256];		 //you think are necessary.
	int pageTable[256];

	fp = fopen("addresses.txt","r"); //addresses to convert
	bp = fopen("BACKING_STORE.bin","rb"); //backing store

	initializePageTable(pageTable);

	if(fp!=NULL && bp!=NULL){
		fpp = fopen("answers.txt","w"); //you will put your answers here



		fclose(fp);
		fclose(bp);
		fclose(fpp);
	}


}
