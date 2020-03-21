#include<stdio.h>

int main(void){
	unsigned long long a;
	sscanf("FFFFFFFFFFFFFFFF","%llX",&a);
	printf("%llu\n",a);
}
