/*
	Author: Gabinete, Keith Ginoel S.
	Section: CMSC 125 - CD-7L
    Lab Exercise 09
	Program Description: 
		This is a simple program that demonstrates the use of mutex (lock) in multi-threading.
*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>		// time()
#include <math.h>		// srand() and ran()
#include <pthread.h>	// PTHREADS
#include <semaphore.h>	// SEMAPHORES
#include <unistd.h>
#define N 5

// This method will be performed by each philosopher
void * Philosophize(void * id);

// Chopsticks: Shared resources
sem_t chopstick[N]; 		// array of semaphores
// IF YOU DO NOT WANT TO USE SEMAPHORES, YOU CAN USE MUTEXES

int main()
{
	int i;

	// Create 5 PHILOSOPHER
	pthread_t philo[N];
  	int *ids = (int *) malloc(sizeof(int) * N);

	// Initialize Semaphores
	for(i=0; i<N; i++){
		sem_init(&chopstick[i], 0, 1); // isa lang yung pwede humawak ng chopstick
	}

	// CREATE PHILOSOPHER threads
	for(i=0; i<N; i++){
    ids[i] = i;
		pthread_create(&philo[i], NULL, Philosophize, (void *) &ids[i]); // (0,1,2,3,4)
	}

	// JOIN PHILOSOPHER threads
	for(i=0; i<N; i++){
		pthread_join(philo[i], NULL);
	}

	return 0;
}

void * Philosophize(void * id){
	int ID = *((int *) id);

	int randomNum;

	int leftChopstick;
	int rightChopstick;

	while(1){

		// 1. THINK for a while
		randomNum = rand()%5;     // 0 - 4
		printf("Philosopher %d started thinking.\nThinking for %d seconds...\n", ID, randomNum);
  		sleep(randomNum);

		// 2. PICK-UP left chopstick
		if (ID >0) leftChopstick = ID-1;
		else leftChopstick = 4;

		sem_wait(&(chopstick[leftChopstick]));

		// 3. PICK-UP right chopstick
		rightChopstick = ID;
		
		sem_wait(&(chopstick[rightChopstick]));

		// 4. EAT for a while
		randomNum = rand()%5;
		printf("Philosopher %d started eating. Eating for %d seconds...\n", ID, randomNum);
		sleep(randomNum);
		printf("Philosopher %d stopped eating.\n", ID);

		// 5. RELEASE left chopstick
		sem_post(&(chopstick[leftChopstick])); // end of the critical region

		// 6. RELEASE right choptick
		sem_post(&(chopstick[rightChopstick])); // end of the critical region

	}

}
