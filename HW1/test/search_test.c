#include<sys/types.h>
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
    char buf[200];
	struct dirent* ent;
    DIR* d = opendir("/proc");
	if(d==NULL){
		perror("open /proc: ");
		exit(1);
	}
	while((ent=readdir(d))!=NULL){
		if(ent->d_type==DT_DIR && (reg_find("^[0-9]+$",ent->d_name)==0))
			printf("%s\n",ent->d_name);
	}
	closedir(d);
	return 0;
}

