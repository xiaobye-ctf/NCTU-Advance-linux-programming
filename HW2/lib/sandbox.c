#include <unistd.h>
#include <sys/stat.h>
#include<dlfcn.h>
#include <dirent.h>
#include <sys/types.h>
#include<stdio.h>
#include<limits.h>
#include<stdlib.h>
#include<string.h>
#include"comm.h"

static int debug=0;
static void *libc_handle=NULL;

//validate access right
int valid_access(const char* target){
	char* root,*real_root,*real_target;
	int len_real_root,len_real_target;
	if(debug) ENTER();
	root = getenv("XIAOBYE_SANDBOX_ROOT");
	if(root==NULL||target == NULL){
		printf("no root or target!??\n");
		return 1;
	}
	
	real_root=realpath(root,NULL);
	if(real_root==NULL){
		return 0;
	}

	real_target=realpath(target,NULL);
	if(real_target==NULL){
		free(real_root);
		return 0;
	}
	len_real_root = strlen(real_root);
	len_real_target = strlen(real_target);

	if(debug){
		printf("\e[32;1mroot: %s, real root: %s\e[0m\n",root,real_root);
		printf("\e[32;1mtarget: %s, real target: %s\e[0m\n",target,real_target);
	}

	if((len_real_target>=len_real_root) && (strncmp(real_target,real_root,len_real_root)==0)){
		free(real_root);
		free(real_target);
		if(debug) printf("\e[34;1mAccess allow!\e[0m\n");
		return 1;	
	}else{
		free(real_root);
		free(real_target);
		if(debug) printf("\e[34;1mAccess deny!\e[0m\n");
		return 0;
	}
}


/**************/
/*one argument*/
/**************/
//DIR *opendir(const char *name);
HOOK_ARG_1(remove,filename,int,const char*,filename)
//int unlink(const char *pathname);
HOOK_ARG_1(unlink,pathname,int,const char*,pathname)
//int rmdir(const char *path);
HOOK_ARG_1(rmdir,path,int,const char*,path)
//int chdir(const char *path);
HOOK_ARG_1(chdir,path,int,const char*,path)

/****************************/
/*one argument(special case)*/
/****************************/
//int remove(const char *filename)
HOOK_ARG_1_NUL(opendir,name,DIR*,const char*,name)

/***************/
/*two arguments*/
/***************/
//int chmod(const char *  pathname, mode_t mode);
HOOK_ARG_2(chmod,pathname,int,const char*,pathname,mode_t,mode)
//int mkdir(const char * path, mode_t mode);
HOOK_ARG_2(mkdir,path,int,const char*,path,mode_t,mode)
//int creat(const char *path, mode_t mode);
HOOK_ARG_2(creat,path,int,const char*,path,mode_t,mode)
//int creat64(const char *path, mode_t mode);
HOOK_ARG_2(creat64,path,int,const char*,path,mode_t,mode)
//int open(const char *pathname, int flags);
HOOK_ARG_2(open,pathname,int,const char*,pathname,int,flags)
//int open64(const char *pathname, int flags);
HOOK_ARG_2(open64,pathname,int,const char*,pathname,int,flags)

/*****************************/
/*two arguments(special case)*/
/*****************************/
//FILE *fopen(const char *filename, const char *mode)
HOOK_ARG_2_NUL(fopen,filename,FILE*,const char*__restrict,filename,const char*__restrict,mode)
//FILE *fopen64(const char *filename, const char *mode)
HOOK_ARG_2_NUL(fopen64,filename,FILE*,const char*__restrict,filename,const char*__restrict,mode)


/******************************/
/*two arguments(special cases)*/
/******************************/
//int rename(const char *old, const char *new);
HOOK_ARG_2_DC(rename,old,new,int,const char*,old,const char*,new)
//int symlink(const char *path1, const char *path2);
HOOK_ARG_2_DC(symlink,path1,path2,int,const char*,path1,const char*,path2)
//int link(const char * path1, const char *path2);
HOOK_ARG_2_DC(link,path1,path2,int,const char *,path1,const char*,path2)


/*****************/
/*three arguments*/
/*****************/
//int chown(const char *path, uid_t owner, gid_t group);
HOOK_ARG_3(chown,path,int,const char *,path,uid_t,owner,gid_t,group)
//ssize_t readlink(const char *path, char *buf, size_t bufsiz);
HOOK_ARG_3(readlink,path,ssize_t,const char*,path,char*,buf,size_t,bufsiz)
//int __xstat(int ver, const char * path, struct stat * stat_buf);
HOOK_ARG_3(__xstat,path,int,int,ver,const char *,path,struct stat*,stat_buf)
//int __xstat64(int ver, const char * path, struct stat64 * stat_buf);
HOOK_ARG_3(__xstat64,path,int,int,ver,const char *,path,struct stat*,stat_buf)


/*****************/
/*four arguments*/
/*****************/
//int openat(int dirfd, const char *pathname, int flags,mode_t mode)
HOOK_ARG_4(openat,pathname,int,int,dirfd,const char*,pathname,int,flags,mode_t,mode);
//int openat(int dirfd, const char *pathname, int flags,mode_t mode)
HOOK_ARG_4(openat64,pathname,int,int,dirfd,const char*,pathname,int,flags,mode_t,mode);

/****************/
/*exec functions*/
/****************/

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
//int execve(const char *pathname, char *const argv[],char *const envp[]);
HOOK_EXEC(execve,pathname,int,const char *pathname, char *const argv[],char *const envp[])
static __attribute__((constructor(101))) void hook_start(){
	if(strncmp("True",getenv("XIAOBYE_DEBUG"),4)==0) debug=1;
	else debug=0;
	if(debug){
		ENTER();
		printf("Start hooking....\n");
		printf("Start loading libc.so.6\n");
	}
}

static __attribute__((destructor)) void hook_stop(){
	if(debug){
		ENTER();
		printf("Stop hooking....\n");
	}
}

