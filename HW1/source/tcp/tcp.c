#include<stdio.h>
#include"utils.h"

void parse_tcp4(CONN_RECORD* conn,char* line){
	char l_i[40],l_p[10],r_i[40],r_p[10];
	unsigned long long inod;
	sscanf(line,"%*d:%*[ ]%[0-9A-F]:%[0-9A-F]%*[ ]%[0-9A-F]:%[0-9A-F]%*[ ]%*X%*[ ]%*[^ ]%*[ ]%*[^ ]%*[ ]%*[^ ]%*[ ]%*[^ ]%*[ ]%*[^ ]%*[ ]%llu%*[ ]",
		   l_i,
           l_p,
		   r_i,
           r_p,
		   &inod
    );
	//convert local address's format from hex to ip format
	hex_to_ipv4(l_i,conn->l_ip);
	hex_to_port(l_p,&conn->l_port);
	//convert remote address's format from hex to ip format
	hex_to_ipv4(r_i,conn->r_ip);
	hex_to_port(r_p,&conn->r_port);
	//convert inode from string to unsigned long long
	conn->inode=inod;
#ifdef DEBUG
	printf("##################################\n");
	printf("%s\n",line);
	printf("local ip: %s\n",l_i);
	printf("local port: %s\n",l_p);
    printf("remote ip: %s\n",r_i);
    printf("remote port: %s\n",r_p);
    printf("inode: %d\n",inod);
	printf("local addr: %s:%d, remote addr: %s:%d, inode: %llu\n",conn->l_ip,conn->l_port,conn->r_ip,conn->r_port,conn->inode);
#endif

}




