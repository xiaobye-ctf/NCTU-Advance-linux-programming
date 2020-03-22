#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<dirent.h>
#include<regex.h>
#include<string.h>


int main(void){
    char buf[200];
    char cmdline[1024];
    long size;
    FILE *f; 
	int pid=2166;    
    sprintf(buf,"/proc/%u/comm",pid);

	printf("open %s\n",buf);
    f = fopen(buf,"r");
	perror("fopen: ");
	
	fgets(cmdline,1024,f);
	size = strlen(cmdline);
	cmdline[size-1]='\0';
    fclose(f);

    printf("%s",cmdline);
	return 0;

}
