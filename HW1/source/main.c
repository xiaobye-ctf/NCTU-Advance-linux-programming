#include<stdio.h>
#include"comm.h"
#include"tcp.h"
#include"udp.h"
#include"utils.h"
void print_info(CONN_RECORD *conn,char * pid_cmdline){
	printf("local addr: %s:%d, remote addr: %s:%d, inode: %llu, pid/cmdline: %s\n",
		    conn->l_ip,
		    conn->l_port,
			conn->r_ip,
			conn->r_port,
			conn->inode,
			pid_cmdline
	);
}

void udp(char *pattern){
	char buf[BUFSIZE];
	char pid_cmdline[2*BUFSIZE];
	CONN_RECORD conn;
	FILE * f;
	int pid=-1;
	printf("UDP\n\n");
	/*start parsing "/proc/net/udp"*/
	f = fopen("/proc/net/udp","r");
	//deprecate redundant line
	fgets(buf,BUFSIZE,f);
	
	//parsing every line
	while(fgets(buf,BUFSIZE,f)!=NULL){
		parse_udp4(&conn,buf);
		pid = search_proc_by_inode(conn.inode);

		//if find corresponding pid,then get cmdline
		if(pid!=-1){
			if(get_cmdline(pid,buf)==NULL){
				sprintf(pid_cmdline,"%u/-",pid);
			}else{
				sprintf(pid_cmdline,"%u/%s",pid,buf);
			}
		}else{
			sprintf(pid_cmdline,"-/-");
		}

		//matching and print
		if(pattern!=NULL){
			if(reg_find(pattern,pid_cmdline)==0){
				print_info(&conn,pid_cmdline);
			}
		}else{
			print_info(&conn,pid_cmdline);
		}


	}

	fclose(f);	
	/*end parsing "/proc/net/udp"*/


	printf("\n\nUDP6\n\n");
	/*start parsing "/proc/net/udp6"*/
	f = fopen("/proc/net/udp6","r");
	//deprecate redundant line
	fgets(buf,BUFSIZE,f);
	
	//parsing every line
	while(fgets(buf,BUFSIZE,f)!=NULL){
		parse_udp6(&conn,buf);
		pid = search_proc_by_inode(conn.inode);
		//if find corresponding pid,then get cmdline
		if(pid!=-1){
			if(get_cmdline(pid,buf)==NULL){
				sprintf(pid_cmdline,"%d/-",pid);
			}else{
				sprintf(pid_cmdline,"%d/%s",pid,buf);
			}
		}else{
			sprintf(pid_cmdline,"-/-");
		}

		//matching
		if(pattern!=NULL){
			if(reg_find(pattern,pid_cmdline)==0){
				print_info(&conn,pid_cmdline);
			}
		}else{
			print_info(&conn,pid_cmdline);
		}


	}

	fclose(f);	
	/*end parsing "/proc/net/udp6"*/

}

void tcp(char *pattern){
	char buf[BUFSIZE];
	char pid_cmdline[2*BUFSIZE];
	CONN_RECORD conn;
	FILE * f;
	int pid=-1;
	printf("TCP\n\n");
	/*start parsing "/proc/net/tcp"*/
	f = fopen("/proc/net/tcp","r");
	//deprecate redundant line
	fgets(buf,BUFSIZE,f);
	
	//parsing every line
	while(fgets(buf,BUFSIZE,f)!=NULL){
		parse_tcp4(&conn,buf);
		pid = search_proc_by_inode(conn.inode);

		//if find corresponding pid,then get cmdline
		if(pid!=-1){
			if(get_cmdline(pid,buf)==NULL){
				sprintf(pid_cmdline,"%u/-",pid);
			}else{
				sprintf(pid_cmdline,"%u/%s",pid,buf);
			}
		}else{
			sprintf(pid_cmdline,"-/-");
		}

		//matching and print
		if(pattern!=NULL){
			if(reg_find(pattern,pid_cmdline)==0){
				print_info(&conn,pid_cmdline);
			}
		}else{
			print_info(&conn,pid_cmdline);
		}


	}

	fclose(f);	
	/*end parsing "/proc/net/tcp"*/


	printf("\n\nTCP6\n\n");
	/*start parsing "/proc/net/tcp6"*/
	f = fopen("/proc/net/tcp6","r");
	//deprecate redundant line
	fgets(buf,BUFSIZE,f);
	
	//parsing every line
	while(fgets(buf,BUFSIZE,f)!=NULL){
		parse_tcp6(&conn,buf);
		pid = search_proc_by_inode(conn.inode);
		//if find corresponding pid,then get cmdline
		if(pid!=-1){
			if(get_cmdline(pid,buf)==NULL){
				sprintf(pid_cmdline,"%d/-",pid);
			}else{
				sprintf(pid_cmdline,"%d/%s",pid,buf);
			}
		}else{
			sprintf(pid_cmdline,"-/-");
		}

		//matching
		if(pattern!=NULL){
			if(reg_find(pattern,pid_cmdline)==0){
				print_info(&conn,pid_cmdline);
			}
		}else{
			print_info(&conn,pid_cmdline);
		}


	}

	fclose(f);	
	/*end parsing "/proc/net/tcp6"*/

}


int main(int argc,char**argv){
	if(argc==2){
		tcp(argv[1]);	
		udp(argv[1]);
	}else{
		tcp(NULL);
		udp(NULL);
	}
}
