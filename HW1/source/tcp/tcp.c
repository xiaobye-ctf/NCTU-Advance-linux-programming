#include<stdio.h>
#include"utils.h"
#define BUFFSIZE 1024

void parse_tcp4(CONN_RECORD* conn,char* line){
	char buf[BUFFSIZE];
	char l_i[40],l_p[10],r_i[40],r_p[10];
	int inod;
	sscanf(buf,"%*d:%*[ ]%[0-9A-F]:%[0-9A-F]%*[ ]%[0-9A-F]:%[0-9A-F]%*[ ]%*X%*[ ]%*[^ ]%*[ ]%*[^ ]%*[ ]%*[^ ]%*[ ]%*[^ ]%*[ ]%*[^ ]%*[ ]%d%*[ ]",
		   l_i,
           l_p,
		   r_i,
           r_p,
		   &inod
    );
	//convert local address's format to ip format
	hex_to_ipv4(l_i,conn->l_ip);
	hex_to_port(l_p,&conn->l_port);
	//convert remote address's format to ip format
	hex_to_ipv4(r_i,conn->r_ip);
	hex_to_port(r_p,&conn->r_port);

#ifdef DEBUG
	printf("##################################\n");
	printf("%s\n",line);
	printf("local ip: %s\n",l_i);
	printf("local port: %s\n",l_p);
    printf("remote ip: %s\n",r_i);
    printf("remote port: %s\n",r_p);
    printf("inode: %d\n",inod);
	printf("local addr: %s:%d, remote addr: %s:%d, inode: %d\n",conn->l_ip,conn->l_port,conn->r_ip,conn->r_port,conn->inode);
#endif

}




