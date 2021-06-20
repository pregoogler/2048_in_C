#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>

int board[5][5] = {
        {2, 8,  2,  4,  0},
        {8, 16, 32, 64, 128},
        {16, 32,  64,  128,  256},
        {32, 64,  128,  256,  512},
        {64, 128,  256,  512,  1024}
};      //게임 판

int score = 0;

int getch(void)
{
    struct termios oldt, newt;
    int ch;
    tcgetattr( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    return ch;
}   //맥에서 getch 사용할 수 있도록

void printMenu(){
    for(int i = 0;i < 25; ++i){
        printf("-");
    }
    printf("\n1.\tGameStart");
    printf("\n2.\tHow to play");
    printf("\n3.\tRanking");
    printf("\n4.\tExit");
    printf("\n");
    for(int i = 0; i < 25; ++i){
        printf("-");
    }
    printf("\n>> ");
}   //메뉴 출력

void printBoard(){
    for(int i = 0; i < 5; ++i){
        for(int j = 0; j < 5; ++j){
            printf("%d\t", board[i][j]);
        }
        printf("\n");
    }
}   //게임판 출력
void boardInit(){
    for(int i = 0; i < 5; ++i){
        for(int j = 0; j < 5; ++j){
            board[i][j] = 0;
        }
    }
}   //게임판 초기화

int ifGameOver(){
    for(int i = 0; i < 5; ++i){
        for(int j = 0; j < 4; ++j){
            if(board[i][j] == board[i][j+1])
                return 0;
        }
    }
    for(int i = 0; i < 4; ++i){
        for(int j = 0; j < 5; ++j){
            if(board[i][j] == board[i+1][j])
                return 0;
        }
    }
    return 1;
}   //게임이 끝나면 0 리턴, 안 끝났으면 1리턴

int slideUp(){
    int i = 1;
    int j = 0;
    int stat = 0;
    for(i = 1; i < 5; ++i){
        for(j = 0; j < 5; ++j){
            //printf("%d %d\n", i, j);
            if((board[i-1][j] == 0) && (board[i][j] != 0)){
                board[i-1][j] = board[i][j];
                board[i][j] = 0;
                i = 1; j = -1;
                stat = 1;
                continue;
            }
        }
    }
    for(i = 1; i < 5; ++i){
        for(j = 0; j < 5; ++j){
            //printf("%d %d\n", i, j);
            if((board[i-1][j] == board[i][j]) && (board[i][j] != 0)){
                board[i-1][j] *= 2;
                score += board[i-1][j];
                board[i][j] = 0;
                stat = 2;

                continue;
            }
        }
    }
    for(i = 1; i < 5; ++i){
        for(j = 0; j < 5; ++j){
            //printf("%d %d\n", i, j);
            if((board[i-1][j] == 0) && (board[i][j] != 0)){
                board[i-1][j] = board[i][j];
                board[i][j] = 0;
                i = 1; j = -1;
                continue;
            }
        }
    }
    return stat;
}
int slideDown(){
    int i = 0;
    int j = 0;
    int stat = 0;
    for(i = 0; i < 4; ++i){
        for(j = 0; j < 5; ++j){
            if(board[i+1][j] == 0 && board[i][j] != 0){
                board[i+1][j] = board[i][j];
                board[i][j] = 0;
                i = 0; j = -1;
                stat = 1;
                continue;
            }
        }
    }
    for(i = 1; i < 5; ++i){
        for(j = 0; j < 5; ++j){
            if(board[i-1][j]==board[i][j] && board[i][j] != 0){
                board[i-1][j] *= 2;
                score += board[i-1][j];
                board[i][j] = 0;
                stat = 2;
                continue;
            }
        }
    }
    for(i = 0; i < 4; ++i){
        for(j = 0; j < 5; ++j){
            if(board[i+1][j] == 0 && board[i][j] != 0){
                board[i+1][j] = board[i][j];
                board[i][j] = 0;
                i = 0; j = -1;
                continue;
            }
        }
    }
    return stat;
}
int slideRight(){
    int i = 0;
    int j = 0;
    int stat = 0;
    for(i = 0; i < 5; ++i){
        for(j = 0; j < 4; ++j){
            if(board[i][j+1] == 0 && board[i][j] != 0){
                board[i][j+1] = board[i][j];
                board[i][j] = 0;
                i = 0; j = -1;
                stat = 1;
                continue;
            }
        }
    }
    for(i = 0; i < 5; ++i){
        for(j = 1; j < 5; ++j){
            if(board[i][j-1]==board[i][j] && board[i][j] != 0){
                board[i][j-1] *= 2;
                score += board[i][j-1];
                board[i][j] = 0;
                stat = 2;
                continue;
            }
        }
    }
    for(i = 0; i < 5; ++i){
        for(j = 0; j < 4; ++j){
            if(board[i][j+1] == 0 && board[i][j] != 0){
                board[i][j+1] = board[i][j];
                board[i][j] = 0;
                i = 0; j = -1;
                continue;
            }
        }
    }
    return stat;
}
int slideLeft(){
    int i = 0;
    int j = 1;
    int stat = 0;
    for(i = 0; i < 5; ++i){
        for(j = 1; j < 5; ++j){
            if(board[i][j-1] == 0 && board[i][j] != 0){
                board[i][j-1] = board[i][j];
                board[i][j] = 0;
                i = 0; j = 0;
                stat = 1;
                continue;
            }
        }
    }
    for(i = 0; i < 5; ++i){
        for(j = 1; j < 5; ++j){
            if(board[i][j-1]==board[i][j] && board[i][j] != 0){
                board[i][j-1] *= 2;
                score += board[i][j-1];
                board[i][j] = 0;
                stat = 2;
                continue;
            }
        }
    }
    for(i = 0; i < 5; ++i){
        for(j = 1; j < 5; ++j){
            if(board[i][j-1] == 0 && board[i][j] != 0){
                board[i][j-1] = board[i][j];
                board[i][j] = 0;
                i = 0; j = 0;
                continue;
            }
        }
    }
    return stat;
}

void makeRandNum(){
    int i, j, k;
    do{
        i = rand() % 5;
        j = rand() % 5;
        k = rand() % 2 + 1;
    } while(board[i][j] != 0);
    board[i][j] = k * 2;
}

void gameStart(){
    ///gamerInfo
    int isSuccess = 0; //0 if fail, 1 if success;
    int moveCount = 0;
    int comboCount = 0;

    ///gameInit
    score = 0;
    char input;
    int tmp1;
    int comboStat = 0;

    boardInit();
    int i = rand() % 5;
    int j = rand() % 5;
    tmp1 = rand() % 2 + 1;
    int tmpi = i, tmpj = j;
    board[i][j] = tmp1 * 2;

    do {
        i = rand() % 5;
        j = rand() % 5;
        //printf("%d %d || %d %d\n", tmpi, tmpj, i, j); //중복인 지점 확인
    } while(tmpi == i && tmpj == j);
    tmp1 = rand() % 2 + 1;
    board[i][j] = tmp1 * 2;
    ///

    ///loop game
    while(1){
        fflush(stdin);
        printBoard();
        printf("Combo Count : %d\n", comboCount);
        printf("Score : %d\n", score);
        input = getch();
        //printf("%c\n",input);

        switch (input) {
            case 'w':
            case 'W':
                //printf("up 드가자\n");
                tmp1 = slideUp();
                if(tmp1)
                    makeRandNum();
                else
                    comboStat = 0;
                if(tmp1 == 1) comboStat = 0;
                if(tmp1 == 2){
                    if(comboStat == 1)
                        comboCount ++;
                    comboStat = 1;
                }
                break;
            case 's':
            case 'S':
                //printf("down 드가자\n");
                tmp1 = slideDown();
                if(tmp1)
                    makeRandNum();
                else
                    comboStat = 0;
                if(tmp1 == 1) comboStat = 0;
                if(tmp1 == 2){
                    if(comboStat == 1)
                        comboCount ++;
                    comboStat = 1;
                }
                break;
            case 'd':
            case 'D':
                //printf("right 드가자\n");
                tmp1 = slideRight();
                if(tmp1)
                    makeRandNum();
                else
                    comboStat = 0;
                if(tmp1 == 1) comboStat = 0;
                if(tmp1 == 2){
                    if(comboStat == 1)
                        comboCount ++;
                    comboStat = 1;
                }
                break;
            case 'a':
            case 'A':
                //printf("left 드가자\n");
                tmp1 = slideLeft();
                if(tmp1)
                    makeRandNum();
                else
                    comboStat = 0;
                if(tmp1 == 1) comboStat = 0;
                if(tmp1 == 2){
                    if(comboStat == 1)
                        comboCount ++;
                    comboStat = 1;
                }
                break;
            default:
                continue;
        }
        ////Break Condition
        if(ifGameOver()){
            printBoard();
            printf("GameOver!\n");
            break;
        }
        ////
        ////Clear Condition
        for(int i = 0; i < 5; ++i){
            for(int j = 0; j < 5; ++j){
                if(board[i][j] == 2048) {
                    printf("Clear!\n");

                    break;
                }
            }
        }
        ////
        system("clear");
    }
    ////

}

void printHowTo(){
    int k;
    while(1){
        for(int i = 0;i < 60; ++i){
            printf("-");
        }
        printf("\n1.\t게임 시작시 2개의 2또는 4가 랜덤한 곳에서 생성된다.");
        printf("\n2.\tw,a,s,d 중 하나를 누르면 해당하는 방향으로 게임판에 있는 숫자를 전부 몰게 된다.");
        printf("\n3.\t이동하면서 같은 숫자를 만날 경우 합쳐지며, 빈 자리 중 한칸에 랜덤하게 2 또는 4가 생성된다.");
        printf("\n4.\t이를 반복하여 2048타일을 만들면 게임 클리어");
        printf("\n5.\t2048을 만들기 전 더이상 숫자를 몰 수 없는 경우 게임 오버");
        printf("\n*. 숫자가 더해질 때 3개 이상이 한번에 더해지지 않는다");
        printf("\n#그만 보려면 0 입력");
        printf("\n");
        for(int i = 0; i < 60; ++i){
            printf("-");
        }
        printf("\n>> ");
        k = getch();

        if(k == '0') {
            system("clear");
            return;
        }
        else{
            system("clear");
            continue;
        }
    }
}

int main() {
    int input = 0;
    srand(time(NULL));
    while(1){
        printMenu();
        scanf("%d", &input);
        switch (input) {
            case 1:
                system("clear");
                gameStart();
                break;
            case 2:
                system("clear");
                printHowTo();
                break;
            case 3:

                break;
            case 4:
                printf("게임을 종료합니다.\n");
                return 0;
            default:
                system("clear");
                continue;
        }
    }
}
