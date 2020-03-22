#define BUFSIZE 1024
typedef struct{
    char l_ip[60];
    int l_port;
    char r_ip[60];
    int r_port;
    unsigned long long inode;
}CONN_RECORD;


