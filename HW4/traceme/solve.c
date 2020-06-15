#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/user.h>
long long get_flag_addr(pid_t pid){
	long long addr;
	char filename[1000];
	sprintf(filename,"/proc/%lld/maps",pid);
	FILE* f = fopen(filename,"r");
	if(f==NULL){
		perror("fopen: ");
		exit(1);
	}
	fscanf(f,"%llx",&addr);
//	printf("%llx\n",addr);
	fclose(f);
	return addr;
}
void get_flag(pid_t pid,long long addr){
		union {
			char flag[8*5];
			long long val[5];
		}buf;
		for(int i = 0 ; i < 5 ; i++){
			buf.val[i] = ptrace(PTRACE_PEEKDATA,pid,addr+0x201b80+i*8,NULL);
		}
		printf("%s\n",buf.flag);
}
int main(int argc,char **argv){
	int pid=0;
	struct user_regs_struct regs;
	int status=0;
	if((pid=fork())==0){
	//child
		ptrace(PTRACE_TRACEME,0,NULL,NULL);
		execve("./traceme",&argv[1],NULL);
	}else if(pid>0){
		int s;
		long long flag;
		if(waitpid(pid,&status,0)<0) perror("waitpid: ");
		flag = get_flag_addr(pid);
		while(s=WIFSTOPPED(status)){
			if(s<0){
				perror("WIFSTOPPED: ");
				exit(1);
			}
			get_flag(pid,flag);
			if(ptrace(PTRACE_SINGLESTEP,pid,NULL,NULL)<0){
				perror("PTRACE_SINGLESTEP: ");
				exit(1);;
			}
			if(waitpid(pid,&status,0)<0) perror("waitpid: ");
		}
	}else{
		perror("fork: ");
	}
}
