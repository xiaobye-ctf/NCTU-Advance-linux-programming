#include <sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include<limits.h>

extern char **environ;
static char new_path[PATH_MAX];
static char *path;
static char * env[4]={NULL,NULL,NULL,NULL};
void config_env(const char * app_root,const char *hook_lib,const char* debug){
	setenv("LD_PRELOAD",hook_lib,1);
	setenv("MY_APP_ROOT",app_root,1);
	setenv("MY_DEBUG",debug,1);//"True" or "False"
}
void exec_cmd(char **argv){
	execvp(argv[optind++],&argv[optind]);
}

int getopt(int argc, char * const argv[],const char *optstring);
int main(int argc,char **argv){
	int pid;
	int stat;
	char *plib = "./sandbox.so";
	char *app_root = "./";
	char *debug = "False";
	char opt;
	while ((opt = getopt(argc, argv, "--p:d:D")) != -1) {
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
	config_env(app_root,plib,debug);
	if((pid=fork())<0){
		perror("fork: ");
		exit(-1);
	}else if(pid > 0){
		wait(&stat);
	}else{
		exec_cmd(argv);
	}

	return 0;
}
