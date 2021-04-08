#include "p3130113-p3130165-p3160129-auebsh.h"


void run5(char **args,int filein,int fileout){
	
	char ** final = malloc(sizeof(char)*255);//telikes entoles gia execute
	
	if (final == NULL) {
		printf("Out of memory!\n");
		return;
	}

	int redirect=0;// 0 false kai 100 true
	int pipes=0; //an panw apo ena pipe lathos
	int right=0;  //
	int left=0;
	int i=0;
	int j=0;
	int len=0; //length of cmd

	while(args[len]!=NULL){
		if(strcmp(args[len],"<")==0){
			right++;	
		}
		if(strcmp(args[len],">")==0){
			left++;	
		}
		if(strcmp(args[len],">>") == 0){
			perror("Syntax Error");
			exit(0);
		}
		if(strcmp(args[len],"|")==0){	
			pipes++;
		}
		len++;
	}
	
	if((right==0)&&(left==0)&&(pipes==0)){
		if(len==1){
			final[0]=args[0];
			
		}else{
			for(i=0;i<len; i++){
				final[i]=args[i];
			}
		}
		execute(final);
	}
	if(right+left>0){ //exoume redirection
		if(pipes==0){    //den exoume swlinwsh,ara oti kai sto shell3
			for(i=0;i<len; i++){
				final[i]=args[i];
			}
			final=run2_3(final);
			execute(final);
		}else{   //exoume pipe

			while(args[i]!=NULL){
				
				if(strcmp(args[i],"<")==0){
					i++;
					handle(args[i],0);
				}else if(strcmp(args[i],">")==0){		
			         	i++;	
				 	handle(args[i],1);
				 	redirect=100;
				}else if(strcmp(args[i],"|")==0){	
						if(redirect==0){//dn exei >
							pipeline(final);
							pipes=0;
						}else{// exei >
							execute(final);
							redirect=0;
							dup2(fileout,1);
						}
						memset(final, 0, sizeof(char)*255);//gemizei me 0(2o orisma) ton pinaka final
						j=0;
				}else{
					final[j]=args[i];
					j++;
				}
				i++; 
			}
		}
		execute(final);
	}
	dup2(filein, 0);
	dup2(fileout, 1);
}



void run4(char **args,int filein,int fileout){
	
	char ** final = malloc(sizeof(char)*255);//telikes entoles gia execute
	
	if (final == NULL) {
		printf("Out of memory!\n");
		return;
	}

	int redirect=0;// 0 false kai 100 true
	int pipes=0; //an panw apo ena pipe lathos
	int right=0;  //
	int left=0;
	int i=0;
	int j=0;
	int len=0; //length of cmd

	while(args[len]!=NULL){
		if(strcmp(args[len],"<")==0){
			right++;	
		}
		if(strcmp(args[len],">")==0){
			left++;	
		}
		if(strcmp(args[len],">>") == 0){
			perror("Syntax Error");
			exit(0);
		}
		if(strcmp(args[len],"|")==0){	
			pipes++;
		}
		len++;
	}
	
	if(pipes>1){
		perror("More than one pipes");
		exit(0);
	}

	if((right==0)&&(left==0)&&(pipes==0)){
		if(len==1){
			final[0]=args[0];
			
		}else{
			for(i=0;i<len; i++){
				final[i]=args[i];
			}
		}
		execute(final);
	}
	if(right+left>0){ //exoume redirection
		if(pipes==0){    //den exoume swlinwsh,ara oti kai sto shell3
			for(i=0;i<len; i++){
				final[i]=args[i];
			}
			final=run2_3(final);
			execute(final);
		}else{   //exoume pipe

			while(args[i]!=NULL){
				
				if(strcmp(args[i],"<")==0){
					i++;
					handle(args[i],0);
				}else if(strcmp(args[i],">")==0){		
			         	i++;	
				 	handle(args[i],1);
				 	redirect=100;
				}else if(strcmp(args[i],"|")==0){	
						if(redirect==0){//dn exei >
							pipeline(final);
							pipes=0;
						}else{// exei >
							execute(final);
							redirect=0;
							dup2(fileout,1);
						}
						memset(final, 0, sizeof(char)*255);//gemizei me 0(2o orisma) ton pinaka final
						j=0;
				}else{
					final[j]=args[i];
					j++;
				}
				i++; 
			}
		}
		execute(final);
	}
	dup2(filein, 0);
	dup2(fileout, 1);
}






char ** run2_3(char ** args){

	char ** final=malloc(sizeof(char)*255);
	int a=0,b=0;
	int flag=0;	

	while(args[a]!=NULL){
		if(strcmp(args[a],"<")==0){
			a++;
			flag=0;//to flag orizei oti prokytai gia input
			handle(args[a],flag);
		}else if (strcmp(args[a],">")==0){
			a++;
			flag=1;//to flag orizei oti prokytai gia output
			handle(args[a],flag);
		}else if (strcmp(args[a],">>")==0){//mh egkyros syntaktika xarakthras
			perror("Syntax Error");
			exit(1);				
		}else{
			final[b]=args[a];
			b++;
		}
		a++;
	}
return final;
}

void handle(char * args,int flag){

	int  x;

	if(flag==0){
		x=open(args, O_RDWR);//anoigei to prwto arxeia gia dinei thn dynatothta gia read kai write
		dup2(x,0);
	}else if(flag==1){
		x=open(args,O_RDWR | O_TRUNC | O_CREAT | O_APPEND,0777);//dhmiourgei arxeio an den yparxei,an yparxei sbhnei ta periexomeno toy.me to 0777 orizei t dikaiwmata 
		dup2(x,1);
	}
}


char *read1(){

	char *line=malloc(sizeof(char)*255);
	

	if (line==NULL){
		perror("Problem with memory allocation.\n");
		return 0;
	}	

	fgets(line,765,stdin);//dexomaste triplasio input apo to apaitoumeno kai sthn synexeia
	if(strlen(line)>255){//elegxoume ean ksepernaei to 255 kai emfanizoume katallhlo mynhma
		printf("Out of limit!\n");//mias kai h fgets dn ektelei ton sygkekrimeno elegxo
		exit(0);
	}
		
	return line;
}


char ** split_line(char * line){
	
	int i=0;
	char * q=malloc(sizeof(char)*255);
	char ** qq=malloc(sizeof(char)*255);

	if (q==NULL){//sfalma kata thn desmeush ths mnhmhs
		perror("Problem with memory allocation.\n");
		return 0;
	}
	
	if (qq==NULL){//sfalma kata thn desmeush ths mnhmhs
		perror("Problem with memory allocation.\n");
		return 0;
	}

	line = strtok(line,"\n");//krataei ws line thn line ews tn xarakthra \n(allagh grammhs)
	q =strtok(line," ");//krataei ws q to prwto token mexri to prwto keno

	//synexizei me t ypoloipa token
	while(true){
		qq[i]=q;
		q= strtok(NULL," ");
		i++;
		if(q==NULL){//molis to teleutaio stoixeio ginei NULL teleiwnei me ta args
			break;
		}	
	
	}

	free(q);//apodesmeush mnhmhs
	return qq;//epistrofh tou pinaka me ta args
}

void execute(char **args){
	pid_t pid, waitPid;
	int  status;
	pid = fork();

	if (pid < 0) {
		perror("ERROR: Fork failed.\n");
		exit(1);
	}
	if (pid == 0) {	
		execvp(args[0],args);//ektelesh ths entolhs kai eksodos 
		exit(1);
	}else{
		waitPid = wait(&status);//apotrepei diergasies zombie
		if (waitPid == -1) {
			perror("ERROR: Waitpid failed.\n");
			exit(1);
		}
	}
}


void pipeline(char ** args){
	
	pid_t pid, waitPid;
	int  length, status, fd[2], returnValue;
	
	returnValue = pipe(fd);		//fd[0]: pipe input, fd[1]: pipe output
	if (returnValue == -1) {
		printf("ERROR: Pipe command failed.\n");
	}

	pid = fork();
	
	//elegxos oti i pipe petyxe.
	if (pid < 0) {
		perror("ERROR: Fork failed.\n");
		//return -1;
	}
	//diergaria paidi.
	if (pid == 0) {	
		
		close(1);
		dup(fd[1]);
		close(fd[0]);
		execvp(args[0],args);

	}//diergasia pateras
	else {
		close(0);
		dup(fd[0]);
		close(fd[1]);
		//o pateras perimenei ton termatismo tou paidiou.
		waitPid = wait(&status);
		if (waitPid == -1) {
			perror("ERROR: Waitpid failed.\n");
		}
		close(fd[0]);
	}
}

