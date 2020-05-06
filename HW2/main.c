#define _GNU_SOURCE
#include <sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include<limits.h>
extern char **environ;
int main(int argc,char **argv){
	int pid;
	int stat;
	char* arg[]={"./test_case",NULL};
	char *path;
	char new_path[10000];
	char ld_preload[PATH_MAX];
	char * env[4]={NULL,NULL,NULL,NULL};
	char *lib = "./sandbox.so";

	path = getenv("PATH");
	sprintf(ld_preload,"LD_PRELOAD=%s",lib);
	sprintf(new_path,"%s:./",path);

	env[0]=new_path;
	env[1]=ld_preload;
	env[2]="MY_APP_ROOT=./";
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
		execvpe("./test_case",arg,env);	
	}	
}
