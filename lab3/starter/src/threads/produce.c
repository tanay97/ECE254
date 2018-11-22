// Use this to see if a number has an integer square root
#define EPS 1.E-7


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>

double g_time[2];

int num;
int maxmsg;
int num_p;
int num_c;
int* buffer;

int p_count;
int c_count;

sem_t producer_space;
sem_t consumer_ready;

pthread_mutex_t mutex;	

void* producer( void *param );
void* consumer( void *param );

int main(int argc, char *argv[])
{
	int i;
	struct timeval tv;

	if (argc != 5) {
		printf("Usage: %s <N> <B> <P> <C>\n", argv[0]);
		exit(1);
	}

	num = atoi(argv[1]);	/* number of items to produce */
	maxmsg = atoi(argv[2]); /* buffer size                */
	num_p = atoi(argv[3]);  /* number of producers        */
	num_c = atoi(argv[4]);  /* number of consumers        */

	gettimeofday(&tv, NULL);
	g_time[0] = (tv.tv_sec) + tv.tv_usec/1000000.;

	// Initializing semaphores
	sem_init( &producer_space, 0, maxmsg );
	sem_init( &consumer_ready, 0, 0 );
	pthread_mutex_init(&mutex, NULL);

	// Initializing arrays for buffers, producers, consumers
	buffer = malloc(maxmsg * sizeof(int));
	pthread_t pid[num_p];
	pthread_t cid[num_c];
	
	// Creating producers and consumers
	for (i = 0; i < num_p; i++) {
		int* id = malloc(sizeof(int));
		*id = i;
		pthread_create(&pid[i], NULL, producer, id);
	}
	for (i = 0; i < num_c; i++) {
		int * id = malloc(sizeof(int));
		*id = i;
		pthread_create(&cid[i], NULL, consumer, id);
	}

	// Joining producers and consumers
	for (i = 0; i < num_p; i++) {
		pthread_join( pid[i], NULL );
	}
        for (i = 0; i < num_c; i++) {
                pthread_join( cid[i], NULL );
        }

	// Cleaning up
	sem_destroy(&producer_space);
	sem_destroy(&consumer_ready);
	pthread_mutex_destroy(&mutex);
	free(buffer);
	
    gettimeofday(&tv, NULL);
    g_time[1] = (tv.tv_sec) + tv.tv_usec/1000000.;

    printf("System execution time: %.6lf seconds\n", \
            g_time[1] - g_time[0]);
	exit(0);
}

void* producer( void *param ) {
	int* id = (int*) param;
	
	for (int i = 0; i < num; i++) {
		if ((i % num_p) == *id) {
			sem_wait(&producer_space);			
			pthread_mutex_lock(&mutex);
			
			// Critical Section
			buffer[p_count % maxmsg] = i;
			p_count++;
			
			pthread_mutex_unlock(&mutex);
			sem_post(&consumer_ready);
		}
	}
	free(id);
	pthread_exit(0);	
}

void* consumer( void *param ) {
        int* id = (int*) param;
	
	int res;
	int root;
        while (1) {
		sem_wait(&consumer_ready);                	
		pthread_mutex_lock(&mutex);
			
		// N items have already been read break out of loop
		if (c_count >= num-1) {
			pthread_mutex_unlock(&mutex);
			sem_post(&consumer_ready);
			break;
		}
			
		res = buffer[c_count % maxmsg];
		c_count++;
       	
		pthread_mutex_unlock(&mutex);
		sem_post(&producer_space);			

		root = sqrt(res);
		if (root * root == res) {
			printf("%d %d %d\n", *id, res, root);
        	}
	}

	free(id);
	pthread_exit(0);
}

