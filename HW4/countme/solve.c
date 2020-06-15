#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/user.h>
int main(int argc,char **argv){
	int pid=0;
	struct user_regs_struct regs;
	int status=0;
	if((pid=fork())==0){
	//child
		ptrace(PTRACE_TRACEME,0,NULL,NULL);
		execve("./countme",&argv[1],NULL);
	}else if(pid>0){
		int s;
		long int count=0;
		if(waitpid(pid,&status,0)<0) perror("waitpid: ");
		while(s=WIFSTOPPED(status)){
			if(s<0){
				perror("WIFSTOPPED: ");
				exit(1);
			}
			count++;
			if(ptrace(PTRACE_SINGLESTEP,pid,NULL,NULL)<0){
				perror("PTRACE_SINGLESTEP: ");
				exit(1);;
			}
			if(waitpid(pid,&status,0)<0) perror("waitpid: ");
		}
		printf("ASM{%ld}\n",count);
	}else{
		perror("fork: ");
	}
}
