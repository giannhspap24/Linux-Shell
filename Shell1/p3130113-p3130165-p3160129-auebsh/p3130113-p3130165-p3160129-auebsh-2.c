#include "p3130113-p3130165-p3160129-auebsh.h"



int main(void) {
	
	int i=0;
	int notok=1;
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
	printf("auebsh2>");
	line=read1();//stn metablhth line pernietai to input

	   
	while(line != NULL ){	
		args=split_line(line);//spame se args thn line
     	  	while(args[i]!=NULL){
			if(args[i][0]=='-'){//elegxos gia entopismo parametrwn
				perror("Invalid arguments");
				notok=100;//brhkame ena argument to opoio einai parametros thn opoia den mporoume na ektelesoume sto sygkekrimeno shell
			}
			i++;
		}
		if(feof(stdin)){
			break;//to ctrl+D kanei eksodo apo to shell1
			
		}
	        if(notok!=100){
		args=run2_3(args);		
		execute(args);//ektelountai oi entoles poy dothhkan
		dup2(filein,0);
		dup2(fileout,1);
		}
		i=0;
		notok=1;
		free(line);//apodesmeush mnhmhs
	   	free(args);//apodesmeush mnhmhs

		printf("auebsh2>");
		line=read1();

	   }	

	printf("\n");
	return 0;
}
