#include <sys/stat.h>
#include<dlfcn.h>
#include <dirent.h>
#include <sys/types.h>
#include<stdio.h>
#include<limits.h>
#include<stdlib.h>
#include<string.h>
#define FILE_OP_DENY(name) printf("[sandbox] %s: access to %s is not allowed\n",__FUNCTION__,name)
#define ENTER() printf("\e[0;33;1m[Enter %s()]\e[0m\n",__FUNCTION__)


#define HOOK_VAR(func)   static void* real_##func = NULL;
#define MAKEFUNC(func,ret,...) ret (*f)(__VA_ARGS__) = (ret (*)(__VA_ARGS__)) real_##func;
//func: target funciton
//path: path to validate
//ret: type of return value 
//arg1: type of arg1
//a1: function's first arguments
#define HOOK_ARG_1(func,path,ret,arg1,a1)\
	HOOK_VAR(func)\
		ret func(arg1 a1){ \
			ENTER();\
			MAKEFUNC(func,ret,arg1)\
			if(valid_access(path)){ \
				return f(a1); \
			}else{ \
				FILE_OP_DENY(path); \
			} \
		}
#define HOOK_ARG_2(func,path,ret,arg1,a1,arg2,a2)\
	HOOK_VAR(func)\
		ret func(arg1 a1,arg2 a2){ \
			ENTER();\
			MAKEFUNC(func,ret,arg1,arg2)\
			if(valid_access(path)){ \
				return f(a1,a2); \
			}else{ \
				FILE_OP_DENY(path); \
			} \
		}

static void *libc_handle;
static void hook_stop() __attribute__((destructor));
static void hook_start() __attribute__((constructor));


int valid_access(const char* target){
	char* root;
	char real_root[PATH_MAX];
	char real_target[PATH_MAX];
	int len_real_root;
	int len_real_target;
#ifdef DEBUG
	ENTER();
#endif
	root = getenv("MY_APP_ROOT");
	if(root==NULL){
		return 1;
	}

	realpath(root,real_root);
	realpath(target,real_target);
	len_real_root = strlen(real_root);
	len_real_target = strlen(real_target);
#ifdef DEBUG
	printf("\e[0;32;1mroot: %s, real root: %s\e[0m\n",root,real_root);
	printf("\e[0;32;1mtarget: %s, target root: %s\e[0m\n",target,real_target);

#endif
	if(len_real_target>=len_real_root && strncmp(real_root,real_target,len_real_target)==0){
		return 1;	
	}else{
		return 0;
	}
}
/**************/
/*one argument*/
/**************/
//DIR *opendir(const char *name);
HOOK_ARG_1(remove,filename,int,const char*,filename)
//int remove(const char *filename)
HOOK_ARG_1(opendir,name,DIR*,const char*,name)
//int unlink(const char *pathname);
HOOK_ARG_1(unlink,pathname,int,const char*,pathname)
//int rmdir(const char *path);
HOOK_ARG_1(rmdir,path,int,const char*,path)

/**************/
/*two argument*/
/**************/
//int stat(const char *pathname, struct stat *statbuf);
HOOK_ARG_2(stat,pathname,int,const char*,pathname,struct stat*, statbuf)
//int stat(const char *pathname, struct stat *statbuf);
void hook_start(){
#ifdef DEBUG
	ENTER();
	printf("Start hooking....\n");
	printf("Start loading libc.so.6\n");
#endif
	if((libc_handle = dlopen("libc.so.6",RTLD_LAZY))==NULL){
		printf("libc_handle: %llX",libc_handle);
		printf("dlopen() error: %s\n",dlerror());
	}
	real_opendir = dlsym(libc_handle,"opendir");
	real_remove = dlsym(libc_handle,"remove");
	real_unlink = dlsym(libc_handle,"unlink");
	real_rmdir = dlsym(libc_handle,"rmdir"); 
	real_stat = dlsym(libc_handle,"stat");
}

void hook_stop(){
#ifdef DEBUG
	ENTER();
	printf("Stop hooking....\n");
#endif

}
