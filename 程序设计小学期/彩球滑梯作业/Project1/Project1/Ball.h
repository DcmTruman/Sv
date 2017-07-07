#include<Windows.h>
#include"LinkList.h"
//��Ϸһ��������ҳ�棬��ҳ�棬��ʼ��Ϸҳ�棬����ҳ��
#define MainPage1        0    //��ҳ��,ѡ���λ�ڡ���ʼ��Ϸ��
#define MainPage2        1    //��ҳ��,ѡ���λ�ڡ�ѡ�
#define MainPage3        2    //��ҳ��,ѡ���λ�ڡ��˳���Ϸ��
#define GamePage         3   //��Ϸ�н���
#define OptionPage1     4   //ѡ����棬ѡ���λ�ڡ��Ѷȡ�
#define OptionPage2     5  //ѡ����棬ѡ���λ�ڡ�С�����ࡱ
#define OptionPage3     6 //ѡ����棬ѡ���λ�ڡ�С����ɫ��
#define OptionPage4     7   //ѡ����棬λ�ڡ����ء�
//��������
#define bNormal			    0  //��ͨ���ӣ���ɫ
#define bWithTrap			1  //������ӣ���ɫ
#define bRmoving	        2  //�����ƶ��İ��ӣ���ɫ
#define bLMoving          3  //�����ƶ��İ��ӣ���ɫ
#define bJumping	        4  //�������ӣ��ٻ�
#define bBroken		        5  //������ӣ���ɫ
//С������
#define Light                  0//��С�� 
#define Heavy                1//��С��
//�Ѷ�
#define Easy    0//���Ѷ�
#define Mid    1//�е��Ѷ�
#define Hard   2//�����Ѷ�

//С��״̬
#define ON      0//��ľ����
#define NotOn 1//����ľ��
#define Lapped 2//�غ�bug

typedef int Type;  
typedef struct _GAME_COORD
{
	short x;
	short y;
	Type type;
}GAME_COORD, *PGAME_COORD;
typedef int Page;


BOOL isOptionPage(Page page);
BOOL isGamePage(Page page);
BOOL isMainPage(Page page);

void CreateMain(HWND hwnd,int boundary_x, int boundary_y);
void CreateGame(HWND, int, int, int);
// ���ñ߽�����
void SetBoundary(int x, int y);
void CreateBoard();
void CreateBall(int);
int GetBoardNum();
void BoardMove(HWND);//ÿ��ʱ�����ڣ������ƶ�
void OnTimer(HWND);
int StatusBall();
void MakeBoard(HWND);
int isGameOver();
int GetScore();
int GetLife();
void MoveLBall();
void MoveRBall();
void Destory();

PGAME_COORD GetBoardTail();
PGAME_COORD GetBallBody();
PGAME_COORD board_get_at(int i);
// ��ñ߽�����
PGAME_COORD GetBoundary();
