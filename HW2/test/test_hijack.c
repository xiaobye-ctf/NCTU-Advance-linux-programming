#include <sys/stat.h>
#include<dlfcn.h>
#include <dirent.h>
#include <sys/types.h>
#include<stdio.h>
#include<limits.h>
#include<stdlib.h>
#include<string.h>
#define FILE_OP_DENY(name) printf("[sandbox] %s: access to %s is not allowed\n",__FUNCTION__,name)
#define ENTER() printf("\e[33;1m[Enter %s()]\e[0m\n",__FUNCTION__)

#define LOAD_FUNC(name) \
	printf("Loading function %s\n",#name);\
	real_##name = dlsym(libc_handle,#name);\
	if(debug && real_##name == NULL)\
		fprintf(stderr, "\e[5;31m%s\e[0m\n",dlerror());\

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
			if(debug)ENTER();\
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
			if(debug)ENTER();\
			MAKEFUNC(func,ret,arg1,arg2)\
			if(valid_access(path)){ \
				return f(a1,a2); \
			}else{ \
				FILE_OP_DENY(path); \
			} \
		}
//DC means Double Check.
//Check for what?Both two path argument.
#define HOOK_ARG_2_DC(func,path1,path2,ret,arg1,a1,arg2,a2)\
	HOOK_VAR(func)\
		ret func(arg1 a1,arg2 a2){ \
			if(debug)ENTER();\
			MAKEFUNC(func,ret,arg1,arg2)\
			if(valid_access(path1) && valid_access(path2)){ \
				return f(a1,a2); \
			}else{ \
				FILE_OP_DENY(path1); \
			} \
		}

#define HOOK_ARG_3(func,path,ret,arg1,a1,arg2,a2,arg3,a3)\
	HOOK_VAR(func)\
		ret func(arg1 a1,arg2 a2,arg3 a3){ \
			if(debug)ENTER();\
			MAKEFUNC(func,ret,arg1,arg2,arg3)\
			if(valid_access(path)){ \
				return f(a1,a2,a3); \
			}else{ \
				FILE_OP_DENY(path); \
			} \
		}
#define HOOK_ARG_4(func,path,ret,arg1,a1,arg2,a2,arg3,a3,arg4,a4)\
	HOOK_VAR(func)\
		ret func(arg1 a1,arg2 a2,arg3 a3,arg4 a4){ \
			if(debug)ENTER();\
			MAKEFUNC(func,ret,arg1,arg2,arg3,arg4)\
			if(valid_access(path)){ \
				return f(a1,a2,a3,a4); \
			}else{ \
				FILE_OP_DENY(path); \
			} \
		}


static int debug=0;
static void *libc_handle;
static void hook_stop() __attribute__((destructor));
static void hook_start() __attribute__((constructor));


int valid_access(const char* target){
	char* root;
	char real_root[PATH_MAX];
	char real_target[PATH_MAX];
	int len_real_root;
	int len_real_target;
	if(debug) ENTER();
	root = getenv("MY_APP_ROOT");
	if(root==NULL){
		return 1;
	}

	realpath(root,real_root);
	realpath(target,real_target);
	len_real_root = strlen(real_root);
	len_real_target = strlen(real_target);
	if(debug){
		printf("\e[32;1mroot: %s, real root: %s\e[0m\n",root,real_root);
		printf("\e[32;1mtarget: %s, target root: %s\e[0m\n",target,real_target);
	}
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
//int chdir(const char *path);
HOOK_ARG_1(chdir,path,int,const char*,path)

/***************/
/*two arguments*/
/***************/
//int chmod(const char *  pathname, mode_t mode);
HOOK_ARG_2(chmod,pathname,int,const char*,pathname,mode_t,mode)
//int mkdir(const char * path, mode_t mode);
HOOK_ARG_2(mkdir,path,int,const char*,path,mode_t,mode)
//int creat(const char *path, mode_t mode);
HOOK_ARG_2(creat,path,int,const char*,path,mode_t,mode)
//int open(const char *pathname, int flags);
HOOK_ARG_2(open,pathname,int,const char*,pathname,int,flags)
//FILE *fopen(const char *filename, const char *mode)
HOOK_ARG_2(fopen,filename,FILE*,const char*,filename,const char*, mode)


/******************************/
/*two arguments(special cases)*/
/******************************/
//int rename(const char *old, const char *new);
HOOK_ARG_2_DC(rename,old,new,int,const char*,old,const char*,new)
//int symlink(const char *path1, const char *path2);
HOOK_ARG_2_DC(symlink,path1,path2,int,const char*,path1,const char*,path2)
//int link(const char * path1, const char *path2);
HOOK_ARG_2_DC(link,path1,path2,int,const char *,path1,const char*,path2)

//HOOK_ARG_2(stat,path,int,const char*,path,struct stat *,stat_buf)
/*****************/
/*three arguments*/
/*****************/
//int chown(const char *path, uid_t owner, gid_t group);
HOOK_ARG_3(chown,path,int,const char *,path,uid_t,owner,gid_t,group)
//ssize_t readlink(const char *path, char *buf, size_t bufsiz);
HOOK_ARG_3(readlink,path,ssize_t,const char*,path,char*,buf,size_t,bufsiz)
//int __xstat(int ver, const char * path, struct stat * stat_buf);
HOOK_ARG_3(__xstat,path,int,int,ver,const char *,path,struct stat*,stat_buf)


/*****************/
/*four arguments*/
/*****************/
//int openat(int dirfd, const char *pathname, int flags,mode_t mode)
HOOK_ARG_4(openat,pathname,int,int,dirfd,const char*,pathname,int,flags,mode_t,mode);

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
	LOAD_FUNC(opendir);
	LOAD_FUNC(remove);
	LOAD_FUNC(unlink);
	LOAD_FUNC(rmdir); 
	LOAD_FUNC(chdir);
	LOAD_FUNC(chmod);
	LOAD_FUNC(mkdir);
	LOAD_FUNC(creat);
	LOAD_FUNC(open);
	LOAD_FUNC(fopen);
	LOAD_FUNC(chown);
	LOAD_FUNC(readlink);
	LOAD_FUNC(__xstat);
	LOAD_FUNC(openat);
	LOAD_FUNC(rename);
	LOAD_FUNC(symlink);
	LOAD_FUNC(link);
	//LOAD_FUNC(stat);
}

void hook_stop(){
	if(debug){
		ENTER();
		printf("Stop hooking....\n");
	}
}
