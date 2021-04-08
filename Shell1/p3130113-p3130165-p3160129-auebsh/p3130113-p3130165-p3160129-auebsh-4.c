#include "p3130113-p3130165-p3160129-auebsh.h"



int main(void){
	
	
	int filein,fileout;
	char *line=malloc(sizeof(char)*255);//desmeush enos pinaka 255 xarakthrvn gia to line kai epistrofh tou pointer se ayton
	char **args=malloc(sizeof(char)*255);//desmeush enos pinaka 255 xarakthrvn gia to args kai epistrofh tou pointer se ayton

	if (line==NULL){//sfalma kata thn desmeush ths mnhmhs
		perror("Problem with memory allocation.\n");
		return 0;
	}	
	
	if (args==NULL){//sfalma kata thn desmeush ths mnhmhs
		perror("Problem with memory allocation,\n");
		return 0;
	}

	
	filein=dup(0);//dhmiourgia antigrafou
	fileout=dup(1);//dhmiourgia antigrafou
	printf("auebsh4>");
	line=read1();//stn metablhth line pernietai to input


	   while(line != NULL ){	
		args=split_line(line);
		if(feof(stdin)){
		break;
		}
		run4(args,filein,fileout);
		free(line);
	   	free(args);
		printf("auebsh4>");
		line=read1();
	   }


	printf("\n");
	return 0;
}

