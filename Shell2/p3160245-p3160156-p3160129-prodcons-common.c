#include "p3160245-p3160156-p3160129-prodcons.h"
//initialize circular buffer
//capacity: maximum number of elements in the buffer
//sz: size of each element 
void cb_init(circular_buffer *cb, size_t capacity, size_t sz)
{
	cb->buffer = malloc(capacity * sz);
	if (cb->buffer == NULL) {
		printf("Could not allocate memory..Exiting! \n");
		exit(1);
	}
	// handle error
	cb->buffer_end = (char *)cb->buffer + capacity * sz;
	cb->capacity = capacity;
	cb->count = 0;
	cb->sz = sz;
	cb->head = cb->buffer;
	cb->tail = cb->buffer;
}

//destroy circular buffer
void cb_free(circular_buffer *cb)
{
	free(cb->buffer);
	// clear out other fields too, just to be safe
}

//add item to circular buffer
void cb_push_back(circular_buffer *cb, const void *item)
{
	if (cb->count == cb->capacity)
	{
		printf("Access violation. Buffer is full\n");
		exit(1);
	}
	memcpy(cb->head, item, cb->sz);
	cb->head = (char*)cb->head + cb->sz;
	if (cb->head == cb->buffer_end)
		cb->head = cb->buffer;
	cb->count++;
}

//remove first item from circular item
void cb_pop_front(circular_buffer *cb, void *item)
{
	if (cb->count == 0)
	{
		printf("Access violation. Buffer is empy\n");
		exit(1);
	}
	memcpy(item, cb->tail, cb->sz);
	cb->tail = (char*)cb->tail + cb->sz;
	if (cb->tail == cb->buffer_end)
		cb->tail = cb->buffer;
	cb->count--;
}

//checks if buffer is full
int isFull(circular_buffer *cb) {
	if (cb->count == cb->capacity)
		return 1;
	return 0;
}

//checks if buffer is empty
int isEmpty(circular_buffer *cb) {
	if (cb->count == 0)
		return 1;
	return 0;
}

// write to txt file 
void to_txt_out(int pd, char input, char * file_name, char * str2) {
	FILE *f = fopen(file_name, "a");
	if (f == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}

	fprintf(f, "%s %d: %d\n", str2, pd, input);
	fclose(f);
}

void open_file( char * file_name){
	FILE *f = fopen(file_name, "w");
}


//print 2d array
void print_2d_array(int ** arr, int num_rows, int num_columns, char * str) {
	int j = 0;
	int k = 0;
	for (k = 0; k < num_rows; k++) {
		printf("%s %s %d :","\u25AA", str, k + 1);
		for (j = 0; j < num_columns; j++) {
			if(arr[k][j] != 0){			
			printf("%d, ", arr[k][j]);
			}
		}
		printf("\n");
	}
}




