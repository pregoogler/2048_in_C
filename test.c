#include <stdio.h>
#include <time.h>

int main(void){
	time_t t = time(NULL);
	while(1){
	
	printf("%ld\n", t);
	}
	return 0;
}
