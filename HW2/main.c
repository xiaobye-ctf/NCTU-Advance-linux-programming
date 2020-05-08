#include <sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include<limits.h>
#include<dlfcn.h>

static char new_path[PATH_MAX];
static char *path;
void  exec_cmd(int argc,char **argv,const char * app_root,const char *hook_lib,const char* debug){
	char cmd[10000];
	char tmp[10000];
	if(argc==optind){
		fprintf(stderr,"no command given.\n");
		exit(1);
	}
	setenv("XIAOBYE_SANDBOX_ROOT",app_root,1);
	setenv("XIAOBYE_DEBUG",debug,1);//"True" or "False"
#ifdef DEBUG
	printf("XIAOBYE_SANDBOX_ROOT: %s\n",getenv("XIAOBYE_SANDBOX_ROOT"));
	printf("XIAOBYE_DEBUG: %s\n",getenv("XIAOBYE_DEBUG"));
#endif
	snprintf(cmd,10000,"LD_PRELOAD=%s",hook_lib);
	for(;optind<argc;optind++){
		snprintf(tmp,10000,"%s %s",cmd,argv[optind]);
		snprintf(cmd,10000,"%s",tmp);
	}
	
#ifdef DEBUG
	printf("cmd: %s\n",cmd);
#endif
	system(cmd);
}

int main(int argc,char **argv){
	char *plib = "./sandbox.so";
	char *app_root = "./";
	char *debug = "False";
	char opt;
	while ((opt = getopt(argc, argv, "p:d:D")) != -1) {
        switch (opt) {
			case 'p':
				plib = optarg;
                break;
            case 'd':
                app_root = optarg;
                break;
			case 'D':
				debug = "True";
				break;
            default: 
				fprintf(stderr, "usage: ./sandbox [-p sopath] [-d basedir] [--] cmd [cmd args ...]\n");
				fprintf(stderr, "        -p: set the path to sandbox.so, default = ./sandbox.so\n");
				fprintf(stderr, "        -d: the base directory that is allowed to access, default = .\n");
				fprintf(stderr, "        -D: Display debug information\n");
				fprintf(stderr, "        --: separate the arguments for sandbox and for the executed command\n");


                exit(EXIT_FAILURE);
        }
    }
	exec_cmd(argc,argv,app_root,plib,debug);

	return 0;
}
