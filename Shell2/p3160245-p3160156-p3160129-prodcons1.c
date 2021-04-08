#include <pthread.h>
#include <stdio.h>
#include "p3160245-p3160156-p3160129-prodcons.h"

//decleare funcs
void *consumer(void *conId);
void *producer(void *prodId);

//store product thread arguments
typedef struct prod_arguments {
	int prod_id;
	int rand_nums;
	int seed;
} PROD_ARGUMENTS;


//Pointer to buffer
circular_buffer *cb;
//check producer thread status
int producerThreadFinished = 0;

//decleare mutex
pthread_mutex_t lock;
pthread_cond_t cond_buffer_empty;
pthread_cond_t cond_buffer_full;

int ** producers;
int ** consumers;

//main func
int main(int argc, char *argv[]) {
    //check input 
	if (argc != 6) {
		printf("ERROR: the program should take 5 arguments,\n 1: the number of producer threads to create,\n 2: the number of cunsumer threads to create\n 3: buffer size, 4: #rand numbers, 5: random numbers seed\n");
		exit(-1);
	}	

	//check input
	if((atoi(argv[1]) | atoi(argv[2])) != 1){
		printf("Error : Producer and Consumer must be 1\n");
		exit(-1);
		}
	
	//thread id
	int producer_id = 1;
	int consumer_id = 1;
	
	//program input
	int max_producer_threads = atoi(argv[1]);	
	int max_consumer_threads = atoi(argv[2]);
	int max_buffer_capacity = atoi(argv[3]);
	int max_rand_nums = atoi(argv[4]);
	int rand_nums_seed = atoi(argv[5]);

	//producer thread args
	PROD_ARGUMENTS prod_Args;
	prod_Args.prod_id = producer_id;
	prod_Args.rand_nums = max_rand_nums;
	prod_Args.seed = rand_nums_seed;

	//store produsers output
	int i;
	producers = malloc(max_producer_threads * sizeof(int *));
	if (max_producer_threads == 0)
		return -1;
	for (i = 0; i < max_producer_threads; i++) {
		producers[i] = malloc(max_rand_nums * sizeof(int));
		if (producers[i] == NULL)
			return -1;
	}
    
	//store consumers output 
	consumers = malloc(max_consumer_threads * sizeof(int *));
	if (max_producer_threads == 0)
		return -1;
	for (i = 0; i < max_consumer_threads; i++) {
		consumers[i] = malloc(max_rand_nums * sizeof(int));
		if (consumers[i] == NULL)
			return -1;
	}

	//Create Threads
	pthread_t *	cons_thread = malloc(max_producer_threads * sizeof(pthread_t));
	pthread_t *	prod_thread = malloc(max_consumer_threads * sizeof(pthread_t));

	//Pointer to buffer	
	cb = (circular_buffer*)malloc(sizeof(struct circular_buffer));
	//initialize circular buffer
	cb_init(cb, max_buffer_capacity, sizeof(char));
    
	//check memory errors
	if (cons_thread == NULL || prod_thread == NULL) {
		printf("NOT ENOUGH MEMORY!\n");
		return -1;
	}
	// mutex initialisation
	pthread_mutex_init(&lock, NULL);


	//Creat threads
	int rc = pthread_create(&prod_thread[0], NULL, producer, &prod_Args);
	int rp = pthread_create(&cons_thread[0], NULL, consumer, &consumer_id);


	//check thread 
	if (rp != 0 || rc != 0) {
		printf("ERROR: return code from prod_thread is %d\n", rp);
		printf("ERROR: return code from cons_thread is %d\n", rc);
		exit(-1);
	}

	void *status;
	//main thread waits for producer thread
	int rpj = pthread_join(prod_thread[0], &status);
    
	//check for join error
	if (rpj != 0) {
		printf("ERROR: return code from pthread_join(prod_thread) is %d\n", rpj);
		exit(-1);
	}
    
	//main thread waits for consumer thread
	int rcj = pthread_join(cons_thread[0], &status);
    
	//check for join error
	if (rcj != 0) {
		printf("ERROR: return code from pthread_join(cons_thread) is %d\n", rcj);
		exit(-1);
	}

    //print producers/consumers to terminal
	print_2d_array(producers, max_producer_threads, max_rand_nums, "Producer ");
	print_2d_array(consumers, max_consumer_threads, max_rand_nums, "Consumer ");

    // destroy mutex
	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&cond_buffer_full);
	pthread_cond_destroy(&cond_buffer_empty);
    
	//free memory
	free(cons_thread);
	free(prod_thread);
	cb_free(cb);
}

//consumer thread func
void *consumer(void *conId) {
	int *tid = (int *)conId;
	int rt;
	int j = 0;

	while (1) {
        
		//check if buffer is empty, if true wait
		while (isEmpty(cb)) {
			//apodesmeush tou mutex gia to 2o thread oso to buffer einai empty			
			rt = pthread_cond_wait(&cond_buffer_empty, &lock);
			//check for error
			if (rt != 0) {
		          printf("ERROR: return code from pthread_cond_wait(cons_thread) is %d\n", rt);
		          exit(-1);
	           }
		}

		// δέσμευση χώρου στην μνήμη για char αριθμό		
		char *tmp_read = (char*)malloc(1);

		//remove item from buffer
		cb_pop_front(cb, (void*)tmp_read);
		//printf("consumer thread with ID:%d removes number: %d from buffer.\n",*tid,*tmp_read);

		//to txt file
		to_txt_out(*tid, *tmp_read, "cons_out.txt", "Consumer");

		//insert to 2d array 
		consumers[*tid - 1][j] = *tmp_read;
		j++;

        //send signal to producer thread
		rt = pthread_cond_signal(&cond_buffer_full);
		//check for error in signal
		if (rt != 0) {
		          printf("ERROR: return code from pthread_cond_signal(prod_thread) is %d\n", rt);
		          exit(-1);
		}
		pthread_mutex_unlock(&lock);
		free(tmp_read);
        
		//check conditions for consumer thread to stop
		if (producerThreadFinished == 1 && isEmpty(cb)) {
			break;
		}
	}
	pthread_exit(tid);
}

//producer thread func
void *producer(void *args) {
	int i,rt;
	int j = 0;
	PROD_ARGUMENTS *prod_Args = (PROD_ARGUMENTS *)args;
	
	srand(time(NULL));
    
	//rand number seed
	unsigned int* seedp;
	int num = prod_Args->prod_id * prod_Args->seed;
	seedp = &num;

	//producer thread work load
	for (i = 0; i < prod_Args->rand_nums; i++) {
		
        //check if buffer is full, if true wait
		while (isFull(cb)) {
			//apodesmeush tou mutex gia to 2o thread oso to buffer einai empty			
			rt = pthread_cond_wait(&cond_buffer_full, &lock);
			//check for error
			if (rt != 0) {
		          printf("ERROR: return code from pthread_cond_wait(prod_thread) is %d\n", rt);
		          exit(-1);
	           }
		}

		// διάβασμα τυχαίου αριθμού [0,256)
		char input = rand_r(seedp) % 256;
		//insert item to buffer
		cb_push_back(cb, (void*)&input);		

		//output to txt
		to_txt_out(prod_Args->prod_id, input, "prod_in.txt", "Producer");

		//store to 2d array
		producers[prod_Args->prod_id - 1][j] = input;
		j++;


		//send signal to consumer thread 		  
		rt = pthread_cond_signal(&cond_buffer_empty);
		//check for error in signal
		if (rt != 0) {
		          printf("ERROR: return code from pthread_cond_signal(prod_thread) is %d\n", rt);
		          exit(-1);
		}
		pthread_mutex_unlock(&lock);

	}
    // producer thread finished
	producerThreadFinished = 1;
	pthread_exit(NULL);
}
























































