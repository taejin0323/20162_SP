#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <curses.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
 
int hp = 100;        // 체력
int a = 0;            // 문자열 위치
int i = 0;            // for문용
int length = 0;
char hpText[3] = { 0 };
node *ptr = 0;
char enterText[20] = { 0 };
int enterHere = 0;
int sleep_time = 1;
 
void function(int signum)
{
    reset();        // 메모리 할당 해제
    curs_set(1);    // 커서 표시 활성화
    endwin();        // 콘솔 윈도우창 닫기
    exit(1);        // 프로그램 종료
}
 
// 큐를 비워주고 메모리 할당을 해제해주는 함수
void reset()
{
    node *temp = NULL;
    node *temp2 = NULL;
 
    temp = ptr;
 
    if (temp != NULL)
    {
        while (length > 0)
        {
            temp = ptr;
 
            while (temp->link)
            {
                temp2 = temp;
                temp = temp->link;
            }
 
            free(temp);
 
            if (temp2 != NULL)
                temp2->link = NULL;
 
            length--;
            temp2 = NULL;
        }
 
        ptr = NULL;
    }
}
 
// 화면에 글자를 보이게만 하는 함수
void thread_1(void *none)
{
    int t = sleep_time;
 
    while (hp > 0)
    {
        //alarm(2);
 
        node *temp = 0;
 
        addQueue(returnWord(), (rand() % 40) + 8);
        temp = ptr;
 
        while (temp)
        {
            draw(temp->row, temp->col, temp->str);
            temp = temp->link;
        }
 
        move(17, 12);
 
        sleep(t);
 
    }
}
 
// 자연수를 문자열로
int itoa(int n, char *str)
{
    int temp;
 
    if (n <= 0)
    {
        strcpy(str, "0");
        return 0;
    }
 
    temp = itoa(n / 10, str);
 
    *(str + temp) = 48 + (n % 10);
 
    return temp + 1;
}
 
// 입력한 문자열을 큐에서 찾아서 없애주는 함수
void findWord(char *str)
{
    node *temp = 0;
    temp = ptr;
 
    while (temp)
    {
        if (!strcmp(temp->str, str))
        {
            strcpy(temp->str, "");
            return;
        }
        else
            temp = temp->link;
    }
}
 
// 노드 생성
node *makeNode()
{
    node *temp = 0;
 
    temp = (node *)malloc(sizeof(*temp));
    temp->link = 0;
}
 
// 한 줄씩 밑으로 내려오게 해 주는 코드
void makePlusOne()
{
    node *temp = ptr->link;
 
    while (temp)
    {
        temp->row += 1;
        temp = temp->link;
    }
}
 
// 큐에 하나씩 넣는 함수
void addQueue(char *str, int col)
{
    node *temp = 0;
    node *temp2 = 0;
 
    if (ptr == 0)
    {
        ptr = makeNode();
        strcpy(ptr->str, str);
        ptr->row = 1;
        ptr->col = col;
    }
    else
    {
        temp = makeNode();
        strcpy(temp->str, str);
        temp->row = 1;
        temp->col = col;
        temp->link = ptr;
        ptr = temp;
        makePlusOne();
    }
 
    length++;
 
    if (length > 15)
    {
        while (temp->link)
        {
            temp2 = temp;
            temp = temp->link;
        }
 
        hp -= strlen(temp->str);
 
        hpText[2] = '\0';
 
        itoa(hp, hpText);
        move(17, 55);
        addstr("    ");
        move(17, 55);
        addstr(hpText);
 
        free(temp);
        temp2->link = 0;
 
        length--;
    }
}
 
// 단어 데이터베이스에서 글자를 골라서 리턴해주는 부분
char *returnWord()
{
    char *database[] = { "Apple", "Jung", "Cocaine", "Hello", "Elite", "Fail", "Game",
                         "Halo", "Icon", "Jail", "Knight", "Lake", "Monkey", "Nope" };
 
    if (a == 13)
        a = 0;
    else
        a++;
 
    return database[a];
}
 
// row행 col열에 문자열 str을 출력해주는 부분
void draw(int row, int col, char *str)
{
    move(row, 0);
    addstr("                                                       ");
    move(row, col);
    addstr(str);
    refresh();
}
 
void startGame()
{
    pthread_t t1;
 
    clear();
 
    draw(16, 0, "   -------------------------------------------------------- ");
    draw(17, 0, "  | Enter :                                     | HP :     |");
    draw(18, 0, "   -------------------------------------------------------- ");
 
    itoa(hp, hpText);
    move(17, 55);
    addstr("    ");
    move(17, 55);
    addstr(hpText);
 
    pthread_create(&t1, NULL, thread_1, NULL);
 
    while (hp > 0)
    {
        for (enterHere = 0; enterHere < 20;)
        {
            int c = getch();
 
            if (c == '\n')
            {
                enterText[enterHere] = '\0';
                findWord(enterText);
 
                for (i = 0; i < 20; i++)
                {
                    enterText[i] = '\0';
                }
 
 
                draw(17, 0, "  | Enter :                                     | HP : ");
                move(17, 12);
 
                break;
            }
            else if (c == 127)
            {
                if (enterHere > 0)
                {
                    enterText[--enterHere] = '\0';
                    move(17, 12);
                    addstr("                    ");
                    move(17, 12);
                    addstr(enterText);
                }
                else
                {
                    move(17, 12);
                    addstr("                    ");
                }
            }
            else
            {
                enterText[enterHere++] = c;
                move(17, 12);
                addstr(enterText);
            }
 
            refresh();
        }
    }
 
    pthread_join(&t1, NULL);
 
    reset();
    clear();
}
