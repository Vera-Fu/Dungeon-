#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<Windows.h>
#include<time.h>
#include"conioex.h"

#define MAX_MAP 15

//------------自定义结构体------------
typedef enum {
    DISP_START = 0,		
    DISP_LOAD,			
    DISP_END,		
}SCENE;

typedef struct {
    int x;
    int y;
}PLAYER;

typedef struct {
    int x;              //此区域的x坐标，同时也是玩家的x坐标
    int y;              //此区域的y坐标，同时也是玩家的y坐标
    int index;          //区域类型
    int isCheck;        //此区域是否已清除
    int isUp;           //是否有向上的路
    int isDown;         //是否有向下的路
    int isLeft;         //是否有向左的路
    int isRight;        //是否有向右的路
}MAPS;

typedef struct {
    char name[31];
    int hp;
    int atk;
    int def;
    int spd;
    int eva;
    int cri;
}STATUS;


//------------声明结构体------------
MAPS map[MAX_MAP];

//------------全局变量声明------------
//int floor = 1;

//------------函数声明------------
void title();               //タイトル画面
void runGame();
void moveMent(PLAYER player);
void mapsPaint(MAPS map);
void mapsPaint1();

void hideCursor();          //cursorを隠させて

int main() {
    hideCursor();
    title();

    return 0;
}

//タイトル画面
void title() {
    int index = 0;
    while (1)
    {
        gotoxy(50, 5);
        printf("┏━━━━━━━━━━━━━━┓");
        gotoxy(50, 6);
        printf("┃%s┃", "  ■Dungeon■ ");
        gotoxy(50, 7);
        printf("┗━━━━━━━━━━━━━━┛");
        switch (index)
        {
        case DISP_START:
            gotoxy(53, 15);
            textattr(0x70);
            printf("ゲーム開始");
            textattr(0x0F);
            gotoxy(52, 17);
            printf("ゲームロード");
            gotoxy(53, 19);
            printf("ゲーム終了");
            rewind(stdin);
            break;
        case DISP_LOAD:
            gotoxy(53, 15); 
            printf("ゲーム開始");          
            gotoxy(52, 17);
            textattr(0x70);
            printf("ゲームロード");
            gotoxy(53, 19);
            textattr(0x0F);
            printf("ゲーム終了");
            rewind(stdin);
            break;
        case DISP_END:
            gotoxy(53, 15);       
            printf("ゲーム開始");
            gotoxy(52, 17);
            printf("ゲームロード");
            gotoxy(53, 19);
            textattr(0x70);
            printf("ゲーム終了");
            textattr(0x0F);
            rewind(stdin);
            break;
        default:
            break;
        }        
        if (inport(PK_UP))
        {
            index--;
            if (index < 0)
            {
                index = 2;
            }
            reinport();
            clrscr();
        }
        if (inport(PK_DOWN))
        {
            index++;
            if (index > 2)
            {
                index = 0;
            }
            reinport();
            clrscr();
        }

        if (inport(PK_ENTER))
        {
            switch (index)
            {
            case DISP_START:
                runGame();
                break;
            case DISP_LOAD:
                break;
            case DISP_END:
                return;
            default:
                break;
            }
        }                    
    }
}

void runGame() {
    clrscr();
    PLAYER player = { 5, 9 };
    gotoxy(player.x, player.y);
    printf("♀");
    mapsPaint1();
    moveMent(player);
    getchar();
}

void moveMent(PLAYER player) {
    while (1)
    {
        
        gotoxy(player.x, player.y);
        printf("♀");
        //当该区域已被探索，便将其显示
        for (int i = 0; i < MAX_MAP; i++)
        {
            if (map[i].isCheck == 1)
            {
                mapsPaint(map[i]);
            }
        }
        //玩家向上移动的处理
        if (inport(PK_UP))
        {
            for (int i = 0; i < MAX_MAP; i++)
            {
                if (player.x == map[i].x && player.y - 5 == map[i].y)
                {
                    player.y -= 5;
                    map[i].isCheck = 1; 
                    break;
                }
            }
            reinport();
            clrscr();
        }
        //玩家向下移动的处理
        if (inport(PK_DOWN))
        {
            for (int i = 0; i < MAX_MAP; i++)
            {
                if (player.x == map[i].x && player.y + 5 == map[i].y)
                {
                    player.y += 5;
                    map[i].isCheck = 1;
                    break;
                }
            }
            reinport();
            clrscr();
        }
        //玩家向左移动的处理
        if (inport(PK_LEFT))
        {
            for (int i = 0; i < MAX_MAP; i++)
            {
                if (player.x - 10 == map[i].x && player.y == map[i].y)
                {
                    player.x -= 10;
                    map[i].isCheck = 1;
                    break;
                }
            }
            reinport();
            clrscr();
        }
        //玩家向右移动的处理
        if (inport(PK_RIGHT))
        {
            for (int i = 0; i < MAX_MAP; i++)
            {
                if (player.x + 10 == map[i].x && player.y == map[i].y)
                {
                    player.x += 10;
                    map[i].isCheck = 1;
                    break;
                }
            }
            reinport();
            clrscr();
        }
    }
    
    
}

//区域绘制
void mapsPaint(MAPS map) {
    if (map.isUp == 1)
    {
        gotoxy(map.x + 2, map.y);
        printf("┃");
    }
    if (map.isDown == 1)
    {
        gotoxy(map.x + 2, map.y + 4);
        printf("┃");
    }
    if (map.isLeft == 1)
    {
        gotoxy(map.x - 2, map.y + 2);
        printf("━");
    }
    if (map.isRight == 1)
    {
        gotoxy(map.x + 6, map.y + 2);
        printf("━");
    }
    gotoxy(map.x, map.y + 1);
    printf("■■■");
    gotoxy(wherex() - 6, wherey() + 1);
    printf("■  ■");
    gotoxy(wherex() - 6, wherey() + 1);
    printf("■■■");
    
    //gotoxy(map.x, map.y);
    //printf("♀");

    //getchar();
}



//关卡1的地图绘制
void mapsPaint1() {
    gotoxy(5, 10);
    map[0] = { wherex(), wherey() - 1, 0, 1, 1, 1, 0, 1 };      //(x,y,index,isCheck,isUp,isDown,isLeft,isRight);
    //mapsPaint(map[0]);
    gotoxy(5, 5);
    map[1] = { wherex(), wherey() - 1, 0, 0, 0, 1, 0, 0 };
    //mapsPaint(map[1]);
    gotoxy(5, 15);
    map[2] = { wherex(), wherey() - 1, 0, 0, 1, 0, 0, 1 };
    //mapsPaint(map[2]);
    gotoxy(15, 10);
    map[3] = { wherex(), wherey() - 1, 0, 0, 0, 0, 1, 1 };
    //mapsPaint(map[3]);
    gotoxy(25, 10);
    map[4] = { wherex(), wherey() - 1, 0, 0, 1, 0, 1, 1 };
    //mapsPaint(map[4]);
    gotoxy(25, 5);
    map[5] = { wherex(), wherey() - 1, 0, 0, 0, 1, 0, 1 };
    //mapsPaint(map[5]);
    gotoxy(35, 5);
    map[6] = { wherex(), wherey() - 1, 0, 0, 0, 1, 1, 0 };
    //mapsPaint(map[6]);
    gotoxy(35, 10);
    map[7] = { wherex(), wherey() - 1, 0, 0, 1, 0, 1, 1 };
    //mapsPaint(map[7]);
    gotoxy(45, 10);
    map[8] = { wherex(), wherey() - 1, 0, 0, 0, 0, 1, 0 };
    //mapsPaint(map[8]);
    gotoxy(15, 15);
    map[9] = { wherex(), wherey() - 1, 0, 0, 0, 0, 1, 1 };
    //mapsPaint(map[9]);
    gotoxy(25, 15);
    map[10] = { wherex(), wherey() - 1, 0, 0, 0, 0, 1, 0 };
    //mapsPaint(map[10]);
}

//cursorを隠させて
void hideCursor() {
    CONSOLE_CURSOR_INFO cur;
    cur.dwSize = 1;
    cur.bVisible = 0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cur);
}


