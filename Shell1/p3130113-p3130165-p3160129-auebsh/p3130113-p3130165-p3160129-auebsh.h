#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdbool.h>



void run5(char **args,int input,int out);

void run4(char **args,int input,int out);

void pipeline(char ** input);

char ** run2_3(char ** args);

void execute(char ** args);

char * read1();

char ** split_line(char * line);

void handle(char * args,int flag);
