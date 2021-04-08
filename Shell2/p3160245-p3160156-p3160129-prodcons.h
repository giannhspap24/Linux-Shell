#include "stdio.h" 
#include "sys/types.h"
#include "stdlib.h"
#include "string.h"

typedef struct circular_buffer
{
	void *buffer;     // data buffer
	void *buffer_end; // end of data buffer
	size_t capacity;  // maximum number of items in the buffer
	size_t count;     // number of items in the buffer
	size_t sz;        // size of each item in the buffer
	void *head;       // pointer to head
	void *tail;       // pointer to tail
} circular_buffer;

void cb_init(circular_buffer *cb, size_t capacity, size_t sz);

void cb_free(circular_buffer *cb);

void cb_push_back(circular_buffer *cb, const void *item);

void cb_pop_front(circular_buffer *cb, void *item);

int isFull(circular_buffer *cb);

int isEmpty(circular_buffer *cb);

void to_txt_out(int pd, char input, char * file_name, char * str2);

void open_file( char * file_name);

void print_2d_array(int ** arr, int num_rows, int num_columns, char * str);











