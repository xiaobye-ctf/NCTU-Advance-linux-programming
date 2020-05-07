#include <string.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "comm.h"
//validate access right
int valid_access(const char* target){
	char* root;
	char real_root[PATH_MAX];
	char real_target[PATH_MAX];
	int len_real_root;
	int len_real_target;
	if(debug) ENTER();
	root = getenv("MY_APP_ROOT");
	if(root==NULL){
		return 1;
	}

	realpath(root,real_root);
	realpath(target,real_target);
	len_real_root = strlen(real_root);
	len_real_target = strlen(real_target);
	if(debug){
		printf("\e[32;1mroot: %s, real root: %s\e[0m\n",root,real_root);
		printf("\e[32;1mtarget: %s, target root: %s\e[0m\n",target,real_target);
	}
	if(len_real_target>=len_real_root && strncmp(real_root,real_target,len_real_root)==0){
		return 1;	
	}else{
		return 0;
	}
}


