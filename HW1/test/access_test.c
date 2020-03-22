#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<dirent.h>
#include<regex.h>
#include<string.h>
#include<unistd.h>
int main(void){
	char buf[100];
	if(eaccess("/proc/2166/fd/43",R_OK)!=0)printf("Permission deny!\n");
	perror("");
	readlink("/proc/2166/fd/43",buf,100);
	printf(buf);
	return 0;
}
