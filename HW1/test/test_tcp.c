#include<stdio.h>
//example
//sl  local_address rem_address   st tx_queue rx_queue tr tm->when retrnsmt   uid  timeout inode
//0: 0100007F:18EB 00000000:0000 0A 00000000:00000000 00:00000000 00000000     0        0 19586 1 000000002f386331 100 0 0 10 0

int main(void){
	char buf[1024];
	int inod;
	char l_ip[33],l_port[33],r_ip[33],r_port[33];
	FILE* f = fopen("/proc/net/tcp","r");

	fgets(buf,sizeof(buf),f);
	printf("%s\n",buf);

	fgets(buf,sizeof(buf),f);
	printf("%s\n\n\n",buf);

	
	sscanf(buf,"%*d:%*[ ]%[0-9A-F]:%[0-9A-F]%*[ ]%[0-9A-F]:%[0-9A-F]%*[ ]%*X%*[ ]%*[^ ]%*[ ]%*[^ ]%*[ ]%*[^ ]%*[ ]%*[^ ]%*[ ]%*[^ ]%*[ ]%d%*[ ]",
		   l_ip,
		   l_port,r_ip,
		   r_port,&inod
	);
	printf("local ip: %s\n",l_ip);
	printf("local port: %s\n",l_port);
	printf("remote ip: %s\n",r_ip);
	printf("remote port: %s\n",r_port);
	printf("inode: %d\n",inod);


	return 0;
}


