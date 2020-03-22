#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<stdlib.h>
#include<dirent.h>
#include<stdio.h>
#include<regex.h>


int reg_find(char* pattern,char* target){
	regex_t reg;
	int nmatch=1;
	regmatch_t matched[1];
		
	regcomp(&reg,pattern,REG_EXTENDED);
	return regexec(&reg,target,nmatch,matched,0);
}

int main(void){
    DIR* d = opendir("/proc");
	DIR* d_fd;
	int n;
    char buf[200];
	char buf1[100];
	struct dirent* ent;
	struct dirent* ent_fd;
	int pid=-1;
	int find=0;
	unsigned long long inode = 30272;
	//open "/proc"
	if(d==NULL){
		perror("open /proc: ");
		exit(1);
	}
	
	//enumerating pids in "/proc"
	while((ent=readdir(d))!=NULL){
		if(ent->d_type==DT_DIR && (reg_find("^[0-9]+$",ent->d_name)==0)){
#ifdef DEBUG
			printf("####################################\n");
			printf("%s\n",ent->d_name);
#endif
			sprintf(buf,"/proc/%s/fd",ent->d_name);
			//satrt enumerating fds in "/proc/{pid}/fd"
			d_fd = opendir(buf);
			if(d_fd==NULL){
				perror(buf);
				exit(1);
			}
			while((ent_fd=readdir(d_fd))!=NULL){
				sprintf(buf,"/proc/%s/fd/%s",ent->d_name,ent_fd->d_name);
#ifdef DEBUG
				printf("%s\n",buf);
#endif
				//read the content of link and check if it is socket that we want
				n = readlink(buf,buf1,sizeof(buf1));
#ifdef DEBUG
				perror("readlink: ");
#endif
				buf1[n]='\0';
				sprintf(buf,"socket:[%llu]",inode);
				if(strcmp(buf1,buf)==0){
#ifdef DEBUG
					printf("find target pid: %s\n",ent->d_name);
#endif 
					sscanf(ent->d_name,"%u",&pid);
					find=1;
					break;
				}
				
			}
			closedir(d_fd);
#ifdef DEBUG
			perror("closedir(d_fd): ");
#endif 
			//end enumerating fds in "/proc/pid/fd"
			if(find) break;	
		}
	}
	closedir(d);
#ifdef DEBUG
			perror("closedir(d): ");
#endif
	return pid;
}


