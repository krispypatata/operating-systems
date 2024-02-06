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
sem_t chopstick[N];
// IF YOU DO NOT WANT TO USE SEMAPHORES, YOU CAN USE MUTEXES

int main()
{
	int i;

	// Create 5 PHILOSOPHER
	pthread_t philo[N];
  int *ids = (int *) malloc(sizeof(int) * N);

	// Initialize Semaphores
	for(i=0; i<N; i++){
		sem_init(&chopstick[i], 0, 1);
	}

	// CREATE PHILOSOPHER threads
	for(i=0; i<N; i++){
    ids[i] = i;
		pthread_create(&philo[i], NULL, Philosophize, (void *) &ids[i]);
	}

	// JOIN PHILOSOPHER threads
	for(i=0; i<N; i++){
		pthread_join(philo[i], NULL);
	}

	return 0;
}

void * Philosophize(void * id){
	int ID = *((int *) id);

	while(1){

		// 1. THINK for a while


		// 2. PICK-UP left chopstick


		// 3. PICK-UP right chopstick


		// 4. EAT for a while


		// 5. RELEASE left chopstick


		// 6. RELEASE right choptick

	}

}
