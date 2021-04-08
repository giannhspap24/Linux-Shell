#include "p3130113-p3130165-p3160129-auebsh.h"

int main(void) {
	
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

	
	printf("auebsh1>");
	line=read1();//stn metablhth line pernietai to input
	
	while(line != NULL){
		
		args=split_line(line);//spame se args thn line
		if(feof(stdin)){
			break;//to ctrl+D kanei eksodo apo to shell1
		}
		if(args[1]!=NULL){
			perror("Too many arguments!");//to prwto shell dn dexetai parametroys
		}else if(strcmp(args[0],">>") == 0){
			perror("Syntax Error");
		}else{
			execute(args);//ektelountai oi entoles poy dothhkan
		}

		printf("auebsh1>");
		line=read1();
	}

	free(line);//apodesmeush mnhmhs
	free(args);//apodesmeush mnhmhs		

	printf("\n");
	return 0;
}
