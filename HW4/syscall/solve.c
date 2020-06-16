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
	int entry=1;
	if((pid=fork())==0){
	//child
		ptrace(PTRACE_TRACEME,0,NULL,NULL);
		execve("./syscall",&argv[1],NULL);
	}else if(pid>0){
		long int count=0;
		if(waitpid(pid,&status,0)<0) perror("waitpid: ");
		if(ptrace(PTRACE_SETOPTIONS,pid,NULL,PTRACE_O_EXITKILL|PTRACE_O_TRACESYSGOOD)){
			perror("PTRACE_SETOPTIONS: ");
			exit(1);
		}
		
		while(WIFSTOPPED(status)){
			
			if(WSTOPSIG(status)&0x80){
				if(entry){
					count++;
				}
				entry^=1;
			}
			if(ptrace(PTRACE_SYSCALL,pid,NULL,NULL)<0){
				perror("PTRACE_SYSCALL: ");
				exit(1);;
			}
			if(waitpid(pid,&status,0)<0) perror("waitpid: ");
		}
		printf("ASM{%ld}\n",count);
	}else{
		perror("fork: ");
	}
}
