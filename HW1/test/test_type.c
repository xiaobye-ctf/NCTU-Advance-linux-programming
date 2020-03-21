#include<stdio.h>

int main(void){
	unsigned long long a;
	sscanf("FFFFFFFFFFFFFFFF","%lX",&a);
	printf("%llu\n",a);
}
