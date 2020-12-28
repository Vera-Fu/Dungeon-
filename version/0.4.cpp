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
    int isCheck;        //此区域是否已清除
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
}STATUS;


//------------声明结构体------------
MAPS map[MAX_MAP];
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
void endGame();             //结束游戏功能
void runGame();             //开始游戏
void moveMent(PLAYER* player, MAPS* map);       //读取玩家的输入
void mapsPaint(MAPS* map);  //区域绘制                    
void mapsType(MAPS* map);   //区域类型设置
void mapsPaint1();          //关卡地图绘制
void logs(const char* c);   //logs保存
void logsRead();            //logs打印
void charaInput(STATUS* status);

void enemyBattle(STATUS* enemy);

void hideCursor();          //cursorを隠させて

int main() {
    enemyBattle(enemy);
    charaInput(status);
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

void runGame() {
    clrscr();
    G_floor = 1;
    G_checkMapNum = 1;
    PLAYER player = { 5, 9, 0, 0, 10};
    mapsPaint1();
    while (1)
    {
        moveMent(&player, map);
        logsRead();
    }
    
    getchar();
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
                break;
            case 2:
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

void moveMent(PLAYER* player, MAPS* map) {   
    gotoxy(player->x, player->y);
    printf("♀");
    //当该区域已被探索，便将其显示
    for (int i = 0; i < MAX_MAP; i++)
    {
        if ((map + i)->isCheck == 1)
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
                if ((map + i)->isCheck == 0)
                {
                    (map + i)->isCheck = 1;
                    G_checkMapNum += 1;
                }
                logs("moveup\n");
                G_logs += 1;
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
                if ((map + i)->isCheck == 0)
                {
                    (map + i)->isCheck = 1;
                    G_checkMapNum += 1;
                }
                logs("movedown\n");
                G_logs += 1;
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
                if ((map + i)->isCheck == 0)
                {
                    (map + i)->isCheck = 1;
                    G_checkMapNum += 1;
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
                if ((map + i)->isCheck == 0)
                {
                    (map + i)->isCheck = 1;
                    G_checkMapNum += 1;
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
        //msleep(10);
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
        if (a >= 40 && a < 50)
        {
            map->index = 4;
        }
        if (a >= 50 && a < 65)
        {
            map->index = 5;
        }
        if (a >= 65 && a < 100)
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

void charaInput(STATUS* status) {
    if (G_status > MAX_STATUS)
    {

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
    (status + G_status)->atk = (seedNum / 2) % 70 + 30;
    (status + G_status)->def = (seedNum / 3) % 30 + 10;
    (status + G_status)->spd = (seedNum / 4) % 10;
    (status + G_status)->eva = (seedNum / 5) % 50;
    (status + G_status)->cri = (seedNum / 6) % 100;
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
    printf("作成完了！ENTERを押して...");
    rewind(stdin);
    getchar();
}

void enemyBattle(STATUS* enemy) {
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
                fopen("logs.txt", "w");
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


