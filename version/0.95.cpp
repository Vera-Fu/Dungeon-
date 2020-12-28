#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<Windows.h>
#include<time.h>
#include"conioex.h"

#define MAX_MAP 15
#define MAX_LOGS 26
#define MAX_STATUS 15

//------------自定义结构体------------
typedef enum {
    DISP_START = 0,
    DISP_LOAD,
    DISP_END,
}SCENE;

typedef struct {
    int x;
    int y;
    int potionNum;
    int charaNum;
    int gold;
}PLAYER;

typedef struct {
    int x;              //此区域的x坐标，同时也是玩家的x坐标
    int y;              //此区域的y坐标，同时也是玩家的y坐标
    int index;          //区域类型:1.商店 2.空房 3.回复房（免费） 4.招募房 5.随机房 6.战斗房 7.boss房 8.下一层楼梯 9.初始房
    int isCheck;        //此区域是否已清除　0：未踏入 1：再踏入不可重新触发 2：再踏入可重新触发
    int isUp;           //是否有向上的路
    int isDown;         //是否有向下的路
    int isLeft;         //是否有向左的路
    int isRight;        //是否有向右的路
}MAPS;

typedef struct {
    char name[31];
    int maxHp;
    int hp;
    int atk;
    int def;
    int spd;
    int eva;
    int cri;
    int isBattle;       //0は休憩中、1は出戦中、2は戦闘不能
}STATUS;


//------------声明结构体------------
MAPS map[MAX_MAP];
PLAYER player[1];
STATUS status[MAX_STATUS];
STATUS enemy[1];

//------------全局变量声明------------
int G_floor;
int G_checkMapNum;
int G_logs;
int G_status;

//------------函数声明------------
void title();               //タイトル画面
void menu();                //菜单界面
void runGame();             //开始游戏
void moveMent(PLAYER* player, MAPS* map);       //读取玩家的输入

void mapsPaint(MAPS* map);  //区域绘制                    
void mapsType(MAPS* map);   //区域类型设置
void mapsPaint1();          //关卡地图绘制
void mapsPaint2();
void mapsPaint3();

void logs(const char* c);   //logs保存
void logsRead();            //logs打印

void room(MAPS* map);
void shop(PLAYER* player, MAPS* map, STATUS* status);
void healRoom(STATUS* status, MAPS* map);
void charaRoom(PLAYER* player, MAPS* map, STATUS* status);
void randomRoom(PLAYER* player, MAPS* map, STATUS* status);
void enemyRoom(STATUS* enemy);
void bossRoom(STATUS* enemy);
void nextFloorRoom(PLAYER* player, MAPS* map, STATUS* status);

void charaInput(STATUS* status);
void charaOutput(STATUS* status, PLAYER* player);

void battle(STATUS* status, STATUS* enemy, PLAYER* player);
void battleS(STATUS* status, STATUS* enemy);

void saveGame(PLAYER* player, MAPS* map, STATUS* status);
void loadGame(PLAYER* player, MAPS* map, STATUS* status);

void endGame();             //结束游戏功能

void hideCursor();          //cursorを隠させて

int main() {
    //enemyBattle(enemy);
    //charaInput(status);
    //charaInput(status);
    //charaInput(status);
    charaInput(status);
    charaInput(status);   
    fopen("logs.txt", "w");
    hideCursor();
    title();

    return 0;
}

//タイトル画面
void title() {
    clrscr();
    int index = 0;
    while (1)
    {
        reinport();
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
                loadGame(player, map, status);
                break;
            case DISP_END:
                endGame();
                break;
            default:
                break;
            }
        }
    }
}

void menu() {
    int index = 0;
    clrscr();
    while (1)
    {
        reinport();
        for (int i = 6; i < 25; i++)
        {
            gotoxy(45, i);
            printf("┃");
        }
        for (int i = 6; i < 25; i++)
        {
            gotoxy(72, i);
            printf("┃");
        }
        gotoxy(45, 5);
        printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
        gotoxy(45, 25);
        printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
        gotoxy(53, 7);
        printf("■■MENU■■");
        switch (index)
        {
        case 0:
            gotoxy(53, 15);
            textattr(0x70);
            printf("つづく");
            textattr(0x0F);
            gotoxy(53, 17);
            printf("セーブ");
            gotoxy(53, 19);
            printf("キャラステータス");
            gotoxy(53, 21);
            printf("ゲーム終了");
            rewind(stdin);
            break;
        case 1:
            gotoxy(53, 15);
            printf("つづく");
            gotoxy(53, 17);
            textattr(0x70);
            printf("セーブ");
            textattr(0x0F);
            gotoxy(53, 19);
            printf("キャラステータス");
            gotoxy(53, 21);
            printf("ゲーム終了");
            rewind(stdin);
            break;
        case 2:
            gotoxy(53, 15);
            printf("つづく");
            gotoxy(53, 17);
            printf("セーブ");
            gotoxy(53, 19);
            textattr(0x70);
            printf("キャラステータス");
            textattr(0x0F);
            gotoxy(53, 21);
            printf("ゲーム終了");
            rewind(stdin);
            break;
        case 3:
            gotoxy(53, 15);
            printf("つづく");
            gotoxy(53, 17);
            printf("セーブ");
            gotoxy(53, 19);
            printf("キャラステータス");
            gotoxy(53, 21);
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
                index = 3;
            }
            reinport();
            clrscr();
        }
        if (inport(PK_DOWN))
        {
            index++;
            if (index > 3)
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
            case 0:
                return;
            case 1:
                saveGame(player, map, status);
                break;
            case 2:
                charaOutput(status, player);
                break;
            case 3:
                endGame();
                break;
            default:
                break;
            }
        }
    }
}

void runGame() {
    clrscr();
    G_floor = 1;
    G_checkMapNum = 1;
    player[0] = { 5, 9, 1, 0, 30 };
    mapsPaint1();
    while (1)
    {
        moveMent(player, map);
        logsRead();
    }

    getchar();
}

void moveMent(PLAYER* player, MAPS* map) {
    gotoxy(player->x, player->y);
    printf("♀");
    //当该区域已被探索，便将其显示
    gotoxy(25, 2);
    printf("%d層目", G_floor);
    for (int i = 0; i < MAX_MAP; i++)
    {
        if ((map + i)->isCheck == 1 || (map + i)->isCheck == 2)
        {
            mapsPaint(map + i);
        }
    }
    if (inport(PK_ESC))
    {
        menu();
        reinport();
        clrscr();
    }
    //玩家向上移动的处理
    if (inport(PK_UP))
    {
        for (int i = 0; i < MAX_MAP; i++)
        {
            if (player->x == (map + i)->x && player->y - 5 == (map + i)->y && (map + i)->isDown == 1)
            {
                player->y -= 5;
                logs("上に移動する\n");
                if ((map + i)->isCheck == 0 || (map + i)->isCheck == 2)
                {
                    if ((map + i)->isCheck == 0)
                    {
                        G_checkMapNum += 1;
                    }
                    (map + i)->isCheck = 1;                   
                    room(map + i);
                }
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
            if (player->x == (map + i)->x && player->y + 5 == (map + i)->y && (map + i)->isUp == 1)
            {
                player->y += 5;
                logs("下に移動する\n");
                if ((map + i)->isCheck == 0 || (map + i)->isCheck == 2)
                {
                    if ((map + i)->isCheck == 0)
                    {
                        G_checkMapNum += 1;
                    }
                    (map + i)->isCheck = 1;
                    room(map + i);
                }
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
            if (player->x - 10 == (map + i)->x && player->y == (map + i)->y && (map + i)->isRight == 1)
            {
                player->x -= 10;
                logs("左に移動する\n");
                if ((map + i)->isCheck == 0 || (map + i)->isCheck == 2)
                {
                    if ((map + i)->isCheck == 0)
                    {
                        G_checkMapNum += 1;
                    }
                    (map + i)->isCheck = 1;
                    room(map + i);
                }
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
            if (player->x + 10 == (map + i)->x && player->y == (map + i)->y && (map + i)->isLeft == 1)
            {
                player->x += 10;
                logs("右に移動する\n");
                if ((map + i)->isCheck == 0 || (map + i)->isCheck == 2)
                {
                    if ((map + i)->isCheck == 0)
                    {
                        G_checkMapNum += 1;
                    }
                    (map + i)->isCheck = 1;                 
                    room(map + i);
                }
                break;
            }
        }
        reinport();
        clrscr();
    }
}

//区域绘制
void mapsPaint(MAPS* map) {
    if (map->index == 0)
    {
        mapsType(map);
    }
    if (map->isUp == 1)
    {
        gotoxy(map->x + 2, map->y);
        printf("┃");
    }
    if (map->isDown == 1)
    {
        gotoxy(map->x + 2, map->y + 4);
        printf("┃");
    }
    if (map->isLeft == 1)
    {
        gotoxy(map->x - 2, map->y + 2);
        printf("━");
    }
    if (map->isRight == 1)
    {
        gotoxy(map->x + 6, map->y + 2);
        printf("━");
    }
    gotoxy(map->x, map->y + 1);
    printf("■■■");
    gotoxy(wherex() - 6, wherey() + 1);
    printf("■  ■");
    gotoxy(wherex() - 6, wherey() + 1);
    printf("■■■");
    switch (map->index)
    {
    case 1:
        gotoxy(map->x + 2, map->y + 2);
        printf("￥");
        break;
    case 2:
        gotoxy(map->x + 2, map->y + 2);
        printf("■");
        break;
    case 3:
        gotoxy(map->x + 2, map->y + 2);
        printf("◇");
        break;
    case 4:
        gotoxy(map->x + 2, map->y + 2);
        printf("◆");
        break;
    case 5:
        gotoxy(map->x + 2, map->y + 2);
        printf("？");
        break;
    case 6:
        gotoxy(map->x + 2, map->y + 2);
        printf("△");
        break;
    case 7:
        gotoxy(map->x + 2, map->y + 2);
        printf("▲");
        break;
    case 8:
        gotoxy(map->x + 2, map->y + 2);
        printf("○");
        break;
    case 9:
        gotoxy(map->x + 2, map->y + 2);
        printf("●");
        break;
    default:
        break;
    }

}

void mapsType(MAPS* map)
{
    if (G_checkMapNum == 8)
    {
        if (G_floor == 3)
        {
            map->index = 7;
        }
        else
        {
            map->index = 8;
        }
    }
    else
    {
        srand((unsigned int)time(NULL));
        int a = rand() % 100;               //用于判断该区域类型
        if (a >= 0 && a < 30)
        {
            map->index = 1;
        }
        if (a >= 30 && a < 35)
        {
            map->index = 2;
        }
        if (a >= 35 && a < 40)
        {
            map->index = 3;
        }     
        if (a >= 40 && a < 55)
        {
            map->index = 4;
        }        
        if (a >= 55 && a < 70)
        {
            map->index = 5;
        }              
        if (a >= 70 && a < 100)
        {
            map->index = 6;
        }
    }
}

//关卡1的地图绘制
void mapsPaint1() {
    gotoxy(5, 10);
    map[0] = { wherex(), wherey() - 1, 9, 1, 1, 1, 0, 1 };      //(x,y,index,isCheck,isUp,isDown,isLeft,isRight);
    gotoxy(5, 5);
    map[1] = { wherex(), wherey() - 1, 0, 0, 0, 1, 0, 0 };
    gotoxy(5, 15);
    map[2] = { wherex(), wherey() - 1, 0, 0, 1, 0, 0, 1 };
    gotoxy(15, 10);
    map[3] = { wherex(), wherey() - 1, 0, 0, 0, 0, 1, 1 };
    gotoxy(25, 10);
    map[4] = { wherex(), wherey() - 1, 0, 0, 1, 0, 1, 1 };
    gotoxy(25, 5);
    map[5] = { wherex(), wherey() - 1, 0, 0, 0, 1, 0, 1 };
    gotoxy(35, 5);
    map[6] = { wherex(), wherey() - 1, 0, 0, 0, 1, 1, 0 };
    gotoxy(35, 10);
    map[7] = { wherex(), wherey() - 1, 0, 0, 1, 0, 1, 1 };
    gotoxy(45, 10);
    map[8] = { wherex(), wherey() - 1, 0, 0, 0, 0, 1, 0 };
    gotoxy(15, 15);
    map[9] = { wherex(), wherey() - 1, 0, 0, 0, 0, 1, 1 };
    gotoxy(25, 15);
    map[10] = { wherex(), wherey() - 1, 0, 0, 0, 0, 1, 0 };
}

void mapsPaint2() {

}

void mapsPaint3() {

}

void logs(const char* c) {
    if (G_logs > MAX_LOGS)
    {
        FILE* fp = fopen("logs.txt", "w");
        fclose(fp);
        G_logs = 0;
    }
    FILE* fp = fopen("logs.txt", "a+");
    fputs(c, fp);
    fclose(fp);
    G_logs += 1;
}

void logsRead() {
    for (int i = 3; i < 30; i++)
    {
        gotoxy(60, i);
        printf("┃");
    }
    for (int i = 3; i < 30; i++)
    {
        gotoxy(115, i);
        printf("┃");
    }
    gotoxy(60, 2);
    printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
    gotoxy(60, 30);
    printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
    FILE* fp = fopen("logs.txt", "r+");
    char c[512];
    for (int i = 3; i < G_logs + 3; i++)
    {
        gotoxy(61, i);
        fscanf(fp, "%s", c);
        printf("%s", c);
    }
    fclose(fp);
}

void room(MAPS* map) {
    mapsPaint(map);
    switch (map->index)
    {
    case 1:
        shop(player, map, status);
        break;
    case 2:
        logs("何もない...\n");
        msleep(1000);
        break;
    case 3:
        healRoom(status, map);
        break;
    case 4:
        charaRoom(player, map, status);
        break;
    case 5:
        randomRoom(player, map, status);
        break;
    case 6:
        enemyRoom(enemy);
        break;
    default:
        break;
    }
}

void shop(PLAYER* player, MAPS* map, STATUS* status) {
    logs("商店を見つかりました\n");
    logsRead();
    gotoxy(wherex(), wherey());
    msleep(500);
    printf(".");
    msleep(500);
    printf(".");
    msleep(500);
    printf(".");
    msleep(1000);
    int index = 0;
    clrscr();
    while (1)
    {
        reinport();
        gotoxy(46, 4);
        printf("お金：%d G", player->gold);
        gotoxy(46, 26);
        printf("回復薬：%d", player->potionNum);
        for (int i = 6; i < 25; i++)
        {
            gotoxy(45, i);
            printf("┃");
        }
        for (int i = 6; i < 25; i++)
        {
            gotoxy(72, i);
            printf("┃");
        }
        gotoxy(45, 5);
        printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
        gotoxy(45, 25);
        printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
        gotoxy(50, 7);
        printf("ルイーダ商店");
        switch (index)
        {
        case 0:
            gotoxy(50, 12);
            textattr(0x70);
            printf("回復薬を買う(10 G)");
            textattr(0x0F);
            gotoxy(50, 14);
            printf("回復(50 G)");
            gotoxy(50, 16);
            printf("キャラを雇う(20-50 G)");
            gotoxy(50, 18);
            printf("閉じる");
            rewind(stdin);
            break;
        case 1:
            gotoxy(50, 12);
            printf("回復薬を買う(10 G)");
            textattr(0x70);
            gotoxy(50, 14);
            printf("回復(50 G)");
            textattr(0x0F);
            gotoxy(50, 16);
            printf("キャラを雇う(20-50 G)");
            gotoxy(50, 18);
            printf("閉じる");
            rewind(stdin);
            break;
            break;
        case 2:
            gotoxy(50, 12);
            printf("回復薬を買う(10 G)");
            gotoxy(50, 14);
            printf("回復(50 G)");
            textattr(0x70);
            gotoxy(50, 16);
            printf("キャラを雇う(20-50 G)");
            textattr(0x0F);
            gotoxy(50, 18);
            printf("閉じる");
            rewind(stdin);
            break;
        case 3:
            gotoxy(50, 12);
            printf("回復薬を買う(10 G)");
            gotoxy(50, 14);
            printf("回復(50 G)");
            gotoxy(50, 16);
            printf("キャラを雇う(20-50 G)");
            textattr(0x70);
            gotoxy(50, 18);
            printf("閉じる");
            textattr(0x0F);
            rewind(stdin);
        default:
            break;
        }
        if (inport(PK_UP))
        {
            index--;
            if (index < 0)
            {
                index = 3;
            }
            reinport();
            clrscr();
        }
        if (inport(PK_DOWN))
        {
            index++;
            if (index > 3)
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
            case 0:
                if (player->gold >= 10)
                {
                    player->gold -= 10;
                    player->potionNum += 1;
                    gotoxy(80, 15);
                    printf("購入成功！閉店");
                    msleep(500);
                    printf(".");
                    msleep(500);
                    printf(".");
                    msleep(500);
                    printf(".");
                    msleep(500);
                    clrscr();
                    return;
                }
                else
                {
                    gotoxy(80, 15);
                    printf("お金は足りない！");
                    break;
                }
            case 1:
                if (player->gold >= 50)
                {
                    player->gold -= 50;
                    for (int i = 0; i < G_status; i++)
                    {
                        (status + i)->hp = (status + i)->maxHp;
                    }
                    gotoxy(80, 15);
                    printf("全てのキャラが回復した！閉店");
                    msleep(500);
                    printf(".");
                    msleep(500);
                    printf(".");
                    msleep(500);
                    printf(".");
                    msleep(500);
                    clrscr();
                    return;
                }
                else
                {
                    gotoxy(80, 15);
                    printf("お金は足りない！");
                    break;
                }
            case 2:
                if (player->gold >= 50)
                {
                    srand(time(NULL));
                    int g = rand() % 30 + 20;
                    player->gold -= g;
                    charaInput(status);
                    gotoxy(80, 15);
                    printf("雇う成功！閉店");
                    msleep(500);
                    printf(".");
                    msleep(500);
                    printf(".");
                    msleep(500);
                    printf(".");
                    msleep(500);
                    clrscr();
                    return;
                }
                else
                {
                    gotoxy(80, 15);
                    printf("お金は足りない！");
                    break;
                }
            case 3:
                clrscr();
                map->isCheck = 2;
                return;
            default:
                break;
            }
        }
    }
}

void healRoom(STATUS* status, MAPS* map) {
    logs("無料回復屋を見つかりました\n");
    logsRead();
    gotoxy(wherex(), wherey());
    msleep(500);
    printf(".");
    msleep(500);
    printf(".");
    msleep(500);
    printf(".");
    msleep(1000);
    int index = 0;
    clrscr();
    while (1)
    {
        reinport();
        for (int i = 6; i < 25; i++)
        {
            gotoxy(45, i);
            printf("┃");
        }
        for (int i = 6; i < 25; i++)
        {
            gotoxy(72, i);
            printf("┃");
        }
        gotoxy(45, 5);
        printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
        gotoxy(45, 25);
        printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
        gotoxy(53, 7);
        printf("回復したいか？");
        gotoxy(54, 8);
        printf("(一回だけ)");
        switch (index)
        {
        case 0:
            gotoxy(53, 15);
            textattr(0x70);
            printf("はい");
            textattr(0x0F);
            gotoxy(53, 17);
            printf("いいえ");
            rewind(stdin);
            break;
        case 1:
            gotoxy(53, 15);
            printf("はい");
            gotoxy(53, 17);
            textattr(0x70);
            printf("いいえ");
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
                index = 1;
            }
            reinport();
            clrscr();
        }
        if (inport(PK_DOWN))
        {
            index++;
            if (index > 1)
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
            case 0:
                for (int i = 0; i < G_status; i++)
                {
                    (status + i)->hp = (status + i)->maxHp;
                    if ((status + index)->isBattle == 2)
                    {
                        (status + index)->isBattle = 0;
                    }                    
                }
                gotoxy(60, 27);
                printf("全てのキャラが回復した！ENTERを押してください...");
                rewind(stdin);
                getchar();
                return;
            case 1:
                clrscr();
                map->isCheck = 2;
                return;
            default:
                break;
            }
        }
    }

}

void charaRoom(PLAYER* player, MAPS* map, STATUS* status) {
    logs("ギルガメッシュ酒場を見つかりました\n");
    logsRead();
    gotoxy(wherex(), wherey());
    msleep(500);
    printf(".");
    msleep(500);
    printf(".");
    msleep(500);
    printf(".");
    msleep(1000);
    int index = 0;
    clrscr();
    while (1)
    {
        reinport();
        for (int i = 6; i < 25; i++)
        {
            gotoxy(45, i);
            printf("┃");
        }
        for (int i = 6; i < 25; i++)
        {
            gotoxy(72, i);
            printf("┃");
        }
        gotoxy(45, 5);
        printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
        gotoxy(45, 25);
        printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
        gotoxy(53, 7);
        printf("雇いたいか？");
        gotoxy(46, 8);
        printf("(一回だけ。25 Gが必要だ。)");
        switch (index)
        {
        case 0:
            gotoxy(53, 15);
            textattr(0x70);
            printf("はい");
            textattr(0x0F);
            gotoxy(53, 17);
            printf("いいえ");
            rewind(stdin);
            break;
        case 1:
            gotoxy(53, 15);
            printf("はい");
            gotoxy(53, 17);
            textattr(0x70);
            printf("いいえ");
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
                index = 1;
            }
            reinport();
            clrscr();
        }
        if (inport(PK_DOWN))
        {
            index++;
            if (index > 1)
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
            case 0:
                if (player->gold >= 25)
                {
                    clrscr();
                    charaInput(status);
                    player->gold -= 25;
                    return;
                }
                else
                {
                    gotoxy(80, 15);
                    printf("お金は足りない！");
                    break;
                }               
            case 1:
                clrscr();
                map->isCheck = 2;
                return;
            default:
                break;
            }
        }
    }
}

void randomRoom(PLAYER* player, MAPS* map, STATUS* status) {
    logs("謎のドーアを見つかりました\n");
    logsRead();
    gotoxy(wherex(), wherey());
    msleep(500);
    printf(".");
    msleep(500);
    printf(".");
    msleep(500);
    printf(".");
    msleep(1000);
    int index = 0;
    clrscr();
    while (1)
    {
        reinport();
        for (int i = 6; i < 25; i++)
        {
            gotoxy(45, i);
            printf("┃");
        }
        for (int i = 6; i < 25; i++)
        {
            gotoxy(72, i);
            printf("┃");
        }
        gotoxy(45, 5);
        printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
        gotoxy(45, 25);
        printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
        gotoxy(50, 7);
        printf("ドーアを開けたいか？");
        switch (index)
        {
        case 0:
            gotoxy(53, 15);
            textattr(0x70);
            printf("はい");
            textattr(0x0F);
            gotoxy(53, 17);
            printf("いいえ");
            rewind(stdin);
            break;
        case 1:
            gotoxy(53, 15);
            printf("はい");
            gotoxy(53, 17);
            textattr(0x70);
            printf("いいえ");
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
                index = 1;
            }
            reinport();
            clrscr();
        }
        if (inport(PK_DOWN))
        {
            index++;
            if (index > 1)
            {
                index = 0;
            }
            reinport();
            clrscr();
        }

        if (inport(PK_ENTER))
        {
            int a;
            int g;
            int blood;
            switch (index)
            {
            case 0:
                srand(time(NULL));
                a = rand() % 5;
                switch (a)
                {
                case 0:
                    for (int i = 0; i < G_status; i++)
                    {
                        (status + i)->hp = (status + i)->maxHp;
                    }
                    gotoxy(60, 27);
                    printf("全てのキャラが回復した！ENTERを押してください...");
                    rewind(stdin);
                    getchar();
                    return;
                case 1:
                    player->potionNum += 1;
                    gotoxy(60, 27);
                    printf("回復薬を入手した！ENTERを押してください...");
                    rewind(stdin);
                    getchar();
                    return;
                case 2:
                    srand(time(NULL));
                    g = rand() % 20 + 5;
                    player->gold += g;
                    gotoxy(60, 27);
                    printf("%d Gを入手した！ENTERを押してください...",g);
                    rewind(stdin);
                    getchar();
                    return;
                case 3:
                    clrscr();
                    enemyRoom(enemy);
                    return;
                case 4:
                    srand(time(NULL));
                    blood = rand() % 100 + 1;
                    for (int i = 0; i < G_status; i++)
                    {
                        (status + i)->hp -= blood;
                    }
                    gotoxy(60, 27);
                    printf("毒ガス！全てのキャラを%d点受けられた！ENTERを押してください...", blood);
                    rewind(stdin);
                    getchar();
                    return;
                default:
                    break;
                }

            case 1:
                clrscr();
                map->isCheck = 2;
                return;
            default:
                break;
            }
        }
    }
}

void charaInput(STATUS* status) {
    clrscr();
    while (G_status >= MAX_STATUS)
    {
        gotoxy(35, 10);
        printf("あなたのキャラは多すぎる。一名を解雇してください。");
        gotoxy(35, 12);
        printf("ENTERを押してください...");
        rewind(stdin);
        getchar();
        charaOutput(status, player);
    }
    int len;
    int seedNum = 0;
    printf("名前を入力してください>>(全角15字以内、半角英数字30字以内)");
    rewind(stdin);
    scanf("%s", (status + G_status)->name);

    len = strlen((status + G_status)->name);
    for (int i = 0; i < len; i++)
    {
        if (i % 2 == 0)
        {
            seedNum += *((status + G_status)->name + i);
            seedNum /= i + 1;
        }
        else
        {
            seedNum -= *((status + G_status)->name + i);
            seedNum *= i + 1;
        }
    }
    if (seedNum < 0)
    {
        seedNum = (-seedNum);
    }
    seedNum *= 111;
    (status + G_status)->hp = seedNum % 200 + 100;
    (status + G_status)->atk = (seedNum / 7) % 66 + 25;
    (status + G_status)->def = (seedNum / 3) % 30 + 10;
    (status + G_status)->spd = (seedNum / 4) % 10;
    (status + G_status)->eva = (seedNum / 5) % 50;
    (status + G_status)->cri = (seedNum / 6) % 80;
    (status + G_status)->isBattle = 0;
    (status + G_status)->maxHp = (status + G_status)->hp;

    printf("キャラ作成中.");
    msleep(500);
    printf(".");
    msleep(500);
    printf(".\n");
    msleep(1000);
    printf("NAME: %s\n\n", (status + G_status)->name);
    printf("HP/MAXHP: %d/%d\n", (status + G_status)->hp, (status + G_status)->maxHp);
    printf("ATK: %d\n", (status + G_status)->atk);
    printf("DEF: %d\n", (status + G_status)->def);
    printf("SPD: %d\n", (status + G_status)->spd);
    printf("EVA: %d\n", (status + G_status)->eva);
    printf("CRI: %d\n\n", (status + G_status)->cri);
    printf("作成完了！ENTERを押してください...");
    G_status += 1;
    rewind(stdin);
    getchar();
}

void charaOutput(STATUS* status, PLAYER* player) {
    clrscr();
    int index = 0;
    int index0 = 0;
    for (int i = 0; i < G_status; i++)
    {
        if ((status + i)->hp<= 0)
        {
            (status + i)->hp = 0;
            (status + i)->isBattle = 2;
        }
        
    }
    while (1)
    {
        reinport();
        for (int i = 3; i < 30; i++)
        {
            gotoxy(5, i);
            printf("┃");
        }
        gotoxy(5, 2);
        printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
        gotoxy(5, 30);
        printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
        for (int i = 3; i < 30; i++)
        {
            gotoxy(60, i);
            printf("┃");
        }
        for (int i = 3; i < 30; i++)
        {
            gotoxy(115, i);
            printf("┃");
        }
        gotoxy(60, 2);
        printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
        gotoxy(60, 30);
        printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
        gotoxy(5, 1);
        printf("お金：%d G", player->gold);

        for (int i = 0; i < G_status; i++)
        {
            if (i == index)
            {
                textattr(0x70);
            }
            gotoxy(7, i + 3);
            printf("%d.%s", i + 1, (status + i)->name);
            textattr(0x0F);
        }
        switch (index0)
        {
        case 0:
            gotoxy(65, 20);
            textattr(0x70);
            printf("回復薬を使う");
            textattr(0x0F);
            gotoxy(85, 20);
            printf("出戦");
            gotoxy(95, 20);
            printf("解雇");
            gotoxy(65, 21);
            printf("(残りは：%d)", player->potionNum);
            gotoxy(83, 21);
            if ((status + index)->isBattle == 0)
            {
                printf("(休憩中)");
            }
            if ((status + index)->isBattle == 1)
            {
                printf("(出戦中)");
            }
            if ((status + index)->isBattle == 2)
            {
                printf("(戦闘不能)");
            }
            break;
        case 1:
            gotoxy(65, 20);            
            printf("回復薬を使う");           
            textattr(0x70);
            gotoxy(85, 20);           
            printf("出戦");
            textattr(0x0F);
            gotoxy(95, 20);
            printf("解雇");
            gotoxy(65, 21);
            printf("(残りは：%d)", player->potionNum);
            gotoxy(83, 21);
            if ((status + index)->isBattle == 0)
            {
                printf("(休憩中)");
            }
            if ((status + index)->isBattle == 1)
            {
                printf("(出戦中)");
            }
            if ((status + index)->isBattle == 2)
            {
                printf("(戦闘不能)");
            }
            break;
        case 2:
            gotoxy(65, 20);
            printf("回復薬を使う");            
            gotoxy(85, 20);                        
            printf("出戦");
            textattr(0x70);
            gotoxy(95, 20);
            printf("解雇");
            textattr(0x0F);
            gotoxy(65, 21);
            printf("(残りは：%d)", player->potionNum);
            gotoxy(83, 21);
            if ((status + index)->isBattle == 0)
            {
                printf("(休憩中)");
            }
            if ((status + index)->isBattle == 1)
            {
                printf("(出戦中)");
            }
            if ((status + index)->isBattle == 2)
            {
                printf("(戦闘不能)");
            }
            break;
        default:
            break;
        }
        if (inport(PK_UP))
        {
            index--;
            if (index < 0)
            {
                index = G_status - 1;
            }
            reinport();
            clrscr();
        }
        if (inport(PK_DOWN))
        {
            index++;
            if (index > G_status - 1)
            {
                index = 0;
            }
            reinport();
            clrscr();
        }
        if (inport(PK_LEFT))
        {
            index0--;
            if (index0 < 0)
            {
                index0 = 2;
            }
            reinport();
            clrscr();
        }
        if (inport(PK_RIGHT))
        {
            index0++;
            if (index0 > 2)
            {
                index0 = 0;
            }
            reinport();
            clrscr();
        }
        if (inport(PK_ENTER))
        {
            switch (index0)
            {
            case 0:
                if ((status + index)->hp == (status + index)->maxHp)
                {
                    gotoxy(62, 23);
                    printf("回復薬を使う必要はない！ENTERを押してください...");
                    rewind(stdin);
                    getchar();
                }
                else
                {
                    (status + index)->hp = (status + index)->maxHp;                    
                    player->potionNum -= 1;
                    (status + index)->isBattle = 0;
                }
                break;
            case 1:
                if ((status + index)->isBattle == 0)
                {
                    for (int i = 0; i < G_status; i++)
                    {
                        (status + i)->isBattle = 0;
                    }
                    (status + index)->isBattle = 1;
                }                
                break;
            case 2:
                for (int i = index; i < G_status; i++)
                {
                    *(status + i) = *(status + i + 1);
                }
                G_status -= 1; 
                break;
            default:
                break;
            }          
            reinport();
            clrscr();
        }
        if (inport(PK_ESC))
        {
            reinport();
            clrscr();
            return;
        }
        gotoxy(65, 4);
        printf("HP/MAXHP: %d/%d\n", (status + index)->hp, (status + index)->maxHp);
        gotoxy(65, 6);
        printf("ATK: %d\n", (status + index)->atk);
        gotoxy(65, 8);
        printf("DEF: %d\n", (status + index)->def);
        gotoxy(65, 10);
        printf("SPD: %d\n", (status + index)->spd);
        gotoxy(65, 12);
        printf("EVA: %d\n", (status + index)->eva);
        gotoxy(65, 14);
        printf("CRI: %d\n\n", (status + index)->cri);
    }
}

void enemyRoom(STATUS* enemy) {
    logs("なんか変な息がある\n");
    logsRead();
    gotoxy(wherex(), wherey());
    msleep(500);
    printf(".");
    msleep(500);
    printf(".");
    msleep(500);
    printf(".");
    msleep(1000);
    clrscr();
    int index = 0;
    char KATAGANA[512][10] = { "ア", "イ", "ウ", "エ", "オ", "カ", "キ", "ク", "ケ", "コ", "サ", "シ", "ス", "セ", "ソ", "タ", "チ", "ツ", "テ", "ト", "ナ", "ニ", "ヌ", "ネ", "ノ", "ハ", "ヒ", "フ", "ヘ", "ホ", "マ", "ミ", "ム", "メ", "モ", "ヤ", "ユ", "ヨ", "ラ", "リ", "ル", "レ", "ロ", "ワ", "ヲ", "ン", "ガ", "ギ", "グ", "ゲ", "ゴ", "ザ", "ジ", "ズ", "ゼ", "ゾ", "ダ", "ジ", "ヅ", "デ", "ド", "バ","ビ", "ブ", "ベ", "ボ", "パ", "ピ", "プ", "ペ", "ポ" };
    srand(time(NULL));
    int num = rand() % 10 + 1;
    for (int i = 0; i < num; i++)
    {
        int roll = rand() % 77;
        char a[3];
        strcpy(a, KATAGANA[roll]);
        strcat(enemy->name, a);
    }
    
    int len;
    int seedNum = 0;
    len = strlen(enemy->name);
    for (int i = 0; i < len; i++)
    {
        if (i % 2 == 0)
        {
            seedNum += *(enemy->name + i);
            seedNum /= i + 1;
        }
        else
        {
            seedNum -= *(enemy->name + i);
            seedNum *= i + 1;
        }
    }
    if (seedNum < 0)
    {
        seedNum = (-seedNum);
    }
    seedNum *= 111;
    enemy->hp = seedNum % 200 + 100;
    enemy->atk = (seedNum / 7) % 66 + 25;
    enemy->def = (seedNum / 3) % 30 + 10;
    enemy->spd = (seedNum / 4) % 10;
    enemy->eva = (seedNum / 5) % 50;
    enemy->cri = (seedNum / 6) % 80;
    enemy->maxHp = enemy->hp;
    while (1)
    {
        reinport();
        for (int i = 3; i < 30; i++)
        {
            gotoxy(1, i);
            printf("┃");
        }
        for (int i = 3; i < 30; i++)
        {
            gotoxy(56, i);
            printf("┃");
        }
        gotoxy(1, 2);
        printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
        gotoxy(1, 30);
        printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
        gotoxy(2, 3);
        printf("あ！やせいの %s ", enemy->name);
        gotoxy(2, 4);
        printf("がとびだしてきた！");
        gotoxy(3, 6);
        printf("HP/MAXHP: %d/%d\n", enemy->hp, enemy->maxHp);
        gotoxy(3, 7);
        printf("ATK: %d\n", enemy->atk);
        gotoxy(3, 8);
        printf("DEF: %d\n", enemy->def);
        gotoxy(3, 9);
        printf("SPD: %d\n", enemy->spd);
        gotoxy(3, 10);
        printf("EVA: %d\n", enemy->eva);
        gotoxy(3, 11);
        printf("CRI: %d\n\n", enemy->cri);

        for (int i = 10; i < 25; i++)
        {
            gotoxy(70, i);
            printf("┃");
        }
        for (int i = 10; i < 25; i++)
        {
            gotoxy(110, i);
            printf("┃");
        }
        gotoxy(70, 9);
        printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
        gotoxy(70, 25);
        printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
        
        int j = 0;
        for (int i = 0; i < G_status; i++)
        {            
            if ((status + i)->hp == 0)
            {
                j++;
            }
            if (j == G_status)
            {
                clrscr();
                gotoxy(55, 15);
                printf("GAME OVER");
                getchar();
                clrscr();
                title();
            }
        }

        gotoxy(87, 12);
        printf("どうする？");
        switch (index)
        {
        case 0:
            gotoxy(87, 15);
            textattr(0x70);
            printf("たたかう");
            textattr(0x0F);
            gotoxy(84, 17);
            printf("状況を確認する");
            gotoxy(88, 19);
            printf("にげる");
            rewind(stdin);
            break;
        case 1:
            gotoxy(87, 15);            
            printf("たたかう");           
            textattr(0x70);
            gotoxy(84, 17);
            printf("状況を確認する");
            textattr(0x0F);
            gotoxy(88, 19);
            printf("にげる");
            rewind(stdin);
            break;
        case 2:
            gotoxy(87, 15);            
            printf("たたかう");            
            gotoxy(84, 17);
            printf("状況を確認する");
            textattr(0x70);
            gotoxy(88, 19);
            printf("にげる");
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
            case 0:
                battle(status, enemy, player);
                break;
            case 1:
                charaOutput(status, player);
                break;
            case 2:
                if (enemy->hp != enemy->maxHp)
                {
                    srand(time(NULL));
                    int g = rand() % 20 + 1;
                    player->gold += g;
                }
                clrscr();
                return;
            default:
                break;
            }
        }

        if (enemy->hp == 0)
        {
            srand(time(NULL));
            int g = rand() % 30 + 10;
            player->gold += g;
            clrscr();
            while (1)
            {
                reinport();
                for (int i = 6; i < 25; i++)
                {
                    gotoxy(45, i);
                    printf("┃");
                }
                for (int i = 6; i < 25; i++)
                {
                    gotoxy(72, i);
                    printf("┃");
                }
                gotoxy(45, 5);
                printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
                gotoxy(45, 25);
                printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
                gotoxy(47, 7);
                printf("%s", enemy->name);
                gotoxy(47, 8);
                printf("は君の強さに感動させた");
                gotoxy(47, 10);
                printf("君のチームに参加したい");
                gotoxy(47, 12);
                printf("雇うか？");
                gotoxy(47, 20);
                printf("%d Gを入手した！", g);
                switch (index)
                {
                case 0:
                    gotoxy(53, 15);
                    textattr(0x70);
                    printf("はい");
                    textattr(0x0F);
                    gotoxy(53, 17);
                    printf("いいえ");
                    rewind(stdin);
                    break;
                case 1:
                    gotoxy(53, 15);
                    printf("はい");
                    gotoxy(53, 17);
                    textattr(0x70);
                    printf("いいえ");
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
                        index = 1;
                    }
                    reinport();
                    clrscr();
                }
                if (inport(PK_DOWN))
                {
                    index++;
                    if (index > 1)
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
                    case 0:
                        while (G_status >= MAX_STATUS)
                        {
                            gotoxy(35, 27);
                            printf("あなたのキャラは多すぎる。一名を解雇してください。");
                            gotoxy(35, 28);
                            printf("ENTERを押してください...");
                            rewind(stdin);
                            getchar();
                            charaOutput(status, player);
                        }
                        strcpy((status + G_status)->name, enemy->name);
                        (status + G_status)->hp = 1;
                        (status + G_status)->atk = enemy->atk;
                        (status + G_status)->def = enemy->def;
                        (status + G_status)->spd = enemy->spd;
                        (status + G_status)->eva = enemy->eva;
                        (status + G_status)->cri = enemy->cri;
                        (status + G_status)->isBattle = 0;
                        (status + G_status)->maxHp = enemy->maxHp;
                        G_status += 1;
                        gotoxy(60, 27);
                        printf("%sは加入した！", enemy->name);
                        gotoxy(60, 28);
                        printf("ENTERを押してください...");
                        rewind(stdin);
                        getchar();
                        return;
                    case 1:
                        clrscr();
                        return;
                    default:
                        break;
                    }
                }
            }
            
        }
    }
}

void battle(STATUS* status, STATUS* enemy, PLAYER* player) {
    int t = 0;
    for (int i = 0; i < G_status; i++)
    {
        if ((status + i)->isBattle == 1)
        {
            t = i;
            break;
        }
        if (i == (G_status - 1) && (status + i)->isBattle == 0)
        {
            gotoxy(72, 27);
            printf("出戦のキャラを選択してください！");
            return;
        }       
    }
    clrscr();
    reinport();

    gotoxy(31, 3);
    printf("戦闘開始！");
    if ((status + t)->spd >= enemy->spd)
    {
        gotoxy(31, 5);
        printf("%sの先攻！", (status + t)->name);
        while (1)
        {
            battleS(status + t, enemy);
            msleep(2000);
            if ((status + t)->hp == 0)
            {
                gotoxy(31, wherey() + 2);
                printf("戦闘失敗...ENTERを押してください...");
                rewind(stdin);
                getchar();
                clrscr();
                return;
            }
            if (enemy->hp == 0)
            {
                gotoxy(31, wherey() + 2);
                printf("戦闘勝利！！ENTERを押してください...");
                rewind(stdin);
                getchar();
                clrscr();
                return;
            }
            battleS(enemy, status + t);
            msleep(2000);
            if ((status + t)->hp == 0)
            {
                gotoxy(31, wherey() + 2);
                printf("戦闘失敗...ENTERを押してください...");
                rewind(stdin);
                getchar();
                clrscr();
                return;
            }
            if (enemy->hp == 0)
            {
                gotoxy(31, wherey() + 2);
                printf("戦闘勝利！！ENTERを押してください...");
                rewind(stdin);
                getchar();
                clrscr();
                return;
            }
        }
    }
    if ((status + t)->spd < enemy->spd)
    {
        gotoxy(31, 5);
        printf("%sの先攻！", enemy->name);
        while (1)
        {
            battleS(enemy, status + t);            
            msleep(2000);
            if ((status + t)->hp == 0)
            {
                gotoxy(31, wherey() + 2);
                printf("戦闘失敗...ENTERを押してください...");
                rewind(stdin);
                getchar();
                clrscr();
                return;
            }
            if (enemy->hp == 0)
            {
                gotoxy(31, wherey() + 2);
                printf("戦闘勝利！！ENTERを押してください...");
                rewind(stdin);
                getchar();
                clrscr();
                return;
            }
            battleS(status + t, enemy);
            msleep(2000);
            if ((status + t)->hp == 0)
            {
                gotoxy(31, wherey() + 2);
                printf("戦闘失敗...ENTERを押してください...");
                rewind(stdin);
                getchar();
                clrscr();
                return;
            }
            if (enemy->hp == 0)
            {
                gotoxy(31, wherey() + 2);
                printf("戦闘勝利！！ENTERを押してください...");
                rewind(stdin);
                getchar();
                clrscr();
                return;
            }
        }
    }
}

void battleS(STATUS* status, STATUS* enemy) {
    srand(time(NULL));
    gotoxy(31, wherey() + 1);
    printf("%sの 攻撃！", status->name);
    if (rand() % 100 < enemy->eva) 
    {
        gotoxy(31, wherey() + 1);
        printf("MISS!\n");
    }
    else 
    {
        if (rand() % 100 < status->cri)
        {
            int damage = (status->atk) * 2 - (enemy->def);
            gotoxy(31, wherey() + 1);           
            printf("会心の一撃！！");
            gotoxy(31, wherey() + 1);
            printf("%s に%dのダメージ！！", enemy->name, damage);
            enemy->hp -= damage;
            if (enemy->hp <= 0)
            {
                enemy->hp = 0;
                gotoxy(31, wherey() + 1);
                printf("%s は息絶えた．．．", enemy->name);
            }
            else
            {
                gotoxy(31, wherey() + 1);
                printf("%s の残りHPは%d\n", enemy->name, enemy->hp);
            }            
        }
        else
        {
            int damage = (status->atk) - (enemy->def);
            if (damage < 0)
            {
                damage = 0;
            }
            gotoxy(31, wherey() + 1);
            printf("%s に%dのダメージ！！", enemy->name, damage);
            enemy->hp -= damage;
            if (enemy->hp <= 0)
            {
                enemy->hp = 0;
                gotoxy(31, wherey() + 1);
                printf("%s は息絶えた．．．", enemy->name);
            }
            else
            {
                gotoxy(31, wherey() + 1);
                printf("%s の残りHPは%d\n", enemy->name, enemy->hp);
            }
        }
    }

}

void saveGame(PLAYER* player, MAPS* map, STATUS* status) {
    unsigned char status_count = G_status;
    FILE* fp = fopen("charaters.bin", "wb");
    fwrite(&status_count, sizeof(status_count), 1, fp);
    fwrite(status, sizeof(*status), status_count, fp);
    fclose(fp);

    FILE* fp1 = fopen("player.bin", "wb");
    fwrite(player, sizeof(*player), 1, fp1);
    fclose(fp1);

    FILE* fp2 = fopen("floor.bin", "wb");
    fwrite(&G_floor, sizeof(G_floor), 1, fp2);
    fclose(fp2);

    unsigned char map_count = G_checkMapNum;
    FILE* fp3 = fopen("maps.bin", "wb");
    fwrite(&map_count, sizeof(map_count), 1, fp3);
    fwrite(map, sizeof(*map), map_count, fp3);
    fclose(fp3);

    gotoxy(60, 27);
    printf("保存中");
    msleep(500);
    printf(".");
    msleep(500);
    printf(".");
    msleep(500);
    printf(".");
    msleep(1000);
    printf("成功！！ENTERを押してください...");
    rewind(stdin);
    getchar();
    clrscr();
}

void loadGame(PLAYER* player, MAPS* map, STATUS* status) {
    unsigned char status_count = G_status;
    FILE* fp = fopen("charaters.bin", "rb");
    if (fp == NULL)
    {
        gotoxy(70, 17);
        printf("（アーカイブがありません。）\n");
    }
    else
    {
        fread(&status_count, sizeof(status_count), 1, fp);
        fread(status, sizeof(*status), status_count, fp);
        G_status = status_count;
        fclose(fp);
    }

    FILE* fp1 = fopen("player.bin", "rb");
    if (fp1 == NULL)
    {
        //printf("player.binがありません。\n");
    }
    else
    {
        fread(player, sizeof(*player), 1, fp1);
        fclose(fp1);
    }

    FILE* fp2 = fopen("floor.bin", "rb");
    if (fp2 == NULL)
    {
        //printf("floor.binがありません。\n");
    }
    else
    {
        fread(&G_floor, sizeof(G_floor), 1, fp2);
        fclose(fp2);
    }
    switch (G_floor)
    {
    case 1:
        mapsPaint1();
    case 2:
        mapsPaint2();
    case 3:
        mapsPaint3();
    default:
        break;
    }

    unsigned char map_count = G_checkMapNum;
    FILE* fp3 = fopen("maps.bin", "rb");
    if (fp3 == NULL)
    {
        printf("maps.binがありません。\n");
        return;
    }
    else
    {
        fread(&map_count, sizeof(map_count), 1, fp3);
        fread(map, sizeof(*map), map_count, fp3);
        G_checkMapNum = map_count;
        fclose(fp3);
    }
    clrscr();
    while (1)
    {
        moveMent(player, map);
        logsRead();
    }

    getchar();
}

void endGame() {
    clrscr();
    int index = 0;
    while (1)
    {
        reinport();
        for (int i = 6; i < 25; i++)
        {
            gotoxy(45, i);
            printf("┃");
        }
        for (int i = 6; i < 25; i++)
        {
            gotoxy(72, i);
            printf("┃");
        }
        gotoxy(45, 5);
        printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
        gotoxy(45, 25);
        printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
        gotoxy(50, 7);
        printf("ゲームを終了させて");
        gotoxy(54, 8);
        printf("いいですか？");
        switch (index)
        {
        case 0:
            gotoxy(53, 15);
            textattr(0x70);
            printf("はい");
            textattr(0x0F);
            gotoxy(53, 17);
            printf("いいえ");
            rewind(stdin);
            break;
        case 1:
            gotoxy(53, 15);
            printf("はい");
            gotoxy(53, 17);
            textattr(0x70);
            printf("いいえ");
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
                index = 1;
            }
            reinport();
            clrscr();
        }
        if (inport(PK_DOWN))
        {
            index++;
            if (index > 1)
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
            case 0:
                exit(0);
            case 1:
                clrscr();
                return;
            default:
                break;
            }
        }
    }

}

//cursorを隠させて
void hideCursor() {
    CONSOLE_CURSOR_INFO cur;
    cur.dwSize = 1;
    cur.bVisible = 0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cur);
}