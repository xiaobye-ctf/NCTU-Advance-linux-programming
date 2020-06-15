#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<ctype.h>
#include<capstone/capstone.h>
int remote(const char* a,int p){
	int sockfd = 0;
	struct sockaddr_in serv_addr;
	if((sockfd = socket(AF_INET,SOCK_STREAM,0))<0){
		perror("socket: ");
		exit(1);
	}
	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(p);
	serv_addr.sin_addr.s_addr = inet_addr(a);
	

	if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0){
		perror("connect: ");
		exit(1);
	}

	return sockfd;
}
char hex2int(char hex){
	
	if(hex>='0'&& hex<='9'){
		return hex-'0';
	}else{
		hex = tolower(hex);
		return hex-'a'+10;
	}
	
}
char* hex2bin(const char* hex,char *buf){
	int len = strlen(hex);
	int i;
	for(i = 0;i<len/2;i++){
		buf[i] = hex2int(hex[2*i])<<4 ;
		buf[i] += hex2int(hex[2*i+1]);
	}
	buf[i]='\0';
	return buf;
}
void disas(const char* bin,char *buf){
	csh handle;
	cs_insn *insn;
	size_t count;
	char tmp[1024];
	if (cs_open(CS_ARCH_X86, CS_MODE_64, &handle) != CS_ERR_OK)exit(1);
	count = cs_disasm(handle, bin, strlen(bin), 0, 0, &insn);
	if (count > 0) {
		size_t j;
		for (j = 0; j < count; j++) {
			sprintf(tmp,"%s %s\n",insn[j].mnemonic,insn[j].op_str);
			strcat(buf,tmp);
		}
		cs_free(insn, count);
	} else
		printf("ERROR: Failed to disassemble given code!\n");

	cs_close(&handle);

}
void bin2hex(const char *bin,char *buf){
	char map[17]="0123456789abcdef";
	int len = strlen(bin);
	int i;
	for(i=0;i<len;i++){
		int val = bin[i];
		buf[2*i]=map[val>>4];
		buf[2*i+1]=map[val%16];
	}
	buf[2*i]='\0';
}
int main(void){
	char hex[4096];
	char bin[4096]={'\0'};
	char ans[4096]={'\0'};
	char hex_ans[4096]={'\0'};
	char flag[4096];
	int sockfd = remote("140.113.213.214",2530);
	FILE *rx = fdopen(sockfd, "r");
	FILE *tx = fdopen(sockfd,"w");
	int c=0;
	while(c!=10){
		fscanf(rx,"%[^>]\n>>> ",&hex);
		fscanf(rx,"%s",&hex);
		fread(flag,strlen("Your answer: "),1,rx);	

		hex2bin(hex,bin);
		disas(bin,ans);
		bin2hex(ans,hex_ans);

		//printf("%s",ans);
		//printf("%s\n",hex_ans);

		fwrite(hex_ans,strlen(hex_ans),1,tx);
		fwrite("\n",1,1,tx);
		fflush(tx);
		fscanf(rx,"%[^=]",flag);

		printf("%s\n",flag);	
		ans[0]='\0';
		c++;
	}
	fclose(rx);
	fclose(tx);
}
