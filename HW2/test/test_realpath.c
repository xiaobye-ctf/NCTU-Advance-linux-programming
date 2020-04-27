#include<stdio.h>
#include<stdlib.h>
#include<limits.h>


int main(int argc,char **argv){
	char buf[PATH_MAX];
	realpath(argv[1],buf);
	printf("%s\n",buf);
}
