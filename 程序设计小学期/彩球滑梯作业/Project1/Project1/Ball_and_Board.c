#include<Windows.h>
#include"Ball.h"

/************* ȫ�ֱ��� ****************/
// С��
GAME_COORD ball;
// ���ӣ�����
PLIST board_list;
// ��Ϸ�߽磨�������£�����Ϊ0,0��
GAME_COORD boundary;
// ��ʱ����ʱ���
DWORD dwTimerElapse=300;
// ÿ���һ������ʱ��ʱ���������̵ı�����
DOUBLE dbLevelSpeedupRatio=0.8;
// �˱�����ʾ���ٸ���������һ������ÿ50������
DWORD dwOneLevelScores=25;
int BoardUp = 0;//ÿ����ʱ�����ڰ�������һ��,ÿһ��ʱ�����ڣ�δ�ڰ��С������
int UpNum = 0;//����ÿ����3�Σ�����һ���°���//ÿ����������ӣ�����+1
int CreatNum=0;
int Status ;//��ʼС���Ƿ�λ��ľ��
Type TypeBoard;//��С���������İ�������
int LIFE; //����4��������������������������ĩβ���Ӹ���������꣬��Ϸ����
int Score;//�ȷ�
int tempScore=0;//ÿ����������ӣ�����+1
int GameOver = 0;//��LIFEΪ0ʱ����Ϊ1
int JumpNum=0;//�������������ǵ�������
int GameDiffculty;//��Ϸ�Ѷ�
int flag = 0;//���ڼ�¼����


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
	//������������ӣ�����������ɰ���
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	srand(ft.dwLowDateTime);
	
	//������Ϸ�߽磨������ģ�
	SetBoundary(boundary_x, boundary_y);


}
void CreateGame(HWND hwnd, int color, int diffculty, int balltype)
{
	//CreatGame�в�û���õ�color������GamePaint�л��õ��������������ø��ˡ���������
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
	}//�����Ѷ�������Ϸ����
	GameDiffculty = diffculty;
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	srand(ft.dwLowDateTime);//�������������
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
	if (i < 7)p->type = bNormal;//�ٷ�֮��ʮ�ĸ��ʲ�����ͨ����
	else if(i<9)
	{
		p->type = rand() % 4 + 1;//����������Ӳ���������ͬ
	}//�ٷ�֮��ʮ�ĸ��ʲ����������
	//�ٷ�֮ʮ�ĸ��ʲ���������
	board_list = ListCreate(NULL);
	ListPushFront(board_list, p);//��ͷ��������˵㣩������洢

}
void CreateBall(int balltype)
{
	PGAME_COORD tail = GetBoardTail();
	ball.x = tail->x+2;//��������һ�����ӣ�����С���ڰ������м�
	ball.y = tail->y - 1;//С����Ȼ�����ڰ����Ϸ���
	tail->type = bNormal;//Ϊ����С���Զ���ɲ�ͬ����
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
//Ҫ�޸İ����ƶ����ԣ����ڴ˺�����
void BoardMove(HWND hwnd)
{
	for (int i = 0; i < GetBoardNum(); i++)
	{
		PGAME_COORD temp = board_get_at(i);
		//�����ƶ��ٶ�������
		if (BoardUp == 1)
		{
			temp->y -= 1;
		}
		if (temp->type == bRmoving)
		{
			if (temp->x >= 11)
			{
				temp->type = bLMoving;
				temp->x -= 1;   //�����ƶ�����
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
			//bug,���ľ���غ�
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
			if (i < 65)p->type = bNormal;//�ٷ�֮65�ĸ��ʲ�����ͨ����
			else if(i<78)p->type = bWithTrap;
			else
			{
				p->type = i % 4 + 2;//����������Ӳ���������ͬ
			}//�ٷ�֮��ʮ�ĸ��ʲ����������
			 //�ٷ�֮ʮ�ĸ��ʲ���������
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
			if (i < 55)p->type = bNormal;//�ٷ�֮��ʮ�ĸ��ʲ�����ͨ����
			else if (i<72)p->type = bWithTrap;
			else
			{
				p->type = i % 5 + 1;//����������Ӳ���������ͬ
			}//�ٷ�֮��ʮ�ĸ��ʲ����������
			 //�ٷ�֮ʮ�ĸ��ʲ���������
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
			if (i < 50)p->type = bNormal;//�ٷ�֮��ʮ�ĸ��ʲ�����ͨ����
			else if (i<72)p->type = bWithTrap;
			else
			{
				p->type = i % 5 + 1;//����������Ӳ���������ͬ
			}//�ٷ�֮��ʮ�ĸ��ʲ����������
			 //�ٷ�֮ʮ�ĸ��ʲ���������
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
