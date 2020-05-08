
#define FILE_OP_DENY(name) printf("[sandbox] %s: access to %s is not allowed\n",__FUNCTION__,name)
#define ENTER() printf("\e[33;1m[Enter %s()]\e[0m\n",__FUNCTION__)

#define MAKEFUNC(func,ret,...) ret (*real_##func)(__VA_ARGS__) = NULL;
#define LOAD_FUNC(name) \
	if(libc_handle==NULL)libc_handle=dlopen("libc.so.6",RTLD_LAZY);\
	if(real_##name==NULL){\
		if(debug)printf("Loading function %s\n",#name);\
		real_##name = dlsym(libc_handle,#name);\
	}\
	if(debug && real_##name == NULL)fprintf(stderr, "\e[5;31m%s\e[0m\n",dlerror());\

//func: target funciton
//path: path to validate
//ret: type of return value 
//arg1: type of arg1
//a1: function's first arguments
#define HOOK_ARG_1(func,path,ret,arg1,a1)\
		MAKEFUNC(func,ret,arg1)\
		ret func(arg1 a1){ \
			if(debug)ENTER();\
			if(valid_access(path)){ \
				LOAD_FUNC(func)\
				if(real_##func!=NULL)return real_##func(a1); \
			}else{ \
				FILE_OP_DENY(path); \
			} \
			return -1;\
		}
#define HOOK_ARG_1_NUL(func,path,ret,arg1,a1)\
		MAKEFUNC(func,ret,arg1)\
		ret func(arg1 a1){ \
			if(debug)ENTER();\
			if(valid_access(path)){ \
				LOAD_FUNC(func)\
				if(real_##func!=NULL)return real_##func(a1); \
			}else{ \
				FILE_OP_DENY(path); \
			} \
			return 0;\
		}

#define HOOK_ARG_2(func,path,ret,arg1,a1,arg2,a2)\
		MAKEFUNC(func,ret,arg1,arg2)\
		ret func(arg1 a1,arg2 a2){ \
			if(debug)ENTER();\
			if(valid_access(path)){ \
				LOAD_FUNC(func)\
				if(real_##func!=NULL)return real_##func(a1,a2); \
			}else{ \
				FILE_OP_DENY(path); \
			} \
			return -1;\
		}
//Have to return NULL
#define HOOK_ARG_2_NUL(func,path,ret,arg1,a1,arg2,a2)\
		MAKEFUNC(func,ret,arg1,arg2)\
		ret func(arg1 a1,arg2 a2){ \
			if(debug)ENTER();\
			if(valid_access(path)){ \
				LOAD_FUNC(func)\
				if(real_##func!=NULL)return real_##func(a1,a2); \
			}else{ \
				FILE_OP_DENY(path); \
			} \
			return 0;\
		}

//DC means Double Check.
//Check for what?Both two path argument.
#define HOOK_ARG_2_DC(func,path1,path2,ret,arg1,a1,arg2,a2)\
		MAKEFUNC(func,ret,arg1,arg2)\
		ret func(arg1 a1,arg2 a2){ \
			if(debug)ENTER();\
			if(valid_access(path1) && valid_access(path2)){ \
				LOAD_FUNC(func)\
				if(real_##func!=NULL)return real_##func(a1,a2); \
			}else{ \
				FILE_OP_DENY(path1); \
			} \
			return -1;\
		}

#define HOOK_ARG_3(func,path,ret,arg1,a1,arg2,a2,arg3,a3)\
		MAKEFUNC(func,ret,arg1,arg2,arg3)\
		ret func(arg1 a1,arg2 a2,arg3 a3){ \
			if(debug)ENTER();\
			if(valid_access(path)){ \
				LOAD_FUNC(func)\
				if(real_##func!=NULL)return real_##func(a1,a2,a3); \
			}else{ \
				FILE_OP_DENY(path); \
			} \
			return -1;\
		}
#define HOOK_ARG_4(func,path,ret,arg1,a1,arg2,a2,arg3,a3,arg4,a4)\
		MAKEFUNC(func,ret,arg1,arg2,arg3,arg4)\
		ret func(arg1 a1,arg2 a2,arg3 a3,arg4 a4){ \
			if(debug)ENTER();\
			if(valid_access(path)){ \
				LOAD_FUNC(func)\
				if(real_##func!=NULL)return real_##func(a1,a2,a3,a4); \
			}else{ \
				FILE_OP_DENY(path); \
			} \
			return -1;\
		}

#define EXEC_OP_DENY(name) printf("[sandbox] %s(\"%s\"): not allowed\n",__FUNCTION__,name)

#define HOOK_EXEC(func,path,ret,...)\
	ret func(__VA_ARGS__){\
		if(debug)ENTER();\
		EXEC_OP_DENY(path);\
		return -1;\
	}

