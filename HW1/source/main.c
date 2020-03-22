#include<stdio.h>
#include"comm.h"
#include"tcp.h"
#include"udp.h"
#include"utils.h"
void print_title(){
	printf("%-5s %-39s %-39s %-50s\n",
			"Proto",
			"Local Address",
			"Foreign Address",
			"PID/Program name and arguments"
	);
}
//if version is ' ',then it is ipv4
void print_info(char* proto,char version,CONN_RECORD *conn,char * pid_cmdline){
	char l_ip_p[100];
	char r_ip_p[100];
	char proto_v[10];

	sprintf(l_ip_p,"%s:%d",conn->l_ip,conn->l_port);
	sprintf(r_ip_p,"%s:%d",conn->r_ip,conn->r_port);
	sprintf(proto_v,"%s%c",proto,version);
	printf("%-5s %-39s %-39s %-50s\n",
			proto_v,
			l_ip_p,
			r_ip_p,
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
	print_title();
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
				print_info("udp",' ',&conn,pid_cmdline);
			}
		}else{
			print_info("udp",' ',&conn,pid_cmdline);
		}


	}

	fclose(f);	
	/*end parsing "/proc/net/udp"*/


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
				print_info("udp",'6',&conn,pid_cmdline);
			}
		}else{
			print_info("udp",'6',&conn,pid_cmdline);
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
	print_title();
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
				print_info("tcp",' ',&conn,pid_cmdline);
			}
		}else{
			print_info("tcp",' ',&conn,pid_cmdline);
		}


	}

	fclose(f);	
	/*end parsing "/proc/net/tcp"*/


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
				print_info("tcp",'6',&conn,pid_cmdline);
			}
		}else{
			print_info("tcp",'6',&conn,pid_cmdline);
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
