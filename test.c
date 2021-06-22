#include <stdio.h>
#include <time.h>

typedef struct GAMER{
    char name[20];
    int isSuccess;
    int moveCount;
    int comboCount;
    int time;
} gamer;
int main(void){
	printf("%d", sizeof(gamer));
	return 0;
}
