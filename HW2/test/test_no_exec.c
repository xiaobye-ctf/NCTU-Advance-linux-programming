#include <sys/stat.h>
#include<dlfcn.h>
#include <dirent.h>
#include <sys/types.h>
#include<stdio.h>
#include<limits.h>
#include<stdlib.h>
#include<string.h>
#define ENTER() printf("\e[33;1m[Enter %s()]\e[0m\n",__FUNCTION__)
#define HOOK_EXEC(func,path,ret,...)\
	ret func(__VA_ARGS__){\
		if(debug)ENTER();\
		EXEC_OP_DENY(path);\
		return -1;\
	}
	
#define EXEC_OP_DENY(name) printf("[sandbox] %s(%s): not allowed\n",__FUNCTION__,name)

static int debug=0;
static void *libc_handle;
static void hook_stop() __attribute__((destructor));
static void hook_start() __attribute__((constructor));


//int execl(const char *path, const char *arg, ...);
HOOK_EXEC(execl,path,int,const char*path,const char*arg,...)
//int execlp(const char *file, const char *arg, ...);
HOOK_EXEC(execlp,file,int,const char*file,const char*arg,...)
//int execle (const char *__path, const char *__arg, ...)
HOOK_EXEC(execle,__path,int,const char *__path, const char *__arg, ...)
//int execv(const char *path, char *const argv[]);
HOOK_EXEC(execv,path,int,const char *path, char *const argv[])
//int execvp(const char *file, char *const argv[]);
HOOK_EXEC(execvp,file,int,const char *file, char *const argv[])
//int system(const char *command)
HOOK_EXEC(system,command,int,const char *command)

void hook_start(){
#ifdef DEBUG
	debug=1;
#endif
	if(debug){
		ENTER();
		printf("Start hooking....\n");
		printf("Start loading libc.so.6\n");
	}
	if((libc_handle = dlopen("libc.so.6",RTLD_LAZY))==NULL){
		fprintf(stderr,"libc_handle: %llX",libc_handle);
		fprintf(stderr,"dlopen() error: %s\n",dlerror());
	}
}

void hook_stop(){
	if(debug){
		ENTER();
		printf("Stop hooking....\n");
	}
}

