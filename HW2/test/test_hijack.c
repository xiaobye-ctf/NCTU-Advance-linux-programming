#include<dlfcn.h>
#include <dirent.h>
#include <sys/types.h>
#include<stdio.h>
#include<limits.h>
#include<stdlib.h>
#include<string.h>

#define FILE_OP_DENY() printf("[sandbox] %s: access to %s is not allowed\n",__FUNCTION__,name)
#define DEBUG_INFO() printf("Enter %s\n",__FUNCTION__)
static void *libc_handle;
static void hook_stop() __attribute__((destructor));
static void hook_start() __attribute__((constructor));

DIR* (*real_opendir)(const char*) = NULL;

void hook_start(){
#ifdef DEBUG
	printf("Start hooking....\n");
	printf("Start loading libc.so.6\n");
#endif
	if((libc_handle = dlopen("libc.so.6",RTLD_LAZY))==NULL){
		printf("dlopen() error: %s\n",dlerror());
	}
	real_opendir = dlsym(libc_handle,"opendir");

}
int valid_access(const char* target){
	char* root;
	char real_root[PATH_MAX];
	char real_target[PATH_MAX];
	int len_real_root;
	int len_real_target;
	
	root = getenv("MY_APP_ROOT");
	if(root==NULL){
		return 1;
	}

	realpath(root,real_root);
	realpath(target,real_target);
	len_real_root = strlen(real_root);
	len_real_target = strlen(real_target);
#ifdef DEBUG
	DEBUG_INFO();
	printf("root: %s, real root: %s\n",root,real_root);
	printf("target: %s, target root: %s\n",target,real_target);
#endif
	if(len_real_target>=len_real_root && strncmp(real_root,real_target,len_real_target)==0){
		return 1;	
	}else{
		return 0;
	}
}

DIR* opendir(const char* name){
	if(valid_access(name)){
		real_opendir(name);
	}else{
		FILE_OP_DENY();
	}
}
