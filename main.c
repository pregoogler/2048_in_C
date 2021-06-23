#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>

#define MAX_TIME 30

int board[5][5] = {
        {2, 8,  2,  4,  0},
        {8, 16, 32, 64, 128},
        {16, 32,  64,  128,  256},
        {32, 64,  128,  256,  512},
        {64, 128,  256,  512,  1024}
};      //게임 판  ////(GameOver Testcase)
int score = 0;
static struct termios initial_settings, new_settings;
static int peek_character = -1;
typedef struct GAMER{
    char name[20];
    int isSuccess;
    int moveCount;
    int comboCount;
    int time;
} gamer; ////이름, 성공여부, 이동횟수, 콤보 카운트 , (시간)

int _kbhit()
{
    unsigned char ch;
    int nread;

    if (peek_character != -1) return 1;
    new_settings.c_cc[VMIN]=0;
    tcsetattr(0, TCSANOW, &new_settings);
    nread = read(0,&ch,1);
    new_settings.c_cc[VMIN]=1;
    tcsetattr(0, TCSANOW, &new_settings);
    if(nread == 1)
    {
        peek_character = ch;
        return 1;
    }
    return 0;
}   //키보드 입력이 들어왔는지 확인
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

void init_keyboard()
{
    tcgetattr(0,&initial_settings);
    new_settings = initial_settings;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;
    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &new_settings);
}
void close_keyboard()
{
    tcsetattr(0, TCSANOW, &initial_settings);
}
int readch()
{ char ch; if(peek_character != -1) { ch = peek_character; peek_character = -1; return ch; } read(0,&ch,1); return ch; }

void makeRandNum(){
    int i, j, k;
    do{
        i = rand() % 5;
        j = rand() % 5;
        k = rand() % 2 + 1;
    } while(board[i][j] != 0);
    board[i][j] = k * 2;
}
int getTotalLine(char *name){
    FILE *fp;
    int line=0;
    char c;
    fp=fopen(name,"r");
    while((c=fgetc(fp))!=EOF)
        if(c=='\n') line++;
    fclose(fp);
    return(line);
}

void gameStart(){
    ///gamerInfo
    int isSuccess = 0; //0 if fail, 1 if success;
    int moveCount = 0;
    int comboCount = 0;
    char gamerName[100];
    const time_t startT = time(NULL);
    time_t playT;
    time_t endT;

    FILE * fp = fopen("gamerInfo.txt", "a+");

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
    init_keyboard();
    ///loop game
    while(1){
        fflush(stdin);
        printBoard();
        printf("Combo Count : %d\n", comboCount);
        printf("Score : %d\n", score);
        printf("MoveCount : %d\n", moveCount);

        ////시간
        printf("제한 시간 : %d분\n", MAX_TIME);
        playT = time(NULL);
        printf("지난 시간 : %d분 %d초\n", (int)((playT-startT) / 60 ),(int)((playT-startT)%60));
        if((int)((playT-startT) / 60 ) >= MAX_TIME){
            printf("Time OVER!\n");
            break;
        }

        if(_kbhit()) {
            input = readch();
            //printf("%c\n",input);

            switch (input) {
                case 'w':
                case 'W':
                    //printf("up 드가자\n");
                    tmp1 = slideUp();
                    if (tmp1) {
                        makeRandNum();
                        moveCount++;
                    } else
                        comboStat = 0;
                    if (tmp1 == 1) comboStat = 0;
                    if (tmp1 == 2) {
                        if (comboStat == 1)
                            comboCount++;
                        comboStat = 1;
                    }
                    break;
                case 's':
                case 'S':
                    //printf("down 드가자\n");
                    tmp1 = slideDown();
                    if (tmp1) {
                        makeRandNum();
                        moveCount++;
                    } else
                        comboStat = 0;
                    if (tmp1 == 1) comboStat = 0;
                    if (tmp1 == 2) {
                        if (comboStat == 1)
                            comboCount++;
                        comboStat = 1;
                    }
                    break;
                case 'd':
                case 'D':
                    //printf("right 드가자\n");
                    tmp1 = slideRight();
                    if (tmp1) {
                        makeRandNum();
                        moveCount++;
                    } else
                        comboStat = 0;
                    if (tmp1 == 1) comboStat = 0;
                    if (tmp1 == 2) {
                        if (comboStat == 1)
                            comboCount++;
                        comboStat = 1;
                    }
                    break;
                case 'a':
                case 'A':
                    //printf("left 드가자\n");
                    tmp1 = slideLeft();
                    if (tmp1) {
                        makeRandNum();
                        moveCount++;
                    } else
                        comboStat = 0;
                    if (tmp1 == 1) comboStat = 0;
                    if (tmp1 == 2) {
                        if (comboStat == 1)
                            comboCount++;
                        comboStat = 1;
                    }
                    break;
                default:
                    system("clear");
                    continue;
            }
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
                    isSuccess = 1;

                    break;
                }
            }
        }
        ////
        usleep(10000);
        system("clear");
    }
    close_keyboard();
    ////
    ////저장

    endT = time(NULL);
    printf("UserName >> ");
    scanf("%s", gamerName);
    printf("%ld\n", endT);
    printf("%ld\n", startT);
    fprintf(fp, "%s %d %d %d %ld\n", gamerName, isSuccess, moveCount, comboCount, endT-startT);
    //// 이름, 성공여부, 이동횟수, 콤보 카운트 , (시간)

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
void ranking(){
    FILE * fp = fopen("gamerInfo.txt", "r");
    int line = getTotalLine("gamerInfo.txt"), k;
    gamer * gp = (gamer *)malloc(sizeof(gamer) * line);
    gamer tmp = {0,};
//    printf("line : %d\n", line);
    for(int i = 0; i < line; ++i){
        fscanf(fp, "%s %d %d %d %d", gp[i].name, &gp[i].isSuccess, &gp[i].moveCount, &gp[i].comboCount, &gp[i].time);
    }
//    for(int i = 0; i < line; ++i){
//        printf("%s\n", gp[i].name);
//    }
//    printf("----\n");
    for(int i = 0; i < line; ++i){
        for(int j = 0; j < line-i-1; ++j){
            if(gp[j].time > gp[j+1].time){
                tmp = gp[j+1];
                gp[j+1] = gp[j];
                gp[j] = tmp;
            }
        }
    }
//    for(int i = 0; i < line; ++i){
//        printf("%s\n", gp[i].name);
//    }
    while(1) {
        printf("-----------------------\n클리어 시간에 따른 랭킹(성공한 사람)\n-----------------------\n");
        int rank = 1;
        for (int i = 0; i < line; ++i) {
            if (gp[i].isSuccess) {
                printf("%d등 : %s - %d분 %d초\n", rank++, gp[i].name, (int) (gp[i].time / 60), gp[i].time % 60);
            }
        }
        printf("\n#그만 보려면 0 입력");
        printf("\n>>");
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
                system("clear");
                ranking();
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