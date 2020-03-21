#include<stdio.h>
#include<arpa/inet.h>
#include"utils.h"
void hex_to_ipv4(char *hex,char* str_ip){
    struct in_addr addr;
    sscanf(hex,"%X",&addr.s_addr);
    char *tmp = inet_ntoa(addr);
    sscanf(tmp,"%s",str_ip);
}
void hex_to_port(char* hex,int* int_port){
    sscanf(hex,"%X",int_port);  
}

