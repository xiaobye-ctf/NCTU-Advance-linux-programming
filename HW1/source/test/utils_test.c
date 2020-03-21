#include<stdio.h>
#include<arpa/inet.h>
void hex_to_ipv4(char *hex,char* str_ip){
	struct in_addr addr;
	sscanf(hex,"%X",&addr.s_addr);
	char *tmp = inet_ntoa(addr);
	sscanf(tmp,"%s",str_ip);
}
void hex_to_port(char* hex,int* int_port){
	sscanf(hex,"%X",int_port);	
}

int main(void){
	char ip[20];
	int port;
	hex_to_ipv4("0100007F",ip);
	hex_to_port("18EB",&port);
	printf("local address: %s:%hu\n",ip,port);

	hex_to_ipv4("00000000",ip);
	hex_to_port("0000",&port);
	printf("remote address: %s:%hu\n",ip,port);
	return 0;
}
