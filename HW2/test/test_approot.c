#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<string.h>
char app_root[]="./";
char app_real_root[PATH_MAX];
int app_real_root_len;
int main(int argc,char **argv){
	char buf[PATH_MAX];
	int buf_len;
	realpath(app_root,app_real_root);
	realpath(argv[1],buf);
	printf("%s\n",app_real_root);
	printf("%s\n",buf);
	
	buf_len = strlen(buf);
	app_real_root_len = strlen(app_real_root);
	if(buf_len>=app_real_root_len  && strncmp(app_real_root,buf,app_real_root_len)==0){
		printf("Valid access\n");
	}else{
		
		printf("Invalid access\n");
	}
}
