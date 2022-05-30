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

//------------�\���̒�`------------
typedef enum {
    DISP_START = 0,
    DISP_LOAD,
    DISP_END,
}SCENE;

typedef struct {
    int x;              //�v���C���[�̍��Wx
    int y;              //�v���C���[�̍��Wy
    int potionNum;      //�񕜖�̐�
    int gold;           //�S�[���h�̐�
}PLAYER;

typedef struct {
    int x;              //���̋��̍��Wx
    int y;              //���̋��̍��Wy
    int index;          //���^�C�v:1.���X 2.���ł��Ȃ� 3.�����񕜉� 4.��W�� 5.�����_���� 6.�G�̉� 7.boss�̉� 8.�K�i 9.���߂Ẳ�
    int isCheck;        //�u���̋��̓N���A�������H�v�̔���@0�F�܂��T�����Ă��Ȃ� 1�F�J��Ԃ����Ƃ͕s�� 2�F�J��Ԃ����Ƃ͉�
    int isUp;           //�u��Ɉړ����邱�Ƃ��ł��邩�H�v�̔���
    int isDown;         //�u���Ɉړ����邱�Ƃ��ł��邩�H�v�̔���
    int isLeft;         //�u���Ɉړ����邱�Ƃ��ł��邩�H�v�̔���
    int isRight;        //�u�E�Ɉړ����邱�Ƃ��ł��邩�H�v�̔���
}MAPS;

typedef struct {
    char name[65];      //�L�����̖��O
    int maxHp;          //�L������hp�̍ő�l
    int hp;             //�L������hp
    int atk;            //�L�����̍U����
    int def;            //�L�����̖h�q��
    int spd;            //�L�����̃X�s�[�h
    int eva;            //�L�����̉��
    int cri;            //�L�����̉�S��
    int isBattle;       //0�͋x�e���A1�͏o�풆�A2�͐퓬�s�\
}STATUS;


//------------�\���̐錾------------
MAPS map[MAX_MAP];      //�}�b�v�̃f�[�^��ۑ�����
PLAYER player[1];       //�v���C���[�̃f�[�^��ۑ�����
STATUS status[MAX_STATUS];      //�L�����̃f�[�^��ۑ�����
STATUS enemy[1];        //�G�̃f�[�^��ۑ�����
STATUS boss[1];         //boss�̃f�[�^��ۑ�����

//------------�O���[�o���ϐ��錾------------
int G_floor;            //�u���v���C���[�͂ǂ̃��x���ɂ��邩�v�̔���
int G_checkMapNum;      //���̃��x���ŒT���������̐�
int G_logs;             //logs�̍s��
int G_status;           //�������Ă���L�����̐�
char bgm[] = "The Kingdom.mp3";
int BGM = opensound(bgm);
char dead[] = "dead.mp3";
int DEAD = opensound(dead);

//------------�֐��錾------------
void title();               //�^�C�g�����
void menu();                //���j���[���
void runGame();             //�Q�[���J�n
void moveMent(PLAYER* player, MAPS* map);       //�v���C���[�̓��͂�ǂݏo��

void mapsPaint(MAPS* map);  //����`��                    
void mapsType(MAPS* map);   //���̃^�[�v�ݒ�
void mapsPaint1();          //���x��1�̃}�b�v�ݒ�
void mapsPaint2();          //���x��2�̃}�b�v�ݒ�
void mapsPaint3();          //���x��3�̃}�b�v�ݒ�

void logs(const char* c);   //logs�ۑ�
void logsRead();            //logs���o�͂���

void room(MAPS* map);       //�u���̋�悪�ǂ̃^�C�v�v�̔���
void shop(PLAYER* player, MAPS* map, STATUS* status);               //���X�̏���
void healRoom(STATUS* status, MAPS* map);                           //�����񕜉��̏���
void charaRoom(PLAYER* player, MAPS* map, STATUS* status);          //��W���̏���
void randomRoom(PLAYER* player, MAPS* map, STATUS* status);         //�����_�����̏���
void enemyRoom(PLAYER* player, MAPS* map, STATUS* status, STATUS* enemy);      //�G�̉��̏���
void bossRoom(PLAYER* player, MAPS* map, STATUS* status, STATUS* enemy);       //boss�̉��̏���
void nextFloorRoom(PLAYER* player, MAPS* map, STATUS* status);      //�K�i�̏���

void charaInput(STATUS* status);        //�L��������͂���
void charaOutput(STATUS* status, PLAYER* player);       //�L�������o�͂���

void battle(STATUS* status, STATUS* enemy, PLAYER* player);     //�퓬����1
void battleS(STATUS* status, STATUS* enemy);                    //�퓬����2

void saveGame(PLAYER* player, MAPS* map, STATUS* status, STATUS* boss);     //�Z�[�u�Q�[��
void loadGame(PLAYER* player, MAPS* map, STATUS* status, STATUS* boss);     //���[�h�Q�[��

void endGame();             //�Q�[���I���̏���

void hideCursor();          //cursor���B������

int main() {    
    hideCursor();
    title();

    return 0;
}

//�^�C�g�����
void title() {
    playsound(BGM, 1);
    fopen("logs.txt", "w");
    clrscr();
    int index = 0;
    while (1)
    {
        reinport();
        gotoxy(50, 5);
        printf("��������������������������������");
        gotoxy(50, 6);
        printf("��%s��", "  ��Dungeon�� ");
        gotoxy(50, 7);
        printf("��������������������������������");
        switch (index)
        {
        case DISP_START:
            gotoxy(53, 15);
            textattr(0x70);
            printf("�Q�[���J�n");
            textattr(0x0F);
            gotoxy(52, 17);
            printf("�Q�[�����[�h");
            gotoxy(53, 19);
            printf("�Q�[���I��");
            rewind(stdin);
            break;
        case DISP_LOAD:
            gotoxy(53, 15);
            printf("�Q�[���J�n");
            gotoxy(52, 17);
            textattr(0x70);
            printf("�Q�[�����[�h");
            gotoxy(53, 19);
            textattr(0x0F);
            printf("�Q�[���I��");
            rewind(stdin);
            break;
        case DISP_END:
            gotoxy(53, 15);
            printf("�Q�[���J�n");
            gotoxy(52, 17);
            printf("�Q�[�����[�h");
            gotoxy(53, 19);
            textattr(0x70);
            printf("�Q�[���I��");
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
                loadGame(player, map, status, boss);
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

//���j���[���
void menu() {
    int index = 0;      //0�F�Â� 1�F�Z�[�u�Q�[�� 2�F�L�������o�͂��� 3�F�Q�[���I��
    clrscr();
    while (1)
    {
        reinport();
        for (int i = 6; i < 25; i++)
        {
            gotoxy(45, i);
            printf("��");
        }
        for (int i = 6; i < 25; i++)
        {
            gotoxy(72, i);
            printf("��");
        }
        gotoxy(45, 5);
        printf("��������������������������������������������������������");
        gotoxy(45, 25);
        printf("��������������������������������������������������������");
        gotoxy(53, 7);
        printf("����MENU����");
        switch (index)
        {
        case 0:
            gotoxy(53, 15);
            textattr(0x70);
            printf("�Â�");
            textattr(0x0F);
            gotoxy(53, 17);
            printf("�Z�[�u");
            gotoxy(53, 19);
            printf("�L�����X�e�[�^�X");
            gotoxy(53, 21);
            printf("�Q�[���I��");
            rewind(stdin);
            break;
        case 1:
            gotoxy(53, 15);
            printf("�Â�");
            gotoxy(53, 17);
            textattr(0x70);
            printf("�Z�[�u");
            textattr(0x0F);
            gotoxy(53, 19);
            printf("�L�����X�e�[�^�X");
            gotoxy(53, 21);
            printf("�Q�[���I��");
            rewind(stdin);
            break;
        case 2:
            gotoxy(53, 15);
            printf("�Â�");
            gotoxy(53, 17);
            printf("�Z�[�u");
            gotoxy(53, 19);
            textattr(0x70);
            printf("�L�����X�e�[�^�X");
            textattr(0x0F);
            gotoxy(53, 21);
            printf("�Q�[���I��");
            rewind(stdin);
            break;
        case 3:
            gotoxy(53, 15);
            printf("�Â�");
            gotoxy(53, 17);
            printf("�Z�[�u");
            gotoxy(53, 19);
            printf("�L�����X�e�[�^�X");
            gotoxy(53, 21);
            textattr(0x70);
            printf("�Q�[���I��");
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
                saveGame(player, map, status, boss);
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

//�Q�[���J�n
void runGame() {
    clrscr();
    printf("�V�l���H�߂��炵����...\n\n");
    msleep(2000);
    printf("�V�l��Dungeon�̎��ҁE�M���K���b�V���E���C�[�_���疳���ȃL�������^������...\n\n");
    msleep(3500);
    printf("GOOD LUCK...");
    msleep(2000);
    clrscr();
    charaInput(status);
    charaInput(status);
    clrscr();

    //������
    G_floor = 1;
    G_checkMapNum = 1;
    player[0] = { 5, 9, 1, 30 };    
    boss[0] = {"Dungeon�̎��ҁE�M���K���b�V���E���C�[�_", 1000, 1000, 100, 50, 10, 5, 10, 1};
    
    mapsPaint1();
    while (1)
    {
        moveMent(player, map);
        logsRead();
    }

    getchar();
}

//�v���C���[�̓��͂�ǂݏo��
void moveMent(PLAYER* player, MAPS* map) {
    gotoxy(player->x, player->y);
    printf("��");    
    gotoxy(25, 2);
    printf("%d�w��", G_floor);
    //�������̋�悪�T�����Ă����A�o�͂���
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
    //�v���C���[�͏�Ɉړ����鏈��
    if (inport(PK_UP))
    {
        for (int i = 0; i < MAX_MAP; i++)
        {
            if (player->x == (map + i)->x && player->y - 5 == (map + i)->y && (map + i)->isDown == 1)
            {
                player->y -= 5;
                logs("��Ɉړ�����\n");
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
    //�v���C���[�͉��Ɉړ����鏈��
    if (inport(PK_DOWN))
    {
        for (int i = 0; i < MAX_MAP; i++)
        {
            if (player->x == (map + i)->x && player->y + 5 == (map + i)->y && (map + i)->isUp == 1)
            {
                player->y += 5;
                logs("���Ɉړ�����\n");
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
    //�v���C���[�͍��Ɉړ����鏈��
    if (inport(PK_LEFT))
    {
        for (int i = 0; i < MAX_MAP; i++)
        {
            if (player->x - 10 == (map + i)->x && player->y == (map + i)->y && (map + i)->isRight == 1)
            {
                player->x -= 10;
                logs("���Ɉړ�����\n");
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
    //�v���C���[�͉E�Ɉړ����鏈��
    if (inport(PK_RIGHT))
    {
        for (int i = 0; i < MAX_MAP; i++)
        {
            if (player->x + 10 == (map + i)->x && player->y == (map + i)->y && (map + i)->isLeft == 1)
            {
                player->x += 10;
                logs("�E�Ɉړ�����\n");
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

//����`��
void mapsPaint(MAPS* map) {
    //�������̋��̓^�C�v�������Ă��Ȃ��A���Ƀ^�C�v��^����
    if (map->index == 0)
    {
        mapsType(map);
    }
    if (map->isUp == 1)
    {
        gotoxy(map->x + 2, map->y);
        printf("��");
    }
    if (map->isDown == 1)
    {
        gotoxy(map->x + 2, map->y + 4);
        printf("��");
    }
    if (map->isLeft == 1)
    {
        gotoxy(map->x - 2, map->y + 2);
        printf("��");
    }
    if (map->isRight == 1)
    {
        gotoxy(map->x + 6, map->y + 2);
        printf("��");
    }
    gotoxy(map->x, map->y + 1);
    printf("������");
    gotoxy(wherex() - 6, wherey() + 1);
    printf("��  ��");
    gotoxy(wherex() - 6, wherey() + 1);
    printf("������");
    switch (map->index)
    {
    case 1:
        gotoxy(map->x + 2, map->y + 2);
        printf("��");
        break;
    case 2:
        gotoxy(map->x + 2, map->y + 2);
        printf("��");
        break;
    case 3:
        gotoxy(map->x + 2, map->y + 2);
        printf("��");
        break;
    case 4:
        gotoxy(map->x + 2, map->y + 2);
        printf("��");
        break;
    case 5:
        gotoxy(map->x + 2, map->y + 2);
        printf("�H");
        break;
    case 6:
        gotoxy(map->x + 2, map->y + 2);
        printf("��");
        break;
    case 7:
        gotoxy(map->x + 2, map->y + 2);
        printf("��");
        break;
    case 8:
        gotoxy(map->x + 2, map->y + 2);
        printf("��");
        break;
    case 9:
        gotoxy(map->x + 2, map->y + 2);
        printf("��");
        break;
    default:
        break;
    }

}

//���̃^�[�v�ݒ�
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
        int a = rand() % 100;               
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

//���x��1�̃}�b�v�ݒ�
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

//���x��2�̃}�b�v�ݒ�
void mapsPaint2() {
    gotoxy(5, 10);
    map[0] = { wherex(), wherey() - 1, 9, 1, 0, 1, 0, 1 };      //(x,y,index,isCheck,isUp,isDown,isLeft,isRight);
    gotoxy(5, 15);
    map[1] = { wherex(), wherey() - 1, 0, 0, 1, 0, 0, 0 };
    gotoxy(15, 10);
    map[2] = { wherex(), wherey() - 1, 0, 0, 1, 0, 1, 1 };
    gotoxy(15, 5);
    map[3] = { wherex(), wherey() - 1, 0, 0, 0, 1, 0, 1 };
    gotoxy(25, 5);
    map[4] = { wherex(), wherey() - 1, 0, 0, 0, 0, 1, 0 };
    gotoxy(25, 10);
    map[5] = { wherex(), wherey() - 1, 0, 0, 0, 1, 1, 1 };
    gotoxy(25, 15);
    map[6] = { wherex(), wherey() - 1, 0, 0, 1, 1, 0, 1 };
    gotoxy(25, 20);
    map[7] = { wherex(), wherey() - 1, 0, 0, 1, 0, 1, 0 };
    gotoxy(15, 20);
    map[8] = { wherex(), wherey() - 1, 0, 0, 0, 0, 0, 1 };
    gotoxy(35, 15);
    map[9] = { wherex(), wherey() - 1, 0, 0, 0, 0, 1, 0 };
    gotoxy(35, 10);
    map[10] = { wherex(), wherey() - 1, 0, 0, 0, 0, 1, 0 };
}

//���x��3�̃}�b�v�ݒ�
void mapsPaint3() {
    gotoxy(5, 10);
    map[0] = { wherex(), wherey() - 1, 9, 1, 0, 1, 0, 1 };      //(x,y,index,isCheck,isUp,isDown,isLeft,isRight);
    gotoxy(5, 15);
    map[1] = { wherex(), wherey() - 1, 0, 0, 1, 0, 0, 1 };
    gotoxy(15, 10);
    map[2] = { wherex(), wherey() - 1, 0, 0, 0, 1, 1, 1 };
    gotoxy(15, 15);
    map[3] = { wherex(), wherey() - 1, 0, 0, 1, 0, 1, 1 };
    gotoxy(25, 15);
    map[4] = { wherex(), wherey() - 1, 0, 0, 0, 1, 1, 0 };
    gotoxy(25, 20);
    map[5] = { wherex(), wherey() - 1, 0, 0, 1, 0, 0, 0 };
    gotoxy(25, 10);
    map[6] = { wherex(), wherey() - 1, 0, 0, 0, 0, 1, 1 };
    gotoxy(35, 10);
    map[7] = { wherex(), wherey() - 1, 0, 0, 1, 0, 1, 0 };
    gotoxy(35, 5);
    map[8] = { wherex(), wherey() - 1, 0, 0, 0, 1, 1, 0 };
    gotoxy(25, 5);
    map[9] = { wherex(), wherey() - 1, 0, 0, 0, 0, 1, 1 };
    gotoxy(15, 5);
    map[10] = { wherex(), wherey() - 1, 0, 0, 0, 0, 0, 1 };
}

//logs�ۑ�
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

//logs���o�͂���
void logsRead() {
    for (int i = 3; i < 30; i++)
    {
        gotoxy(60, i);
        printf("��");
    }
    for (int i = 3; i < 30; i++)
    {
        gotoxy(115, i);
        printf("��");
    }
    gotoxy(60, 2);
    printf("����������������������������������������������������������������������������������������������������������������");
    gotoxy(60, 30);
    printf("����������������������������������������������������������������������������������������������������������������");
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

//�u���̋�悪�ǂ̃^�C�v�v�̔���
void room(MAPS* map) {
    mapsPaint(map);
    switch (map->index)
    {
    case 1:
        shop(player, map, status);
        break;
    case 2:
        msleep(1000);
        logs("�����Ȃ�...\n");       
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
        enemyRoom(player, map, status, enemy);
        break;
    case 7:
        bossRoom(player, map, status,boss);
        break;
    case 8:
        nextFloorRoom(player, map, status);
        break;
    default:
        break;
    }
}

//���X�̏���
void shop(PLAYER* player, MAPS* map, STATUS* status) {
    logs("���X��������܂���\n");
    logsRead();
    gotoxy(wherex(), wherey());
    msleep(500);
    printf(".");
    msleep(500);
    printf(".");
    msleep(500);
    printf(".");
    msleep(1000);
    int index = 0;      //0�F�񕜖�𔃂� 1�F�S�ẴL������hp���񕜂��� 2�F�L�������ق� 3�F����
    clrscr();
    while (1)
    {
        reinport();
        gotoxy(46, 4);
        printf("�����F%d G", player->gold);
        gotoxy(46, 26);
        printf("�񕜖�F%d", player->potionNum);
        for (int i = 6; i < 25; i++)
        {
            gotoxy(45, i);
            printf("��");
        }
        for (int i = 6; i < 25; i++)
        {
            gotoxy(72, i);
            printf("��");
        }
        gotoxy(45, 5);
        printf("��������������������������������������������������������");
        gotoxy(45, 25);
        printf("��������������������������������������������������������");
        gotoxy(50, 7);
        printf("���C�[�_���X");
        switch (index)
        {
        case 0:
            gotoxy(50, 12);
            textattr(0x70);
            printf("�񕜖�𔃂�(10 G)");
            textattr(0x0F);
            gotoxy(50, 14);
            printf("��(50 G)");
            gotoxy(50, 16);
            printf("�L�������ق�(20-45 G)");
            gotoxy(50, 18);
            printf("����");
            rewind(stdin);
            break;
        case 1:
            gotoxy(50, 12);
            printf("�񕜖�𔃂�(10 G)");
            textattr(0x70);
            gotoxy(50, 14);
            printf("��(50 G)");
            textattr(0x0F);
            gotoxy(50, 16);
            printf("�L�������ق�(20-45 G)");
            gotoxy(50, 18);
            printf("����");
            rewind(stdin);
            break;
            break;
        case 2:
            gotoxy(50, 12);
            printf("�񕜖�𔃂�(10 G)");
            gotoxy(50, 14);
            printf("��(50 G)");
            textattr(0x70);
            gotoxy(50, 16);
            printf("�L�������ق�(20-45 G)");
            textattr(0x0F);
            gotoxy(50, 18);
            printf("����");
            rewind(stdin);
            break;
        case 3:
            gotoxy(50, 12);
            printf("�񕜖�𔃂�(10 G)");
            gotoxy(50, 14);
            printf("��(50 G)");
            gotoxy(50, 16);
            printf("�L�������ق�(20-45 G)");
            textattr(0x70);
            gotoxy(50, 18);
            printf("����");
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
                    printf("�w�������I�X");
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
                    printf("�����͑���Ȃ��I");
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
                    printf("�S�ẴL�������񕜂����I�X");
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
                    printf("�����͑���Ȃ��I");
                    break;
                }
            case 2:
                if (player->gold >= 45)
                {
                    srand(time(NULL));
                    int g = rand() % 25 + 20;
                    player->gold -= g;
                    charaInput(status);
                    gotoxy(80, 15);
                    printf("�ق������I�X");
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
                    printf("�����͑���Ȃ��I");
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

//�����񕜉��̏���
void healRoom(STATUS* status, MAPS* map) {
    logs("�����񕜉���������܂���\n");
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
            printf("��");
        }
        for (int i = 6; i < 25; i++)
        {
            gotoxy(72, i);
            printf("��");
        }
        gotoxy(45, 5);
        printf("��������������������������������������������������������");
        gotoxy(45, 25);
        printf("��������������������������������������������������������");
        gotoxy(53, 7);
        printf("�񕜂��������H");
        gotoxy(54, 8);
        printf("(��񂾂�)");
        switch (index)
        {
        case 0:
            gotoxy(53, 15);
            textattr(0x70);
            printf("�͂�");
            textattr(0x0F);
            gotoxy(53, 17);
            printf("������");
            rewind(stdin);
            break;
        case 1:
            gotoxy(53, 15);
            printf("�͂�");
            gotoxy(53, 17);
            textattr(0x70);
            printf("������");
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
                    if ((status + i)->isBattle == 2)
                    {
                        (status + i)->isBattle = 0;
                    }                    
                }
                gotoxy(60, 27);
                printf("�S�ẴL�������񕜂����IENTER�������Ă�������...");
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

//��W���̏���
void charaRoom(PLAYER* player, MAPS* map, STATUS* status) {
    logs("�M���K���b�V�������������܂���\n");
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
            printf("��");
        }
        for (int i = 6; i < 25; i++)
        {
            gotoxy(72, i);
            printf("��");
        }
        gotoxy(45, 5);
        printf("��������������������������������������������������������");
        gotoxy(45, 25);
        printf("��������������������������������������������������������");
        gotoxy(53, 7);
        printf("�ق��������H");
        gotoxy(46, 8);
        printf("(��񂾂��B30 G���K�v���B)");
        switch (index)
        {
        case 0:
            gotoxy(53, 15);
            textattr(0x70);
            printf("�͂�");
            textattr(0x0F);
            gotoxy(53, 17);
            printf("������");
            rewind(stdin);
            break;
        case 1:
            gotoxy(53, 15);
            printf("�͂�");
            gotoxy(53, 17);
            textattr(0x70);
            printf("������");
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
                if (player->gold >= 30)
                {
                    clrscr();
                    charaInput(status);
                    player->gold -= 30;
                    return;
                }
                else
                {
                    gotoxy(80, 15);
                    printf("�����͑���Ȃ��I");
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

//�����_�����̏���
void randomRoom(PLAYER* player, MAPS* map, STATUS* status) {
    logs("��̃h�[�A��������܂���\n");
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
            printf("��");
        }
        for (int i = 6; i < 25; i++)
        {
            gotoxy(72, i);
            printf("��");
        }
        gotoxy(45, 5);
        printf("��������������������������������������������������������");
        gotoxy(45, 25);
        printf("��������������������������������������������������������");
        gotoxy(50, 7);
        printf("�h�[�A���J���������H");
        switch (index)
        {
        case 0:
            gotoxy(53, 15);
            textattr(0x70);
            printf("�͂�");
            textattr(0x0F);
            gotoxy(53, 17);
            printf("������");
            rewind(stdin);
            break;
        case 1:
            gotoxy(53, 15);
            printf("�͂�");
            gotoxy(53, 17);
            textattr(0x70);
            printf("������");
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
            int a;      //0�F�� 1�F�񕜖���� 2�F�S�[���h���� 3�F�퓬���� 4�F�S�̂Ƀ_���[�W
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
                        if ((status + i)->isBattle == 2)
                        {
                            (status + i)->isBattle = 0;
                        }
                    }
                    gotoxy(60, 27);
                    printf("�S�ẴL�������񕜂����IENTER�������Ă�������...");
                    rewind(stdin);
                    getchar();
                    return;
                case 1:
                    player->potionNum += 1;
                    gotoxy(60, 27);
                    printf("�񕜖����肵���IENTER�������Ă�������...");
                    rewind(stdin);
                    getchar();
                    return;
                case 2:
                    srand(time(NULL));
                    g = rand() % 15 + 5;
                    player->gold += g;
                    gotoxy(60, 27);
                    printf("%d G����肵���IENTER�������Ă�������...",g);
                    rewind(stdin);
                    getchar();
                    return;
                case 3:
                    clrscr();
                    enemyRoom(player, map, status, enemy);
                    return;
                case 4:
                    srand(time(NULL));
                    blood = rand() % 100 + 1;
                    for (int i = 0; i < G_status; i++)
                    {
                        (status + i)->hp -= blood;
                    }
                    gotoxy(60, 27);
                    printf("�ŃK�X�I�S�ẴL������%d�_�󂯂�ꂽ�IENTER�������Ă�������...", blood);
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

//�G�̉��̏���
void enemyRoom(PLAYER* player, MAPS* map, STATUS* status, STATUS* enemy) {
    logs("�Ȃ񂩕ςȑ�������\n");
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
    int index = 0;      //0�F�키 1�F�󋵊m�F 2�F������
    char KATAGANA[512][3] = { "�A", "�C", "�E", "�G", "�I", "�J", "�L", "�N", "�P", "�R", "�T", "�V", "�X", "�Z", "�\", "�^", "�`", "�c", "�e", "�g", "�i", "�j", "�k", "�l", "�m", "�n", "�q", "�t", "�w", "�z", "�}", "�~", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "�K", "�M", "�O", "�Q", "�S", "�U", "�W", "�Y", "�[", "�]", "�_", "�W", "�d", "�f", "�h", "�o","�r", "�u", "�x", "�{", "�p", "�s", "�v", "�y", "�|" };
    srand(time(NULL));
    int num = rand() % 10 + 1;
    strcpy(enemy->name, "");
    //�J�^�J�i�̓G���������_���ɐ�������
    for (int i = 0; i < num; i++)
    {
        int roll = rand() % 71;
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
    enemy->eva = (seedNum / 5) % 40;
    enemy->cri = (seedNum / 6) % 80;
    enemy->maxHp = enemy->hp;

    while (1)
    {
        reinport();
        for (int i = 3; i < 30; i++)
        {
            gotoxy(1, i);
            printf("��");
        }
        for (int i = 3; i < 30; i++)
        {
            gotoxy(56, i);
            printf("��");
        }
        gotoxy(1, 2);
        printf("����������������������������������������������������������������������������������������������������������������");
        gotoxy(1, 30);
        printf("����������������������������������������������������������������������������������������������������������������");
        gotoxy(2, 3);
        printf("���I�₹����  ");
        textattr(0x0A);
        printf("%s ", enemy->name);
        textattr(0x0F);
        gotoxy(2, 4);
        printf("���Ƃт����Ă����I");
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
            printf("��");
        }
        for (int i = 10; i < 25; i++)
        {
            gotoxy(110, i);
            printf("��");
        }
        gotoxy(70, 9);
        printf("����������������������������������������������������������������������������������");
        gotoxy(70, 25);
        printf("����������������������������������������������������������������������������������");
        
        int j = 0;
        for (int i = 0; i < G_status; i++)
        {            
            if ((status + i)->hp == 0)
            {
                j++;
            }
            if (j == G_status)
            {
                stopsound(BGM);
                closesound(BGM);
                playsound(DEAD, 0);
                clrscr();
                gotoxy(55, 15);
                printf("GAME OVER");
                G_status = 0;
                getchar();
                clrscr();
                title();
            }
        }

        gotoxy(87, 12);
        printf("�ǂ�����H");
        switch (index)
        {
        case 0:
            gotoxy(87, 15);
            textattr(0x70);
            printf("��������");
            textattr(0x0F);
            gotoxy(84, 17);
            printf("�󋵂��m�F����");
            gotoxy(88, 19);
            printf("�ɂ���");
            rewind(stdin);
            break;
        case 1:
            gotoxy(87, 15);            
            printf("��������");           
            textattr(0x70);
            gotoxy(84, 17);
            printf("�󋵂��m�F����");
            textattr(0x0F);
            gotoxy(88, 19);
            printf("�ɂ���");
            rewind(stdin);
            break;
        case 2:
            gotoxy(87, 15);            
            printf("��������");            
            gotoxy(84, 17);
            printf("�󋵂��m�F����");
            textattr(0x70);
            gotoxy(88, 19);
            printf("�ɂ���");
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
            srand(time(NULL));
            int s = rand() % G_status;
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
                if ((status + s)->hp <= enemy->atk && (status + s)->hp != 0)
                {
                    (status + s)->hp = 1;
                }
                else
                {
                    (status + rand() % G_status)->hp -= enemy->atk;
                }                
                logs("������...�ɂ�...\n");
                clrscr();
                return;
            default:
                break;
            }
        }

        if (enemy->hp == 0)
        {
            srand(time(NULL));
            int g = rand() % 15 + 5;
            player->gold += g;
            clrscr();
            while (1)
            {
                reinport();
                for (int i = 6; i < 25; i++)
                {
                    gotoxy(45, i);
                    printf("��");
                }
                for (int i = 6; i < 25; i++)
                {
                    gotoxy(72, i);
                    printf("��");
                }
                gotoxy(45, 5);
                printf("��������������������������������������������������������");
                gotoxy(45, 25);
                printf("��������������������������������������������������������");
                gotoxy(47, 7);
                printf("%s", enemy->name);
                gotoxy(47, 8);
                printf("�͌N�̋����Ɋ���������");
                gotoxy(47, 10);
                printf("�N�̃`�[���ɎQ��������");
                gotoxy(47, 12);
                printf("�ق����H");
                gotoxy(47, 20);
                printf("%d G����肵���I", g);
                switch (index)
                {
                case 0:
                    gotoxy(53, 15);
                    textattr(0x70);
                    printf("�͂�");
                    textattr(0x0F);
                    gotoxy(53, 17);
                    printf("������");
                    rewind(stdin);
                    break;
                case 1:
                    gotoxy(53, 15);
                    printf("�͂�");
                    gotoxy(53, 17);
                    textattr(0x70);
                    printf("������");
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
                            printf("���Ȃ��̃L�����͑�������B�ꖼ�����ق��Ă��������B");
                            gotoxy(35, 28);
                            printf("ENTER�������Ă�������...");
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
                        printf("%s�͉��������I", enemy->name);
                        gotoxy(60, 28);
                        printf("ENTER�������Ă�������...");
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

//boss�̉��̏���
void bossRoom(PLAYER* player, MAPS* map, STATUS* status, STATUS* enemy) {
    logs("���̂Ƃ���́A�댯�ȑ�������\n");
    textattr(0x0C);
    logsRead();
    gotoxy(wherex(), wherey());
    msleep(500);
    printf(".");
    msleep(500);
    printf(".");
    msleep(500);
    printf(".");
    textattr(0x0F);
    msleep(1000);
    clrscr();
    int index = 0;
    while (1)
    {
        reinport();
        for (int i = 3; i < 30; i++)
        {
            gotoxy(1, i);
            printf("��");
        }
        for (int i = 3; i < 30; i++)
        {
            gotoxy(56, i);
            printf("��");
        }
        gotoxy(1, 2);
        printf("����������������������������������������������������������������������������������������������������������������");
        gotoxy(1, 30);
        printf("����������������������������������������������������������������������������������������������������������������");
        gotoxy(2, 3);
        printf("%s ", boss->name);
        gotoxy(3, 6);
        printf("HP/MAXHP: %d/%d\n", boss->hp, boss->maxHp);
        gotoxy(3, 7);
        printf("ATK: %d\n", boss->atk);
        gotoxy(3, 8);
        printf("DEF: %d\n", boss->def);
        gotoxy(3, 9);
        printf("SPD: %d\n", boss->spd);
        gotoxy(3, 10);
        printf("EVA: %d\n", boss->eva);
        gotoxy(3, 11);
        printf("CRI: %d\n\n", boss->cri);
        gotoxy(2, 15);
        printf("�����ɗ����̂́A�悭�ł���...");
        for (int i = 10; i < 25; i++)
        {
            gotoxy(70, i);
            printf("��");
        }
        for (int i = 10; i < 25; i++)
        {
            gotoxy(110, i);
            printf("��");
        }
        gotoxy(70, 9);
        printf("����������������������������������������������������������������������������������");
        gotoxy(70, 25);
        printf("����������������������������������������������������������������������������������");
        
        int j = 0;
        for (int i = 0; i < G_status; i++)
        {
            if ((status + i)->hp == 0)
            {
                j++;
            }
            if (j == G_status)
            {
                stopsound(BGM);
                closesound(BGM);
                playsound(DEAD, 0);
                clrscr();
                gotoxy(45, 15);
                printf("�N�A�����Ƌ����Ȃ���΂Ȃ�Ȃ��I");
                getchar();
                clrscr();
                for (int k = 0; k < G_status; k++)
                {
                    (status + k)->hp = (status + k)->maxHp;
                }
                boss->hp = boss->maxHp;
                G_floor = 1;
                mapsPaint1();
                player->x = 5;
                player->y = 9;
                G_checkMapNum = 1;                
                clrscr();
                return;
            }
        }
        gotoxy(87, 12);
        printf("�ǂ�����H");
        switch (index)
        {
        case 0:
            gotoxy(87, 15);
            textattr(0x70);
            printf("��������");
            textattr(0x0F);
            gotoxy(84, 17);
            printf("�󋵂��m�F����");
            gotoxy(88, 19);
            printf("�ɂ���");
            rewind(stdin);
            break;
        case 1:
            gotoxy(87, 15);
            printf("��������");
            textattr(0x70);
            gotoxy(84, 17);
            printf("�󋵂��m�F����");
            textattr(0x0F);
            gotoxy(88, 19);
            printf("�ɂ���");
            rewind(stdin);
            break;
        case 2:
            gotoxy(87, 15);
            printf("��������");
            gotoxy(84, 17);
            printf("�󋵂��m�F����");
            textattr(0x70);
            gotoxy(88, 19);
            printf("�ɂ���");
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
                battle(status, boss, player);
                break;
            case 1:
                charaOutput(status, player);
                break;
            case 2:
                map->isCheck = 2;
                clrscr();
                return;
            default:
                break;
            }
        }
        if (boss->hp == 0)
        {
            clrscr();
            if (G_status != MAX_STATUS)
            {
                strcpy((status + G_status)->name, boss->name);
                (status + G_status)->hp = 1;
                (status + G_status)->atk = boss->atk;
                (status + G_status)->def = boss->def;
                (status + G_status)->spd = boss->spd;
                (status + G_status)->eva = boss->eva;
                (status + G_status)->cri = boss->cri;
                (status + G_status)->isBattle = 0;
                (status + G_status)->maxHp = boss->maxHp;
                G_status += 1;
            }
            else
            {
                boss->hp = boss->maxHp;
            }
            gotoxy(55, 15);
            printf("�悭...");
            msleep(500);
            printf("��");
            msleep(500);
            printf("��");
            msleep(500);
            printf("��");
            msleep(1000);
            rewind(stdin);           
            getchar();
            clrscr();
            G_floor = 1;
            mapsPaint1();
            player->x = 5;
            player->y = 9;
            G_checkMapNum = 1;
            clrscr();
            return;
        }
    }

}

//�K�i�̏���
void nextFloorRoom(PLAYER* player, MAPS* map, STATUS* status) {
    logs("�K�i��������܂���\n");
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
            printf("��");
        }
        for (int i = 6; i < 25; i++)
        {
            gotoxy(72, i);
            printf("��");
        }
        gotoxy(45, 5);
        printf("��������������������������������������������������������");
        gotoxy(45, 25);
        printf("��������������������������������������������������������");
        gotoxy(53, 7);
        printf("�K�i����邩�H");
        gotoxy(54, 8);
        printf("(�߂�s��)");
        gotoxy(51, 10);
        printf("(�퓬�s�\�̃L������");
        gotoxy(52, 11);
        printf("�`�[������ޏo�j");
        switch (index)
        {
        case 0:
            gotoxy(53, 16);
            textattr(0x70);
            printf("�͂�");
            textattr(0x0F);
            gotoxy(53, 18);
            printf("������");
            rewind(stdin);
            break;
        case 1:
            gotoxy(53, 16);
            printf("�͂�");
            gotoxy(53, 18);
            textattr(0x70);
            printf("������");
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
                reinport();
                clrscr();
                for (int i = 0; i < G_status; i++)
                {
                    if ((status + i)->isBattle == 2 || (status + i)->hp == 0)
                    {
                        for (int j = i; j < G_status - 1; j++)
                        {
                            strcpy((status + j)->name, (status + j + 1)->name);
                            (status + j)->hp = (status + j + 1)->hp;
                            (status + j)->atk = (status + j + 1)->atk;
                            (status + j)->def = (status + j + 1)->def;
                            (status + j)->spd = (status + j + 1)->spd;
                            (status + j)->eva = (status + j + 1)->eva;
                            (status + j)->cri = (status + j + 1)->cri;
                            (status + j)->isBattle = (status + j + 1)->isBattle;
                            (status + j)->maxHp = (status + j + 1)->maxHp;
                        }
                        G_status -= 1;
                        i -= 1;
                    }
                }

                G_floor++;
                switch (G_floor)
                {
                case 1:
                    mapsPaint1();
                    break;
                case 2:
                    mapsPaint2();
                    break;
                case 3:
                    mapsPaint3();
                    break;
                default:
                    break;
                }               
                player->x = 5;
                player->y = 9;
                G_checkMapNum = 1;
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

//�L��������͂���
void charaInput(STATUS* status) {
    clrscr();
    while (G_status >= MAX_STATUS)
    {
        gotoxy(35, 10);
        printf("���Ȃ��̃L�����͑�������B�ꖼ�����ق��Ă��������B");
        gotoxy(35, 12);
        printf("ENTER�������Ă�������...");
        rewind(stdin);
        getchar();
        charaOutput(status, player);
    }
    int len;
    int seedNum = 0;
    int cmp;
    do
    {   
        cmp = 0;
        clrscr();
        gotoxy(1, 1);
        printf("�L�����̖��O����͂��Ă�������>>(�S�p32���ȓ��A���p�p����64���ȓ�)");
        rewind(stdin);
        scanf("%s", (status + G_status)->name);
        for (int i = 0; i < G_status; i++)
        {
            if (strcmp((status + G_status)->name, (status + i)->name) == 0)
            {
                cmp = 1;
                printf("\n%s�͂����N�̃`�[���ɂ���", (status + G_status)->name);
                rewind(stdin);
                getchar();
            }
        }
        
    } while (cmp == 1);
    
    if (strcmp((status + G_status)->name, "Dungeon�̎��ҁE�M���K���b�V���E���C�[�_") == 0)
    {
        printf("�c�O...����̓_����\n");
        strcat((status + G_status)->name, "(�U)");
        (status + G_status)->hp = 100;
        (status + G_status)->atk = 20;
        (status + G_status)->def = 10;
        (status + G_status)->spd = 0;
        (status + G_status)->eva = 0;
        (status + G_status)->cri = 0;
        (status + G_status)->isBattle = 0;
        (status + G_status)->maxHp = (status + G_status)->hp;

    }
    else if (strcmp((status + G_status)->name, "CHLOE") == 0)
    {
        printf("\n�B���ꂽ�L��������ɓ��ꂽ���߂łƂ�...\n");
        (status + G_status)->hp = 299;
        (status + G_status)->atk = 99;
        (status + G_status)->def = 39;
        (status + G_status)->spd = 9;
        (status + G_status)->eva = 39;
        (status + G_status)->cri = 99;
        (status + G_status)->isBattle = 0;
        (status + G_status)->maxHp = (status + G_status)->hp;
    }
    else
    {
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
        (status + G_status)->def = (seedNum / 3) % 35 + 5;
        (status + G_status)->spd = (seedNum / 4) % 10;
        (status + G_status)->eva = (seedNum / 9) % 40;
        (status + G_status)->cri = (seedNum / 6) % 80;
        (status + G_status)->isBattle = 0;
        (status + G_status)->maxHp = (status + G_status)->hp;
    }
    printf("�L�����쐬��.");
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
    printf("�쐬�����IENTER�������Ă�������...");
    G_status += 1;
    rewind(stdin);
    getchar();
}

//�L�������o�͂���
void charaOutput(STATUS* status, PLAYER* player) {
    clrscr();
    int index = 0;
    int index0 = 0;
    for (int i = 0; i < G_status; i++)
    {
        if ((status + i)->hp <= 0)
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
            printf("��");
        }
        gotoxy(5, 2);
        printf("����������������������������������������������������������������������������������������������������������������");
        gotoxy(5, 30);
        printf("����������������������������������������������������������������������������������������������������������������");
        for (int i = 3; i < 30; i++)
        {
            gotoxy(60, i);
            printf("��");
        }
        for (int i = 3; i < 30; i++)
        {
            gotoxy(115, i);
            printf("��");
        }
        gotoxy(60, 2);
        printf("����������������������������������������������������������������������������������������������������������������");
        gotoxy(60, 30);
        printf("����������������������������������������������������������������������������������������������������������������");
        gotoxy(5, 1);
        printf("�����F%d G", player->gold);

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
            printf("�񕜖���g��");
            textattr(0x0F);
            gotoxy(85, 20);
            printf("�o��");
            gotoxy(95, 20);
            printf("����");
            gotoxy(65, 21);
            printf("(�c��́F%d)", player->potionNum);
            gotoxy(83, 21);
            if ((status + index)->isBattle == 0)
            {
                printf("(�x�e��)");
            }
            if ((status + index)->isBattle == 1)
            {
                printf("(�o�풆)");
            }
            if ((status + index)->isBattle == 2)
            {
                printf("(�퓬�s�\)");
            }
            break;
        case 1:
            gotoxy(65, 20);
            printf("�񕜖���g��");
            textattr(0x70);
            gotoxy(85, 20);
            printf("�o��");
            textattr(0x0F);
            gotoxy(95, 20);
            printf("����");
            gotoxy(65, 21);
            printf("(�c��́F%d)", player->potionNum);
            gotoxy(83, 21);
            if ((status + index)->isBattle == 0)
            {
                printf("(�x�e��)");
            }
            if ((status + index)->isBattle == 1)
            {
                printf("(�o�풆)");
            }
            if ((status + index)->isBattle == 2)
            {
                printf("(�퓬�s�\)");
            }
            break;
        case 2:
            gotoxy(65, 20);
            printf("�񕜖���g��");
            gotoxy(85, 20);
            printf("�o��");
            textattr(0x70);
            gotoxy(95, 20);
            printf("����");
            textattr(0x0F);
            gotoxy(65, 21);
            printf("(�c��́F%d)", player->potionNum);
            gotoxy(83, 21);
            if ((status + index)->isBattle == 0)
            {
                printf("(�x�e��)");
            }
            if ((status + index)->isBattle == 1)
            {
                printf("(�o�풆)");
            }
            if ((status + index)->isBattle == 2)
            {
                printf("(�퓬�s�\)");
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
                    printf("�񕜖���g���K�v�͂Ȃ��IENTER�������Ă�������...");
                    rewind(stdin);
                    getchar();
                }
                else if (player->potionNum == 0) 
                {
                    gotoxy(62, 23);
                    printf("�񕜖򂪂Ȃ��IENTER�������Ă�������...");
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
                if (G_status == 1)
                {
                    gotoxy(62, 23);
                    printf("���߁IENTER�������Ă�������...");
                    rewind(stdin);
                    getchar();
                    break;
                }
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

//�퓬����1
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
            printf("�o��̃L������I�����Ă��������I");
            return;
        }       
    }
    clrscr();
    reinport();

    gotoxy(31, 3);
    printf("�퓬�J�n�I");
    if ((status + t)->spd >= enemy->spd)
    {
        gotoxy(31, 5);
        printf("%s�̐�U�I", (status + t)->name);
        while (1)
        {
            battleS(status + t, enemy);
            msleep(2500);
            if ((status + t)->hp == 0)
            {
                gotoxy(31, wherey() + 2);
                printf("�퓬���s...ENTER�������Ă�������...");
                (status + t)->isBattle = 2;
                rewind(stdin);
                getchar();
                clrscr();
                return;
            }
            if (enemy->hp == 0)
            {
                gotoxy(31, wherey() + 2);
                printf("�퓬�����I�IENTER�������Ă�������...");
                rewind(stdin);
                getchar();
                clrscr();
                return;
            }
            battleS(enemy, status + t);
            msleep(2500);
            if ((status + t)->hp == 0)
            {
                gotoxy(31, wherey() + 2);
                printf("�퓬���s...ENTER�������Ă�������...");
                (status + t)->isBattle = 2;
                rewind(stdin);
                getchar();
                clrscr();
                return;
            }
            if (enemy->hp == 0)
            {
                gotoxy(31, wherey() + 2);
                printf("�퓬�����I�IENTER�������Ă�������...");
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
        printf("%s�̐�U�I", enemy->name);
        while (1)
        {
            battleS(enemy, status + t);            
            msleep(2500);
            if ((status + t)->hp == 0)
            {
                gotoxy(31, wherey() + 2);
                printf("�퓬���s...ENTER�������Ă�������...");
                (status + t)->isBattle = 2;
                rewind(stdin);
                getchar();
                clrscr();
                return;
            }
            if (enemy->hp == 0)
            {
                gotoxy(31, wherey() + 2);
                printf("�퓬�����I�IENTER�������Ă�������...");
                rewind(stdin);
                getchar();
                clrscr();
                return;
            }
            battleS(status + t, enemy);
            msleep(2500);
            if ((status + t)->hp == 0)
            {
                gotoxy(31, wherey() + 2);
                printf("�퓬���s...ENTER�������Ă�������...");
                (status + t)->isBattle = 2;
                rewind(stdin);
                getchar();
                clrscr();
                return;
            }
            if (enemy->hp == 0)
            {
                gotoxy(31, wherey() + 2);
                printf("�퓬�����I�IENTER�������Ă�������...");
                rewind(stdin);
                getchar();
                clrscr();
                return;
            }
        }
    }
}

//�퓬����2
void battleS(STATUS* status, STATUS* enemy) {
    srand(time(NULL));
    gotoxy(31, wherey() + 1);
    printf("%s�� �U���I", status->name);
    if (rand() % 100 < enemy->eva) 
    {
        gotoxy(31, wherey() + 1);
        printf("MISS!\n");
    }
    else 
    {
        srand(time(NULL));
        if (rand() % 100 < status->cri)
        {
            int damage = (status->atk) * 2 - (enemy->def);
            gotoxy(31, wherey() + 1);           
            printf("��S�̈ꌂ�I�I");
            gotoxy(31, wherey() + 1);
            printf("%s ��%d�̃_���[�W�I�I", enemy->name, damage);
            enemy->hp -= damage;
            if (enemy->hp <= 0)
            {
                enemy->hp = 0;
                gotoxy(31, wherey() + 1);
                printf("%s �͑��₦���D�D�D", enemy->name);
            }
            else
            {
                gotoxy(31, wherey() + 1);
                printf("%s �̎c��HP��%d\n", enemy->name, enemy->hp);
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
            printf("%s ��%d�̃_���[�W�I�I", enemy->name, damage);
            enemy->hp -= damage;
            if (enemy->hp <= 0)
            {
                enemy->hp = 0;
                gotoxy(31, wherey() + 1);
                printf("%s �͑��₦���D�D�D", enemy->name);
            }
            else
            {
                gotoxy(31, wherey() + 1);
                printf("%s �̎c��HP��%d\n", enemy->name, enemy->hp);
            }
        }
    }

}

//�Z�[�u�Q�[��
void saveGame(PLAYER* player, MAPS* map, STATUS* status, STATUS* boss) {
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
    fwrite(map, sizeof(*map), MAX_MAP, fp3);
    fclose(fp3);

    FILE* fp4 = fopen("boss.bin", "wb");
    fwrite(boss, sizeof(*boss), 1, fp4);
    fclose(fp4);

    gotoxy(60, 27);
    printf("�ۑ���");
    msleep(500);
    printf(".");
    msleep(500);
    printf(".");
    msleep(500);
    printf(".");
    msleep(1000);
    printf("�����I�IENTER�������Ă�������...");
    rewind(stdin);
    getchar();
    clrscr();
}

//���[�h�Q�[��
void loadGame(PLAYER* player, MAPS* map, STATUS* status, STATUS* boss) {
    fopen("logs.txt", "w");
    unsigned char status_count = G_status;
    FILE* fp = fopen("charaters.bin", "rb");
    if (fp == NULL)
    {
        gotoxy(70, 17);
        printf("�i�A�[�J�C�u������܂���B�j\n");
        return;
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
        //printf("player.bin������܂���B\n");
    }
    else
    {
        fread(player, sizeof(*player), 1, fp1);
        fclose(fp1);
    }

    FILE* fp2 = fopen("floor.bin", "rb");
    if (fp2 == NULL)
    {
        //printf("floor.bin������܂���B\n");
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
        break;
    case 2:
        mapsPaint2();
        break;
    case 3:
        mapsPaint3();
        break;
    default:
        break;
    }

    unsigned char map_count = G_checkMapNum;
    FILE* fp3 = fopen("maps.bin", "rb");
    if (fp3 == NULL)
    {
        //printf("maps.bin������܂���B\n");
        //return;
    }
    else
    {
        fread(&map_count, sizeof(map_count), 1, fp3);
        fread(map, sizeof(*map), MAX_MAP, fp3);        
        G_checkMapNum = map_count;
        fclose(fp3);
    }

    FILE* fp4 = fopen("boss.bin", "rb");
    if (fp4 == NULL)
    {
        
    }
    else
    {
        fread(boss, sizeof(*boss), 1, fp4);
        fclose(fp4);
    }

    clrscr();
    while (1)
    {
        moveMent(player, map);
        logsRead();
    }

    getchar();
}

//�Q�[���I���̏���
void endGame() {
    clrscr();
    int index = 0;
    while (1)
    {
        reinport();
        for (int i = 6; i < 25; i++)
        {
            gotoxy(45, i);
            printf("��");
        }
        for (int i = 6; i < 25; i++)
        {
            gotoxy(72, i);
            printf("��");
        }
        gotoxy(45, 5);
        printf("��������������������������������������������������������");
        gotoxy(45, 25);
        printf("��������������������������������������������������������");
        gotoxy(50, 7);
        printf("�Q�[�����I��������");
        gotoxy(54, 8);
        printf("�����ł����H");
        switch (index)
        {
        case 0:
            gotoxy(53, 15);
            textattr(0x70);
            printf("�͂�");
            textattr(0x0F);
            gotoxy(53, 17);
            printf("������");
            rewind(stdin);
            break;
        case 1:
            gotoxy(53, 15);
            printf("�͂�");
            gotoxy(53, 17);
            textattr(0x70);
            printf("������");
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

//cursor���B������
void hideCursor() {
    CONSOLE_CURSOR_INFO cur;
    cur.dwSize = 1;
    cur.bVisible = 0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cur);
}