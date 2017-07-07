#include<Windows.h>
#include"LinkList.h"
//游戏一共有三个页面，主页面，开始游戏页面，设置页面
#define MainPage1        0    //主页面,选择框位于“开始游戏”
#define MainPage2        1    //主页面,选择框位于“选项”
#define MainPage3        2    //主页面,选择框位于“退出游戏”
#define GamePage         3   //游戏中界面
#define OptionPage1     4   //选项界面，选择框位于“难度”
#define OptionPage2     5  //选项界面，选择框位于“小球种类”
#define OptionPage3     6 //选项界面，选择框位于“小球颜色”
#define OptionPage4     7   //选项界面，位于“返回”
//板子种类
#define bNormal			    0  //普通板子，褐色
#define bWithTrap			1  //陷阱板子，灰色
#define bRmoving	        2  //向右移动的板子，褐色
#define bLMoving          3  //向左移动的板子，褐色
#define bJumping	        4  //弹跳板子，橘黄
#define bBroken		        5  //易碎板子，青色
//小球种类
#define Light                  0//轻小秋 
#define Heavy                1//重小球
//难度
#define Easy    0//简单难度
#define Mid    1//中等难度
#define Hard   2//地狱难度

//小球状态
#define ON      0//在木板上
#define NotOn 1//不在木板
#define Lapped 2//重合bug

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
// 设置边界坐标
void SetBoundary(int x, int y);
void CreateBoard();
void CreateBall(int);
int GetBoardNum();
void BoardMove(HWND);//每次时间周期，向上移动
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
// 获得边界坐标
PGAME_COORD GetBoundary();
