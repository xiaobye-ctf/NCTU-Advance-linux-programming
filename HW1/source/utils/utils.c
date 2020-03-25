#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<dirent.h>
#include<regex.h>
#include<string.h>
#include"comm.h"
#include<unistd.h>
void hex_to_ipv4(char *hex,char* str_ip){
    struct in_addr addr;
    sscanf(hex,"%X",&addr.s_addr);
    char *tmp = inet_ntoa(addr);
    sscanf(tmp,"%s",str_ip);
}
void hex_to_ipv6(char *hex,char* str_ip){
	char str_ip6[4][9];
	char tmp[60]={'\0'};
	int *i_ptr;
    struct in6_addr addr;
	int i;

	i_ptr = (int*)&addr.s6_addr;
	
	sscanf(hex,"%8X%8X%8X%8X",&i_ptr[0],&i_ptr[1],&i_ptr[2],&i_ptr[3]);

    inet_ntop(AF_INET6,&addr,tmp,100);

    sscanf(tmp,"%s",str_ip);
}
void hex_to_port(char* hex,int* int_port){
    sscanf(hex,"%X",int_port);  
}

int reg_find(char* pattern,char* target){
	regex_t reg;
	int nmatch=1;
	regmatch_t matched[1];
		
	regcomp(&reg,pattern,REG_EXTENDED);
	return regexec(&reg,target,nmatch,matched,0);
}

//if pid = -1,then it means that nothing is found
int search_proc_by_inode(unsigned long long inode){
    DIR* d;
	DIR* d_fd;
	int n;
    char path[200];
	char buf3[200];
	char buf2[200];
	char buf1[BUFSIZE];
	struct dirent* ent;
	struct dirent* ent_fd;
	int pid=-1;
	int find=0;
	//open "/proc"
	d = opendir("/proc");
	if(d==NULL){
		perror("open /proc: ");
		exit(1);
	}

	//enumerating pids in "/proc"
	while((ent=readdir(d))!=NULL){
		if(ent->d_type==DT_DIR && (reg_find("^[0-9]+$",ent->d_name)==0)){
			sprintf(path,"/proc/%s/fd",ent->d_name);
			//satrt enumerating fds in "/proc/{pid}/fd"
			//check fd access permission if no permission ,then break
			if(eaccess(path,R_OK)!=0)continue;
#ifdef DEBUG
			printf("####################################\n");
			printf("%s\n",ent->d_name);
#endif

			d_fd = opendir(path);
			if(d_fd==NULL){
				perror(path);
				exit(1);
			}
			while((ent_fd=readdir(d_fd))!=NULL){
				sprintf(path,"/proc/%s/fd/%s",ent->d_name,ent_fd->d_name);
#ifdef DEBUG
				printf("%s\n",path);
#endif
				//read the content of link and check if it is socket that we want
				if(eaccess(path,R_OK)!=0 || (ent_fd->d_type!=DT_LNK))continue;
				n = readlink(path,buf1,BUFSIZE);

//				printf("readlink: %d bytes\n",n);
#ifdef DEBUG
				perror("readlink: ");
#endif
				buf1[n]='\0';
				sprintf(buf3,"socket:[%llu]",inode);
				sprintf(buf2,"0000:[%llu]",inode);
				if(strcmp(buf1,buf3)==0 ||(strcmp(buf1,buf2)==0) ){
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

char* get_cmdline(int pid,char* cmdline){
    char path_comm[200]={'\0'};
	char path_cmdline[200]={'\0'};
    FILE *f_comm,*f_cmdline;
	long size;
	char buf[BUFSIZE]={'\0'},buf1[BUFSIZE]={'\0'};
    sprintf(path_comm,"/proc/%u/comm",pid);
	sprintf(path_cmdline,"/proc/%u/cmdline",pid);

    f_comm = fopen(path_comm,"r");
	if(f_comm==NULL){
		perror(path_comm);
		return NULL;
	}
	f_cmdline = fopen(path_cmdline,"r");
	if(f_cmdline==NULL){
		perror(path_comm);
		return NULL;
	}
	fgets(buf,BUFSIZE,f_cmdline);
	sscanf(buf,"%*[^ ]%*[ ]%[^\n]",buf1);


	fgets(buf,BUFSIZE,f_comm);
	size = strlen(buf);
	buf[size-1]='\0';

	sprintf(cmdline,"%s %s",buf,buf1);

	fclose(f_cmdline);
    fclose(f_comm);


	return cmdline;
}
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

