#include<stdio.h>
#include"comm.h"
#include"tcp.h"
#include"utils.h"

#define BUFFSIZE 1024
void tcp(){
	char buf[BUFFSIZE];
	CONN_RECORD conn;
	FILE * f;
	int pid;
	
	/*start parsing "/proc/net/tcp"*/
	f = fopen("/proc/net/tcp","r");
	//deprecate redundant line
	fgets(buf,BUFFSIZE,f);
	
	//parsing every line
	while(fgets(buf,BUFFSIZE,f)!=NULL){
		parse_tcp4(&conn,buf);
		pid = search_proc_by_inode(conn.inode);
		printf("local addr: %s:%d, remote addr: %s:%d, inode: %llu, pid:%d\n",
			    conn.l_ip,
			    conn.l_port,
				conn.r_ip,
				conn.r_port,
				conn.inode,
				pid
		);

	}

	fclose(f);	
	/*end parsing "/proc/net/tcp"*/
}


int main(void){
	tcp();	
}
