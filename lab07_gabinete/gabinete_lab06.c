/**
	Author: Gabinete, Keith Ginoel S.
	Section: CMSC 125 - CD-7L
	Program Description: 
		This is a simple program that simulates virtual memory management by retrieving the corresponding value stored on a given virtual address.
 */

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

//paging parameters
#define PAGE_SIZE    0x0100     //256 bytes per page
#define FRAME_SIZE   PAGE_SIZE
#define FRAME_COUNT  0x0100     //256 frames

//for extracting page number and offset
#define VPN_MASK     0xFF00   //or 0x0300 
#define VPN_SHIFT    0x0008
#define OFFSET_MASK  0x00FF


// TLB data structure
typedef struct TLBnode{
	int pageNum;
	int frameNumber;
	struct TLBnode *next;
}TLB;

TLB *tlb_head = NULL;


// for reading virtual addresses from an input file and storing them into an array
void readVirtualAddresses(FILE *fp, int *vaCount, int *virtualAddressSpace) {
	// variables for file reading
	char fgets_vaCount[128];
	char fgets_va[128];

	// get the address count
	fgets(fgets_vaCount, sizeof(fgets_vaCount), fp);
	sscanf(fgets_vaCount, " %d ", vaCount);			// store it in the num_of_addresses int variable

	// read the each line from the input (addresses.txt) file
	// each line contains the virtual/logical addresses that we'll need to translate later on
	// store the fetched data into arrays
	for (int i=0; i<*vaCount; i++) {
		fgets(fgets_va, sizeof(fgets_va), fp);
		sscanf(fgets_va, " %d", &virtualAddressSpace[i]); // store in the array
	}
}


// for translating a virtual address to a physical address and getting the value of the address from the physical memory
void performAddressTranslation(int *physicalAddress, int *value, char physicalMemory[256][256], int pfn, int offset, FILE *fpp) {
	// perform address translation
	// 3i. translate to physical address
	*physicalAddress = pfn * FRAME_SIZE + offset;

	// 3ii. get value from physical memory
	*value = physicalMemory[pfn][offset];

	// ===========================================================================================================
	// [OUTPUT]
	fprintf(fpp, "Physical address: %d, ", *physicalAddress);
	fprintf(fpp, "Value: %d", *value);
	// ===========================================================================================================
}


// for updating the tlb
void updateTLB (TLB **temp, TLB **tlb_head, int *tlbSize, int vpn, int pfn, FILE *fpp) {
	// ===========================================================================================================
	// [OUTPUT]
	fprintf(fpp, "::UPDATING TLB::");
	// ===========================================================================================================

	// 1. update tlb with vpn and pfn
	// if the linked list for tlb is still empty
	if (*tlb_head==NULL) {
		// initialize tlb
		(*tlb_head) = (TLB *) malloc(sizeof(TLB));
		(*tlb_head)->pageNum = vpn;
		(*tlb_head)->frameNumber = pfn;
		(*tlb_head)->next = NULL;

		(*tlbSize)++; 		// update size of tlb
	} 
	// if tlb is not empty
	else {
		(*temp) = (*tlb_head);		// point at head of the tlb linked list

		// find the tail of the linked list (for new node insertion)
		while ((*temp)->next!=NULL) {
			(*temp) = (*temp)->next;
		}

		// check if tlb is already at max size
		// if there's still a space available at tlb
		if ((*tlbSize)!=16) {
			// ===========================================================================================================
			// [OUTPUT]
			fprintf(fpp, "TLB HAS SPACE::");
			// ===========================================================================================================

			// create a new node at tail
			(*temp)->next = (TLB *) malloc(sizeof(TLB));
			(*temp) = (*temp)->next;
			(*temp)->pageNum = vpn;
			(*temp)->frameNumber = pfn;
			(*temp)->next = NULL;

			(*tlbSize)++; 		// update size of tlb
		} 
		// if tlb size is at maximum
		else {
			// ===========================================================================================================
			// [OUTPUT]
			fprintf(fpp, "TLB IS FULL, REMOVE THE OLDEST::");
			// ===========================================================================================================

			// create a new node at tail but delete a node at head
			(*temp)->next = (TLB *) malloc(sizeof(TLB));
			(*temp) = (*temp)->next;
			(*temp)->pageNum = vpn;
			(*temp)->frameNumber = pfn;
			(*temp)->next = NULL;

			(*temp) = (*tlb_head);				// temporarily point at head (for freeing)
			(*tlb_head) = (*tlb_head)->next;	// point the head pointer to the next node after the head node (will be the new head node of the tlb)
			(*temp)->next = NULL; 				// cut the connection between the to-be deleted node and the tlb linked list
			free((*temp));						// free memory occupied by temp (delete node at previous head)
		}
	}
}


//initialize an empty page table
//page table size is 256
void initializePageTable(int pageTable[]){
	int i;
	for(i=0;i<256;i+=1){
		pageTable[i] = -1;
	}
}


//main
int main(){
	int next_free_frame=0; 	//Declared here are all the variables
	FILE *fp, *bp, *fpp;				 		//you need. You can add other variables
	char physicalMemory[256][256];		 		//you think are necessary.
	int pageTable[256];

	// ===========================================================================================================
	// defined variables for this exercise
	int TLB_MAX_SIZE = 16;
	int virtualAddressSpace[1000];		// will contain the fetched/read virtual address inputs
	int vaCount = 0;					// total number of virtual addresses stored in addresses.txt
	int tlbSize = 0; 					// will track the size of the tlb
	TLB *temp;							// dynamic pointer

	//placeholder for the VPN, PFN, and offset
	int vpn = 0;
	int pfn = 0;
	int offset = 0;

	int physicalAddress;
	int value;
	// ===========================================================================================================

	fp = fopen("addresses.txt","r"); //addresses to convert
	bp = fopen("BACKING_STORE.bin","rb"); //backing store

	initializePageTable(pageTable);

	if(fp!=NULL && bp!=NULL){
		fpp = fopen("answers.txt","w"); //you will put your answers here

		// 1. Read virtual addresses from addresses.txt
		readVirtualAddresses(fp, &vaCount, virtualAddressSpace);

		// traverse through the contents of the virtual address space
		for (int i=0; i<vaCount; i++) {
			// ===========================================================================================================
			// 2. Extract components (vpn and offset)
			vpn = (virtualAddressSpace[i] & VPN_MASK) >> VPN_SHIFT;
			offset = (virtualAddressSpace[i] & OFFSET_MASK);
			
			// ===========================================================================================================
			// [OUTPUT]
			fprintf(fpp, "Virtual address: %d, ", virtualAddressSpace[i]);
			fprintf(fpp, "PageNum: %d, ", vpn);
			fprintf(fpp, "PageOff: %d, ", offset);
			fprintf(fpp, "TLB ");
			// ===========================================================================================================

			// 3. Search vpn in TLB linked list
			bool tlb_hit = false;
			temp = tlb_head;
			// traverse through the contents of the tlb
			while (temp!=NULL) {
				if (temp->pageNum == vpn) {
					tlb_hit = true;					// set tlb hit flag to true
					pfn = temp->frameNumber; 		// get frame number
				} 
				// move to the next node in the linked list
				temp = temp->next;
			}

			// 3a. if tlb hit
			if (tlb_hit == true) {
				// ===========================================================================================================
				// [OUTPUT]
				fprintf(fpp, "HIT::");
				// ===========================================================================================================

				// perform address translation
				performAddressTranslation(&physicalAddress, &value, physicalMemory, pfn, offset, fpp);
			} 
			// 3b. if tlb miss
			else {
				// ===========================================================================================================
				// [OUTPUT]
				fprintf(fpp, "MISS::");
				// ===========================================================================================================

				// look at the page table using vpn as the index to get pfn
				pfn = pageTable[vpn];

				// check if the vpn is valid or not
				// i. if vpn is valid
				if (pfn > -1) {
					// ===========================================================================================================
					// [OUTPUT]
					fprintf(fpp, "PAGE IN MEM");
					// ===========================================================================================================

					// update tlb with vpn and pfn
					updateTLB(&temp, &tlb_head, &tlbSize, vpn, pfn, fpp);

					// perform address translation
					performAddressTranslation(&physicalAddress, &value, physicalMemory, pfn, offset, fpp);
				}
				// ii. if pfn is invalid
				else {
					// ===========================================================================================================
					// [OUTPUT]
					fprintf(fpp, "PAGE NOT IN MEM, FAULT, LOAD FROM DISK, UPDATE PT");
					// ===========================================================================================================

					// load from BACKING_STORE.bin using fseek() and fread()
					fseek(bp,vpn * PAGE_SIZE, SEEK_SET);
      				fread(&(physicalMemory[next_free_frame][0]),PAGE_SIZE,1,bp);

					// 1. next_free_frame will be your pfn
					pfn = next_free_frame;

					// 2. update page table and TLB with vpn and pfn
					// a. update page table
					pageTable[vpn] = pfn;

					// b. update TLB
					updateTLB(&temp, &tlb_head, &tlbSize, vpn, pfn, fpp);

					// 3. perform address translation
					performAddressTranslation(&physicalAddress, &value, physicalMemory, pfn, offset, fpp);

					// 4. increment next_free_frame
					next_free_frame++;
				}
			}
			// add new line at the end of the output string after each iteration
			fprintf(fpp, "\n");
			
			// for checking
			// printf("Virtual address: %d, PageNum: %d, PageOff: %d, TLB MISS::PAGE NOT IN MEM, FAULT, LOAD FROM DISK, UPDATE PT::UPDATING TLB::Physical address: %d, Value: %d\n", virtualAddressSpace[i], vpn, offset,physicalAddress, value);		
		}

		// clear contents of the tlb linked list avoid memory leaks
		while (tlb_head!=NULL) {
			temp = tlb_head;
			tlb_head = tlb_head->next;
			temp->next = NULL;
			free(temp);
		}
		
		// close the file that is read (addresses.txt)
		fclose(fp);
		fclose(bp);
		fclose(fpp);	
	}
}
