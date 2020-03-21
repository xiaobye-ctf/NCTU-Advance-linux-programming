#include<stdio.h>
#include"comm.h"
#include"tcp.h"
#include"utils.h"

#define BUFFSIZE 1024
void tcp(){
	char buf[BUFFSIZE];
	CONN_RECORD conn;
	FILE * f;
	
	/*start parsing "/proc/net/tcp"*/
	f = fopen("/proc/net/tcp","r");
	//deprecate redundant line
	fgets(buf,BUFFSIZE,f);
	
	//parsing every line
	while(fgets(buf,BUFFSIZE,f)!=NULL){
		parse_tcp4(&conn,buf);
		printf("local addr: %s:%d, remote addr: %s:%d, inode: %llu\n",conn.l_ip,conn.l_port,conn.r_ip,conn.r_port,conn.inode);

	}

	fclose(f);	
	/*end parsing "/proc/net/tcp"*/
}


int main(void){
	tcp();	
}
