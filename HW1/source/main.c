#include<stdio.h>
#include<getopt.h>
#include"comm.h"
#include"tcp.h"
#include"udp.h"
#include"utils.h"

void print_title(){
	printf("%-5s %-45s %-45s %-50s\n",
			"Proto",
			"Local Address",
			"Foreign Address",
			"PID/Program name and arguments"
	);
}
//if version is ' ',then it is ipv4
void print_info(char* proto,char version,CONN_RECORD *conn,char * pid_cmdline){
	char l_ip_p[100]={'\0'};
	char r_ip_p[100]={'\0'};
	char proto_v[10]={'\0'};

	sprintf(l_ip_p,"%s:%d",conn->l_ip,conn->l_port);
	sprintf(r_ip_p,"%s:%d",conn->r_ip,conn->r_port);
	sprintf(proto_v,"%s%c",proto,version);
	printf("%-5s %-45s %-45s %-50s\n",
			proto_v,
			l_ip_p,
			r_ip_p,
			pid_cmdline
	);
}

void udp(char **argv,int argc){
	char buf[BUFSIZE]={'\0'};
	char pid_cmdline[2*BUFSIZE]={'\0'};
	char pattern[2*BUFSIZE]={'\0'};
	CONN_RECORD conn;
	FILE * f;
	int pid=-1;
	printf("UDP\n");

	//concatenate argv with space
	argv_concat(argv,argc,pattern,sizeof(pattern)," ");
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
		if(argc!=0){
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
		if(argc!=0){
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

void tcp(char **argv,int argc){
	char buf[BUFSIZE]={'\0'};
	char pid_cmdline[2*BUFSIZE]={'\0'};
	char pattern[2*BUFSIZE]={'\0'};
	CONN_RECORD conn;
	FILE * f;
	int pid=-1;
	printf("TCP\n");
	print_title();
	/*start parsing "/proc/net/tcp"*/
	f = fopen("/proc/net/tcp","r");
	//deprecate redundant line
	fgets(buf,BUFSIZE,f);

	//concatenate argv with space
	argv_concat(argv,argc,pattern,sizeof(pattern)," ");

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
		if(argc!=0){
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
		if(argc!=0){
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
	struct option opts[]={
		{"tcp",no_argument,0,'t'},
		{"udp",no_argument,0,'u'},
		{0    ,0          ,0,0}
	};
	int opt_index,c,dif;
	int t=0,u=0;
	while((c=getopt_long(argc,argv,"tu",opts,&opt_index))!=-1){
	//	printf("c: %c, opt_index: %d\n",c,opt_index);
		switch(c){
			case 't':
				t=1;
				break;
			case 'u':
				u=1;
				break;
			default:
				break;
		}
	}

	argc-=optind;
	argv+=optind;
#ifdef DEBUG
	if(argv[0]!=NULL)
		printf("%s\n",argv[0]);	
	else
		printf("no pattern");
#endif
	dif=t-u;
	if(dif==1){
		tcp(argv,argc);
	}else if(dif==-1){
		udp(argv,argc);
	}else{
		tcp(argv,argc);
		puts("");
		udp(argv,argc);
	}
	return 0;
}
