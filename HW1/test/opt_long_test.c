#include<stdio.h>
#include<getopt.h>

int main(int argc,char** argv){
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
	if(dif==-1){
			
	}else if(dif==1){
	}

	
}
	
