#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define BUFSIZE 1024
void cmdline_preprocess(char *target,char *cmdline,int size){
	char *buf = (char*)malloc(size);
	FILE* f;
	int i;
	
	memset(buf,0,size);
	f = fopen(target,"r");
	if(f==NULL)exit(1);

	fread(buf,size-1,1,f);

	for(i=0;i<(size-1);i++){
		if(!(buf[i]|buf[i+1])) break;

		if(buf[i]=='\0') buf[i]=' ';
	}
	strcpy(cmdline,buf);

	fclose(f);
	free(buf);
}

int main(void){
	char buf[BUFSIZE];

	cmdline_preprocess("/proc/26250/cmdline",buf,sizeof(buf));
	printf("%s\n",buf);
	return 0;
}
		
