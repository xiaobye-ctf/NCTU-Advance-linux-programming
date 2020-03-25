#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define BUFSIZE 1024
//The dst string should not longer than dst_size
void argv_concat(char **argv,int argc,char* dst,int dst_size,char *sep){
	int count=0;
	int sep_len=0;
	int i;
	
	//nothing in argv
	if(argc==0)return;

	sep_len = strlen(sep);
	for(i=0;i<argc;i++){
		count+=strlen(argv[i]);
	}
	if((count+sep_len*(argc-1))>dst_size){
		fprintf(stderr,"Concat arguments failed!\n");
		exit(1);
	}


	strcpy(dst,argv[0]);
	for(i=1;i<argc;i++){
		strcat(dst,sep);
		strcat(dst,argv[i]);
	}
}
int main(int argc,char **argv){
	char buf[BUFSIZE];
	argv_concat(argv,argc,buf,BUFSIZE," ");
	printf("%s\n",buf);
	return 0;

}
