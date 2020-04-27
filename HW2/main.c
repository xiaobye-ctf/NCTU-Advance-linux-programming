#define _GNU_SOURCE
#include <sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>
extern char **environ;
int main(int argc,char **argv){
	int pid;
	int stat;
	char* arg[]={"ls","-al",NULL};
//char* env[]={"PATH=/usr/local/bin:/usr/bin:/bin:/usr/local/games:/usr/games:/home/xiaobye/.local:/sbin",NULL};
	char * env[]={"A=1",NULL};
	if((pid=fork())<0){
		perror("fork: ");
		exit(-1);
	}else if(pid > 0){
#ifdef DEBUG
		printf("parent: %d\n",getpid());

#endif
		wait(&stat);
	}else{
#ifdef DEBUG
		printf("child: %d\n",getpid());
#endif
		execvpe("ls",arg,NULL);	
		sleep(100);
	}	
}
