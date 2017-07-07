#include<Windows.h>
#include"Ball.h"

/************* 全局变量 ****************/
// 小球
GAME_COORD ball;
// 板子（链表）
PLIST board_list;
// 游戏边界（这是右下，左上为0,0）
GAME_COORD boundary;
// 计时器到时间隔
DWORD dwTimerElapse=300;
// 每提高一级，计时器时钟周期缩短的比例。
DOUBLE dbLevelSpeedupRatio=0.8;
// 此变量表示多少个积分上升一个级别，每50分升级
DWORD dwOneLevelScores=25;
int BoardUp = 0;//每两个时间周期板子上升一次,每一个时间周期，未在板的小球下落
int UpNum = 0;//板子每上升3次，产生一个新板子//每产生五个板子，分数+1
int CreatNum=0;
int Status ;//初始小球是否位于木板
Type TypeBoard;//与小球发生互动的板子类型
int LIFE; //重球4条生命，轻球两条生命，链表末尾板子复活，生命用完，游戏结束
int Score;//比分
int tempScore=0;//每产生五个板子，分数+1
int GameOver = 0;//当LIFE为0时，变为1
int JumpNum=0;//遇到弹跳板子是弹跳次数
int GameDiffculty;//游戏难度
int flag = 0;//用于记录易碎


//BOOL isOptionPage(Page page)
//{
//	if (page == OptionPage)return TRUE;
//	else return FALSE;
//}
//BOOL isGamePage(Page page)
//{
//	if (page == GamePage)return TRUE;
//	else return FALSE;
//}
//BOOL isMainPage(Page page)
//{
//	if (page == MainPage)return TRUE;
//	else return FALSE;
//}
void CreateMain(HWND hwnd, int boundary_x, int boundary_y)
{
	//设置随机数种子，用于随机生成板子
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	srand(ft.dwLowDateTime);
	
	//设置游戏边界（请勿更改）
	SetBoundary(boundary_x, boundary_y);


}
void CreateGame(HWND hwnd, int color, int diffculty, int balltype)
{
	//CreatGame中并没有用到color，它在GamePaint中会用到，不过……懒得改了……略略略
	dwTimerElapse = 100;
	switch (diffculty) {
	case Easy:
		break;
	case Mid:
		dwTimerElapse = dwTimerElapse*dbLevelSpeedupRatio;
		break;
	case Hard:
		dwTimerElapse = dwTimerElapse*dbLevelSpeedupRatio*dbLevelSpeedupRatio;
		break;
	default:
		break;
	}//根据难度设置游戏周期
	GameDiffculty = diffculty;
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	srand(ft.dwLowDateTime);//用于生成随机数
	if (balltype == Light)LIFE = 2;
	else LIFE = 4;
	Score = 0;
	CreatNum = 0;
	BoardUp = 0;
	UpNum = 0;
	tempScore = 0;
	GameOver = 0;
	JumpNum = 0;
	flag = 0;
	CreateBoard();
	CreateBall(balltype);
	SetTimer(hwnd, 12340, dwTimerElapse, NULL);
	
}
void SetBoundary(int x, int y)
{
	boundary.x = x;
	boundary.y = y;
}
PGAME_COORD GetBoundary()
{
	return &boundary;
}
void CreateBoard()
{
	PGAME_COORD p;
	p = (PGAME_COORD)malloc(sizeof(GAME_COORD));
	p->x = rand() % 12;
	p->y = 30;
	int i = rand() % 10;
	if (i < 7)p->type = bNormal;//百分之七十的概率产生普通板子
	else if(i<9)
	{
		p->type = rand() % 4 + 1;//其它特殊板子产生概率相同
	}//百分之二十的概率产生特殊板子
	//百分之十的概率不产生板子
	board_list = ListCreate(NULL);
	ListPushFront(board_list, p);//板头（板子左端点）用链表存储

}
void CreateBall(int balltype)
{
	PGAME_COORD tail = GetBoardTail();
	ball.x = tail->x+2;//五个点组成一个板子，生成小球在板子正中间
	ball.y = tail->y - 1;//小球自然是落在板子上方的
	tail->type = bNormal;//为保护小球，自动变成不同板子
	ball.type = balltype;
	
}
PGAME_COORD GetBoardTail()
{
	return (PGAME_COORD)ListGetAt(board_list, GetBoardNum() - 1);
}
int GetBoardNum()
{
	return ListSize(board_list);
}
PGAME_COORD board_get_at(int i)
{
	return (PGAME_COORD)ListGetAt(board_list, i);
}
//要修改板子移动属性，轻在此函数中
void BoardMove(HWND hwnd)
{
	for (int i = 0; i < GetBoardNum(); i++)
	{
		PGAME_COORD temp = board_get_at(i);
		//板子移动速度在这里
		if (BoardUp == 1)
		{
			temp->y -= 1;
		}
		if (temp->type == bRmoving)
		{
			if (temp->x >= 11)
			{
				temp->type = bLMoving;
				temp->x -= 1;   //板子移动步数
			}
			else
			{
				temp->x += 1;
			}
		}
		else if(temp->type==bLMoving)
		{
			if (temp->x == 0)
			{
				temp->type = bRmoving;
				temp->x += 1;
			}
			else
			{
				temp->x -= 1;
			}
		}
	}
	if (BoardUp)
	{
		UpNum += 1;
		if (UpNum >= 4)
		{
			MakeBoard(hwnd);
			UpNum = 0;
		}
		BoardUp = 0;
	}
	else
	{
		BoardUp = 1;
	}
	PGAME_COORD poshead = board_get_at(0);
	if (poshead->y == 0)
	{
		PGAME_COORD temp=ListPopFront(board_list);
		free(temp);
	}
}
int StatusBall()
{
	int num = GetBoardNum();
	for (int i = 0; i < num; i++)
	{
		PGAME_COORD temp= board_get_at(i);
		if ((ball.y == (temp->y - 1))
			&& ((ball.x - temp->x) <= 4)
			&& ((ball.x - temp->x) >= 0))
		{
			TypeBoard = temp->type;
			if (TypeBoard == bBroken)flag = i;
			return ON;
		}
		else if (((ball.y == (temp->y - 2))
			&& ((ball.x - temp->x) <= 4)
			&& ((ball.x - temp->x) >= 0))
			&& (BoardUp == 1))
		{
			//bug,球和木板重合
			return Lapped;
		}
	}
	return NotOn;
}
void OnTimer(HWND hwnd)
{


		Status = StatusBall();
		switch (Status)
		{
		case ON:
			switch (TypeBoard)
			{
			case bNormal:
				MoveupBall();
				break;
			case bWithTrap:
				LIFE -= 1;
				if (LIFE == 0) {
					GameOver = 1;
					return;
				}
				else
				{
					CreateBall(ball.type);
					ball.y -= 1;
				}
				break;
			case bLMoving:
				MoveupBall();
				if (ball.x > 0)
				{
					ball.x -= 1;
				}
				else if (ball.x == 0)
				{
					ball.x += 1;
				}
				break;
			case bRmoving:
				MoveupBall();
				if (ball.x < 15)
				{
					ball.x += 1;
				}
				else if (ball.x == 15)
				{
					ball.x -= 1;
				}
			break;
		case bJumping:
			if (ball.type == Light)
			{
				JumpNum = 1;
				MoveupBall();
			}
			else
			{
				MoveupBall();
			}
			break;
		case bBroken:
			switch (ball.type) {
			case Light:
				MoveupBall();
				break;
			case Heavy:
				ball.y += 1;
				if (ball.y >= 30)
				{
					LIFE -= 1;
					if (LIFE == 0) {
						GameOver = 1;
						return;
					}
					else
					{
						CreateBall(ball.type);
						ball.y -= 1;
					}
				}
				ListDeleteAt(board_list ,flag);
				flag = 0;
				break;
			default:
				break;
			}
			//MoveupBall();
			break;
		default:
			break;
			}
			break;
		case NotOn:
			ball.y += 1;
			if (ball.y >= 30)
			{
				LIFE -= 1;
				if (LIFE == 0) {
					GameOver = 1;
					return;
				}
				else
				{
					CreateBall(ball.type);
					ball.y -= 1;
				}
			}
			break;
		default:

			break;
		}
	

		BoardMove(hwnd);
		
	
}
int isGameOver() { return GameOver; }
int GetScore() { return Score; }
int GetLife() { return LIFE; }
void MakeBoard(HWND hwnd)
{

	CreatNum++;
	if (CreatNum >= 5)
	{
		Score++;
		tempScore++;
		if (tempScore >= dwOneLevelScores)
		{
			dwTimerElapse = dwTimerElapse*dbLevelSpeedupRatio;
			SetTimer(hwnd, 12340, dwTimerElapse, NULL);
			LIFE++;
			tempScore = 0;
		}
		CreatNum = 0;
	}
	int i = rand() % 100;
	if (GameDiffculty == Easy)
	{
		if (i < 90)
		{
			PGAME_COORD p;
			p = (PGAME_COORD)malloc(sizeof(GAME_COORD));
			p->x = rand() % 12;
			p->y = 30;
			if (i < 65)p->type = bNormal;//百分之65的概率产生普通板子
			else if(i<78)p->type = bWithTrap;
			else
			{
				p->type = i % 4 + 2;//其它特殊板子产生概率相同
			}//百分之二十的概率产生特殊板子
			 //百分之十的概率不产生板子
			ListPushBack(board_list, p);
		}
	}
	else if (GameDiffculty == Mid)
	{
		if (i < 80)
		{
			PGAME_COORD p;
			p = (PGAME_COORD)malloc(sizeof(GAME_COORD));
			p->x = rand() % 12;
			p->y = 30;
			if (i < 55)p->type = bNormal;//百分之七十的概率产生普通板子
			else if (i<72)p->type = bWithTrap;
			else
			{
				p->type = i % 5 + 1;//其它特殊板子产生概率相同
			}//百分之二十的概率产生特殊板子
			 //百分之十的概率不产生板子
			ListPushBack(board_list, p);
		}
	}
	else
	{
		if (i <75)
		{
			PGAME_COORD p;
			p = (PGAME_COORD)malloc(sizeof(GAME_COORD));
			p->x = rand() % 12;
			p->y = 30;
			if (i < 50)p->type = bNormal;//百分之七十的概率产生普通板子
			else if (i<72)p->type = bWithTrap;
			else
			{
				p->type = i % 5 + 1;//其它特殊板子产生概率相同
			}//百分之二十的概率产生特殊板子
			 //百分之十的概率不产生板子
			ListPushBack(board_list, p);
		}
	}
}
PGAME_COORD GetBoardBody() { return board_list; }
PGAME_COORD GetBallBody() { return &ball; }
int MoveupBall()
{

		if (BoardUp == 1)
		{
			if (JumpNum == 0)
			{
				ball.y -= 1;
			}
			else
			{
				ball.y -= 3;
				JumpNum = 0;
			}
			if (ball.y <= 0) {
				LIFE -= 1;
				if (LIFE == 0) {
					GameOver = 1;
					return 1;
				}
				else
				{
					CreateBall(ball.type);
					ball.y -= 1;
				}
			}
			//BoardUp = 0;
		}
	
	return 0;
}
void MoveLBall()
{
	if (ball.type == Light) {
		ball.x = ball.x - 2;
	}
	else
	{
		ball.x = ball.x - 1;
	}
	if (ball.x < 0)
	{
		ball.x = 0;
	}
}
void MoveRBall()
{
	{
		if (ball.type == Light) {
			ball.x = ball.x + 2;
		}
		else
		{
			ball.x = ball.x + 1;
		}
		if (ball.x > 15)
		{
			ball.x = 15;
		}
	}
}
void Destory()
{
	ListDistoryAndFree(board_list);
}
