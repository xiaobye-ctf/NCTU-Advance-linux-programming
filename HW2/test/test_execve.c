#include<unistd.h>
extern char **environ;

int main(void){
	char* const arg[]={"-a",NULL};
//	execve("/bin/ls",arg,environ);
	execvp("ls",arg);
}
