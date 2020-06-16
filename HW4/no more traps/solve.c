#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/user.h>
#include<ctype.h>
#include<string.h>

char hex2int(char hex){

	if(hex>='0'&& hex<='9'){
		return hex-'0';
	}else{
		hex = tolower(hex);
		return hex-'a'+10;
	}

}
char* hex2bin(const char* hex,char *buf){
	int len = strlen(hex);
	int i;
	for(i = 0;i<len/2;i++){
		buf[i] = hex2int(hex[2*i])<<4 ;
		buf[i] += hex2int(hex[2*i+1]);
	}
	buf[i]='\0';
	return buf;
}

int main(int argc,char **argv){
	int pid=0;
	int count = 0;
	struct user_regs_struct regs;
	int status=0;
	unsigned char hex[40960];
	unsigned char bin[40960];
	FILE* f;
	unsigned long int code;
	if((pid=fork())==0){
	//child
		ptrace(PTRACE_TRACEME,0,NULL,NULL);
		execve("./no_more_traps",&argv[1],NULL);
	}else if(pid>0){
		f = fopen("./no_more_traps.txt","r");
		fscanf(f,"%s",hex);
		hex2bin(hex,bin);
		ptrace(PTRACE_CONT,pid,NULL,NULL);
		
		while(waitpid(pid,&status,0)>0){
			if(!WIFSTOPPED(status))continue;
			if(ptrace(PTRACE_GETREGS,pid,NULL,&regs)<0){
				perror("PTRACE_GETREGS: ");
				exit(1);
			}
			code = ptrace(PTRACE_PEEKTEXT,pid,regs.rip-1,0);
			ptrace(PTRACE_POKETEXT,pid,regs.rip-1,(code&0xffffffffffffff00)|bin[count]);
			regs.rip = regs.rip-1;
			ptrace(PTRACE_SETREGS,pid,NULL,&regs);
			ptrace(PTRACE_CONT,pid,NULL,NULL);

			count++;
		}
	}else{
		perror("fork: ");
	}
}
