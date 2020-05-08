# AUP HOMEWORK 2

## Hijack library
### filesystem's functions
```bash
chown
fopen
openat
chdir
open
creat
mkdir
remove
opendir
unlink
rmdir
chmod
readlink
rename
symlink
link
__xstat

#discover by instruction below
gcc sample.c -o sample2 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 && \
(nm -D sample2 |grep 64)

creat64
fopen64
open64
openat64
__xstat64
```
This function make use of function `realpath` that define `stdlib.h` to canonicalize path name,and then compare `real_root` with `real_target` to determine if access `target` is allowed.
```c
//validate access right
int valid_access(const char* target){
	char* root;
	char real_root[PATH_MAX],real_target[PATH_MAX];
	int len_real_root,len_real_target;
	if(debug) ENTER();
	root = getenv("XIAOBYE_SANDBOX_ROOT");
	if(root==NULL||target == NULL){
		printf("no root or target!??\n");
		return 1;
	}
	
	realpath(root,real_root);
	realpath(target,real_target);

	len_real_root = strlen(real_root);
	len_real_target = strlen(real_target);

	if(debug){
		printf("\e[32;1mroot: %s, real root: %s\e[0m\n",root,real_root);
		printf("\e[32;1mtarget: %s, real target: %s\e[0m\n",target,real_target);
	}

	if((len_real_target>=len_real_root) && (strncmp(real_target,real_root,len_real_root)==0)){
		if(debug) printf("\e[34;1mAccess allow!\e[0m\n");
		return 1;	
	}else{
		if(debug) printf("\e[34;1mAccess deny!\e[0m\n");
		return 0;
	}
}

```
For simple,we only show basic macro implementation that design for one-argument function.
If we want to hook a function ,for example chdir,we only need to add `HOOK_ARG_1(chdir,path,int,const char*,path)` into our shared library's source code
```c
//redirect our output to file discriptor that is larger than 2
#define OUTPUT_REDIRECT()\
	if(my_out==NULL){\
		int new_fd=dup(0);\
		my_out=fdopen(new_fd,"w");\
		if(debug && my_out==NULL)fprintf(stderr,"fopen error!");\
	}\
//Display error message if access not allow
#define FILE_OP_DENY(name) \
	OUTPUT_REDIRECT()\
	fprintf(my_out,"[sandbox] %s: access to %s is not allowed\n",__FUNCTION__,name);
    
//Debugging message for me to observe behavior of instruction
#define ENTER() printf("\e[33;1m[Enter %s()]\e[0m\n",__FUNCTION__)

//define function variable for the function that we want to hook
#define MAKEFUNC(func,ret,...) static ret (*real_##func)(__VA_ARGS__) = NULL;
#define LOAD_FUNC(name) \
	if(libc_handle==NULL){\
		libc_handle=dlopen("libc.so.6",RTLD_LAZY);\
		if(libc_handle==NULL)fprintf(stderr,"dlopen error:%s\n",dlerror());\
	}\
	if(real_##name==NULL){\
		if(debug)printf("Loading function %s\n",#name);\
		real_##name = dlsym(libc_handle,#name);\
	}\
	if(debug && real_##name == NULL)fprintf(stderr, "\e[5;31m%s\e[0m\n",dlerror());\
//func: target funciton
//path: path to validate
//ret: type of return value 
//arg1: type of arg1
//a1: function's first arguments
#define HOOK_ARG_1(func,path,ret,arg1,a1)\
		MAKEFUNC(func,ret,arg1)\
		ret func(arg1 a1){ \
			if(debug)ENTER();\
			if(valid_access(path)){ \
				LOAD_FUNC(func)\
				if(real_##func!=NULL)return real_##func(a1); \
			}else{ \
				FILE_OP_DENY(path); \
			} \
			return -1;\
		}
```
### exec's functions
```bash
execl
execlp
execle
execv
system
execve
```
It is much easier to implement hooking exec*.  
Only need to hook them and then return -1.  
If we want to add a hook for these functions,for example `system`,then we only need to add `HOOK_EXEC(system,command,int,const char *command)` in shared library's source code.

```c
#define EXEC_OP_DENY(name)\
	OUTPUT_REDIRECT()\
	fprintf(my_out,"[sandbox] %s(\"%s\"): not allowed\n",__FUNCTION__,name)

#define HOOK_EXEC(func,path,ret,...)\
	ret func(__VA_ARGS__){\
		if(debug)ENTER();\
		EXEC_OP_DENY(path);\
		return -1;\
	}
```

## Command launcher
Concatenate LD_PRELOAD,command and arguments into arrary `cmd` and then call `system(cmd)`
```c
void  exec_cmd(int argc,char **argv,const char * app_root,const char *hook_lib,const char* debug){
	char cmd[10000];
	char tmp[10000];
	if(argc==optind){
		fprintf(stderr,"no command given.\n");
		exit(1);
	}
	setenv("XIAOBYE_SANDBOX_ROOT",app_root,1);
	setenv("XIAOBYE_DEBUG",debug,1);//"True" or "False"

	snprintf(cmd,10000,"LD_PRELOAD=%s",hook_lib);
	for(;optind<argc;optind++){
		snprintf(tmp,10000,"%s %s",cmd,argv[optind]);
		snprintf(cmd,10000,"%s",tmp);
	}

	system(cmd);
}

```
