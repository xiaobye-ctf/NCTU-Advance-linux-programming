#define _GNU_SOURCE
#include <sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include<limits.h>
#define HOOK_LIB "./sandbox.so"
extern char **environ;
static char new_path[PATH_MAX];
static char *path;
static char * env[4]={NULL,NULL,NULL,NULL};

void config_env(const char * app_root,const char* debug){
	path = getenv("PATH");
	snprintf(new_path,PATH_MAX,"%s:./",path);
	
	setenv("LD_PRELOAD",HOOK_LIB,1);
	setenv("PATH",new_path,1);
	setenv("MY_APP_ROOT",app_root,1);
	setenv("DEBUG",debug,1);//"True" or "False"

}
int main(int argc,char **argv){
	config_env("./","True");
	printf("Finsh!\n");
	return 0;
}
