#include<stdio.h>
#include<stdlib.h>
#include<limits.h>


int main(int argc,char **argv){
	char buf[20000];
	realpath(argv[1],buf);
	perror("");
	printf("%s\n",buf);
}
