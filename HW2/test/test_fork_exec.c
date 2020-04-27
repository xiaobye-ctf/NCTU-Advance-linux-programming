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
	char* arg[]={"cat","/proc/self/maps",NULL};
	char *path;
	char new_path[10000];
	char ld_preload[PATH_MAX];
	char * env[3]={NULL,NULL,NULL};
	char *lib = "/lib/x86_64-linux-gnu/libmm-glib.so.0";

	path = getenv("PATH");
	sprintf(new_path,"%s:./",path);
	sprintf(ld_preload,"LD_PRELOAD=%s",lib);

	env[0]=new_path;
	env[1]=ld_preload;
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
		execvpe("cat",arg,env);	
	}	
}
