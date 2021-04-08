#include <pthread.h>
#include <stdio.h>
#include "p3160245-p3160156-p3160129-prodcons.h"
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#define ETIMEDOUT 110

//decleare funcs
void *consumer(void *conId);
void *producer(void *prodId);
int check_prod_threads();
struct timespec ts;
struct timeval  tp;

//store product thread arguments
typedef struct prod_arguments {
	int prod_id;
	int rand_nums;
	int seed;
	int producerThreadFinished;
} PROD_ARGUMENTS;

PROD_ARGUMENTS *prod_Args;

//Pointer to buffer
circular_buffer *cb;


//program input
int max_producer_threads;
int max_consumer_threads;
int max_buffer_capacity;
int max_rand_nums;
int rand_nums_seed;

//decleare mutex
pthread_mutex_t lock;
pthread_cond_t cond_buffer_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_buffer_full = PTHREAD_COND_INITIALIZER;

pthread_t *	cons_thread;
pthread_t *	prod_thread;

int ** producers;
int ** consumers;

//main func
int main(int argc, char *argv[]) {

	if (argc != 6) {
		printf("ERROR: the program should take 5 arguments,\n 1: the number of producer threads to create,\n 2: the number of cunsumer threads to create\n 3: buffer size, 4: #rand numbers, 5: random numbers seed\n");
		exit(-1);
	}

	//program input
	max_producer_threads = atoi(argv[1]);
	max_consumer_threads = atoi(argv[2]);
	max_buffer_capacity = atoi(argv[3]);
	max_rand_nums = atoi(argv[4]);
	rand_nums_seed = atoi(argv[5]);


	//Create Threads
	cons_thread = malloc(max_consumer_threads * sizeof(pthread_t));
	prod_thread = malloc(max_producer_threads * sizeof(pthread_t));

	//Pointer to buffer	
	cb = (circular_buffer*)malloc(sizeof(struct circular_buffer));
	//initialize circular buffer
	cb_init(cb, max_buffer_capacity, sizeof(char));

	if (cons_thread == NULL || prod_thread == NULL) {
		printf("NOT ENOUGH MEMORY!\n");
		return -1;
	}
	// mutex initialisation
	pthread_mutex_init(&lock, NULL);

	//thread id
	int *producer_id = malloc(sizeof(int) * max_producer_threads);
	int *consumer_id = malloc(sizeof(int) * max_consumer_threads);

	//prod_Args = malloc(sizeof(*prod_Args) * max_producer_threads);
	prod_Args = malloc(sizeof(PROD_ARGUMENTS) * max_producer_threads);
    
	//store produsers output
	int j;
	producers = malloc(max_producer_threads * sizeof(int *));
	if (max_producer_threads == 0)
		return -1;
	for (j = 0; j < max_producer_threads; j++) {
		producers[j] = malloc(max_rand_nums * sizeof(int));
		if (producers[j] == NULL)
			return -1;
	}

	//store consumers output 
	consumers = malloc(max_consumer_threads * sizeof(int *));
	if (max_producer_threads == 0)
		return -1;
	for (j = 0; j < max_consumer_threads; j++) {
		consumers[j] = malloc((max_rand_nums*max_producer_threads) * sizeof(int));
		if (consumers[j] == NULL)
			return -1;
	}
	
	open_file("prod_in.txt");
	open_file("cons_out.txt");
	
	//Create producer threads
	int rc;
	int i;

	//create producer thread
	for (i = 0; i < max_producer_threads; i++) {
		producer_id[i] = i + 1;

		prod_Args[i].rand_nums = max_rand_nums;
		prod_Args[i].seed = rand_nums_seed;
		prod_Args[i].prod_id = producer_id[i];
		prod_Args[i].producerThreadFinished = 0;

		rc = pthread_create(&prod_thread[i], NULL, producer, &prod_Args[i]);

		/*elegxos oti to thread dimiourgithike swsta.*/
		if (rc != 0) {
			printf("ERROR: return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	//Creat consumer threads
	for (i = 0; i < max_consumer_threads; i++) {
		consumer_id[i] = i + 1;

		rc = pthread_create(&cons_thread[i], NULL, consumer, &consumer_id[i]);

		/*elegxos oti to thread dimiourgithike swsta.*/
		if (rc != 0) {
			printf("ERROR: return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}


	//main thread waits for producer thread
	for (i = 0; i < max_consumer_threads; i++) {
		rc = pthread_join(cons_thread[i], NULL);

		if (rc != 0) {
			printf("ERROR: return code from pthread_join() is %d\n", rc);
			exit(-1);
		}
	}

	
	 //main thread waits for consumer thread
	for (i = 0; i < max_producer_threads; i++) {
		rc = pthread_join(prod_thread[i], NULL);

		if (rc != 0) {
			printf("ERROR: return code from pthread_join() is %d\n", rc);
			exit(-1);
		}		
	}

	// destroy mutex
	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&cond_buffer_full);
	pthread_cond_destroy(&cond_buffer_empty);
    
	//print producers/consumers to terminal
	print_2d_array(producers, max_producer_threads, max_rand_nums, "Producer ");
	print_2d_array(consumers, max_consumer_threads, (max_rand_nums * max_producer_threads), "Consumer ");
	
	//free memory
	free(prod_Args);
	free(producer_id);
    free(consumer_id);
	free(cons_thread);
	free(prod_thread);
	cb_free(cb);

}

//checks if prod threads have finished 
int check_prod_threads() {
	int i, status = 1;
	for (i = 0; i < max_producer_threads; i++) {
		if (prod_Args[i].producerThreadFinished == 0)
			status = 0;
	}
	return status;
}

//consumer thread func
void *consumer(void *conId) {
	int *tid = (int *)conId;
	int j = 0;
	int rt, i;
	char *tmp_read;

	while (1) {
		//initialize timedwait
		gettimeofday(&tp, NULL);
		ts.tv_sec = tp.tv_sec;
		ts.tv_nsec = tp.tv_usec * 1000;
		ts.tv_sec = 4;

		pthread_mutex_lock(&lock);
		
		//check consumer thread conditions to exit
		if (check_prod_threads() && isEmpty(cb)) {
			break;
		}else {
			while (isEmpty(cb)) {
				//apodesmeush tou mutex gia to 2o thread oso to buffer einai empty		
				rt = pthread_cond_timedwait(&cond_buffer_empty, &lock, &ts);
				//check contions for consumer threads to exit
				if ((check_prod_threads() == 1) && (rt == ETIMEDOUT) && (isEmpty(cb) == 1)) {
					pthread_mutex_unlock(&lock);
					pthread_exit(tid);
				}
			}
		}
		
		 // δέσμευση χώρου στην μνήμη για char αριθμό		
		tmp_read = (char*)malloc(1);
		//remove item from buffer
		cb_pop_front(cb, (void*)tmp_read);
         	//write to txt file
		to_txt_out(*tid, *tmp_read, "cons_out.txt", "Consumer");
		
		//insert to 2d array 
		consumers[*tid - 1][j] = *tmp_read;
		j++;
		
		//send signal to producer thread
		pthread_cond_broadcast(&cond_buffer_full);
		pthread_mutex_unlock(&lock);
		//check consumer thread conditions to exit
		if (check_prod_threads() && isEmpty(cb)) {
			break;
		}

	}//end while
	free(tmp_read);

	//sleep(2);
	pthread_exit(tid);
}

void *producer(void *args) {
	int i;
	int j = 0;

	PROD_ARGUMENTS *prod_Args = (PROD_ARGUMENTS *)args;

	srand(time(NULL));
	//rand number seed
	unsigned int* seedp;
	int num = prod_Args->prod_id * prod_Args->seed;
	seedp = &num;

	
	//producer thread work load
	for (i = 0; i < prod_Args->rand_nums; i++) {
		
		pthread_mutex_lock(&lock);
		//producer thread work load
		while (isFull(cb)) {
			//apodesmeush tou mutex gia to 2o thread oso to buffer einai empty
			pthread_cond_wait(&cond_buffer_full, &lock);
		}

		// διάβασμα τυχαίου αριθμού [0,256)
		char input = rand_r(seedp) % 256;
		
		//insert item to buffer
		if (!isFull(cb)) {
			cb_push_back(cb, (void*)&input);
			//write to txt
			to_txt_out(prod_Args->prod_id, input, "prod_in.txt", "Producer");						
			//store to array
			producers[prod_Args->prod_id - 1][j] = (int)input;
			j++;			
		}
		//send signal to consumer thread 
		pthread_cond_broadcast(&cond_buffer_empty);
		//unlock the code
		pthread_mutex_unlock(&lock);

	}
	// producer thread finished
	prod_Args->producerThreadFinished = 1;
	pthread_exit(NULL);
}






















































