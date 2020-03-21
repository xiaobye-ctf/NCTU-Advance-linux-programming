#include<stdio.h>
#include<arpa/inet.h>
typedef struct{
    char l_ip[60];
    int l_port;
    char r_ip[60];
    int r_port;
    unsigned long long inode;
}CONN_RECORD;

void hex_to_ipv4(char *,char* );
void hex_to_port(char*,int*);

