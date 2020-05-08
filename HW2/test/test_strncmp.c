#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main(void){
	if(strncmp("/","/",1)==0)printf("allow\n");
	else printf("deny\n");

}
