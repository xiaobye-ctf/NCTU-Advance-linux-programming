#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<dirent.h>
#include<regex.h>
#include<string.h>
void hex_to_ipv6(char *hex,char* str_ip){
	char str_ip6[4][9];
	char tmp[60];
	int *i_ptr;
    struct in6_addr addr;
	int i;

	i_ptr = (int*)&addr.s6_addr;
	
	sscanf(hex,"%8X%8X%8X%8X",&i_ptr[0],&i_ptr[1],&i_ptr[2],&i_ptr[3]);
	for(i=0;i<4;i++){
		printf("%X\n",i_ptr[i]);
	}
    inet_ntop(AF_INET6,&addr,tmp,100);

    sscanf(tmp,"%s",str_ip);
}

int main(void){
	char ipv6[100];
	printf("0000000000000000FFFF0000BF00A8C0\n");
	hex_to_ipv6("0000000000000000FFFF0000BF00A8C0",ipv6);
	printf("%s\n",ipv6);
}

