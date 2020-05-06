#include <sys/stat.h>
#include<dlfcn.h>
#include <dirent.h>
#include <sys/types.h>
#include<stdio.h>
#include<limits.h>
#include<stdlib.h>
#include<string.h>
#include"utils.h"
#include"comm.h"

int debug=0;
static void *libc_handle;
static void hook_stop() __attribute__((destructor));
static void hook_start() __attribute__((constructor));


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
