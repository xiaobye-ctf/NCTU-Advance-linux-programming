extern int debug;

#define FILE_OP_DENY(name) printf("[sandbox] %s: access to %s is not allowed\n",__FUNCTION__,name)
#define ENTER() printf("\e[33;1m[Enter %s()]\e[0m\n",__FUNCTION__)

#define LOAD_FUNC(name) \
	if(debug)printf("Loading function %s\n",#name);\
	real_##name = dlsym(libc_handle,#name);\
	if(debug && real_##name == NULL)\
		fprintf(stderr, "\e[5;31m%s\e[0m\n",dlerror());\

#define HOOK_VAR(func)   static void* real_##func = NULL;
#define MAKEFUNC(func,ret,...) ret (*f)(__VA_ARGS__) = (ret (*)(__VA_ARGS__)) real_##func;
//func: target funciton
//path: path to validate
//ret: type of return value 
//arg1: type of arg1
//a1: function's first arguments
#define HOOK_ARG_1(func,path,ret,arg1,a1)\
	HOOK_VAR(func)\
		ret func(arg1 a1){ \
			if(debug)ENTER();\
			MAKEFUNC(func,ret,arg1)\
			if(valid_access(path)){ \
				return f(a1); \
			}else{ \
				FILE_OP_DENY(path); \
			} \
		}
#define HOOK_ARG_2(func,path,ret,arg1,a1,arg2,a2)\
	HOOK_VAR(func)\
		ret func(arg1 a1,arg2 a2){ \
			if(debug)ENTER();\
			MAKEFUNC(func,ret,arg1,arg2)\
			if(valid_access(path)){ \
				return f(a1,a2); \
			}else{ \
				FILE_OP_DENY(path); \
			} \
		}
//DC means Double Check.
//Check for what?Both two path argument.
#define HOOK_ARG_2_DC(func,path1,path2,ret,arg1,a1,arg2,a2)\
	HOOK_VAR(func)\
		ret func(arg1 a1,arg2 a2){ \
			if(debug)ENTER();\
			MAKEFUNC(func,ret,arg1,arg2)\
			if(valid_access(path1) && valid_access(path2)){ \
				return f(a1,a2); \
			}else{ \
				FILE_OP_DENY(path1); \
			} \
		}

#define HOOK_ARG_3(func,path,ret,arg1,a1,arg2,a2,arg3,a3)\
	HOOK_VAR(func)\
		ret func(arg1 a1,arg2 a2,arg3 a3){ \
			if(debug)ENTER();\
			MAKEFUNC(func,ret,arg1,arg2,arg3)\
			if(valid_access(path)){ \
				return f(a1,a2,a3); \
			}else{ \
				FILE_OP_DENY(path); \
			} \
		}
#define HOOK_ARG_4(func,path,ret,arg1,a1,arg2,a2,arg3,a3,arg4,a4)\
	HOOK_VAR(func)\
		ret func(arg1 a1,arg2 a2,arg3 a3,arg4 a4){ \
			if(debug)ENTER();\
			MAKEFUNC(func,ret,arg1,arg2,arg3,arg4)\
			if(valid_access(path)){ \
				return f(a1,a2,a3,a4); \
			}else{ \
				FILE_OP_DENY(path); \
			} \
		}

