#include <Windows.h>
#include"Ball.h"

LONG CALLBACK MainWndProc(
	HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
	);

/*----------------------define��--------------------------*/

//������ɫ


#define COLOR_BOUNDARY      RGB(255,255,255)//�߽���ɫ
#define COLOR_BACKG              RGB(0XEE,0XE9,0XBF)//��ҳ�汳����ɫ
#define COLOR_TEXT                  RGB(0,0,0)
#define Easy    0//���Ѷ�
#define Mid    1//�е��Ѷ�
#define Hard   2//�����Ѷ�

#define CELL_PIXEL			20
/*---------------------------------------------------------*/
// ȫ�ֱ���
HINSTANCE hinst; 
RECT rectBoundary;
Page page = MainPage1;//ҳ��
HBITMAP HBG1;//����1

int Diffculty = Easy;
int Color = RGB(0xff, 0x30, 0x30);

int ballType = Light;//С������


/*----------------------------------------------------------*/
//���ֺ�������
void OnKeyDown(WPARAM P,HWND hwnd);
void ReSizeGameWnd(HWND hwnd);

/*----------------------------------------------------------*/
LONG Bg(HWND hwnd);
int WINAPI WinMain(
	HINSTANCE hinstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	WNDCLASS wc;
	HWND hwnd;
	MSG msg;
	int fGotMessage;
	
	wc.style=CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc=MainWndProc;
	wc.cbClsExtra=0;
	wc.cbWndExtra=0;
	wc.hInstance=hinstance;
	wc.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	wc.hCursor=LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);//?
	wc.lpszMenuName="MainMenu";
	wc.lpszClassName="MainWClass";
	RegisterClass(&wc);
	hwnd=CreateWindow(
		"MainWClass",
		"С��",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(HWND)NULL,
		(HMENU)NULL,
		hinstance,
		(LPVOID)NULL);
	ShowWindow(hwnd,nCmdShow);
	UpdateWindow(hwnd);
	while((fGotMessage=GetMessage(&msg,(HWND)NULL,0,0))!=0&&fGotMessage!=-1)
		/*GetMessage��LPMSG lpMsg��HWND hWnd��UINT wMsgFilterMin��UINT wMsgFilterMax)
		���к�������ʾ������Ϣ����Сֵ�����ֵ*/
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;

}
void GamePaint(HWND hwnd)
{
	HPEN hpen;
	HDC hdc, hdcmem;
	HBITMAP hbmMem;

	HPEN hPenBoundary;
	HPEN hOldPen;

	HBRUSH hOldBrush;
	HBRUSH hBrushBackground;
	HBRUSH hBrushBoard;
	HBRUSH hBrushBall;
	HFONT hFont, hOldFont;

	//HBITMAP hTitle;
	BITMAP bmp;
	PGAME_COORD pBoardBody;
	PGAME_COORD lpBall;
	
	lpBall = GetBallBody();

	RECT rect;
	GetClientRect(hwnd, &rect);
	hdc = GetDC(hwnd);
	hdcmem = CreateCompatibleDC(hdc);
	hbmMem = CreateCompatibleBitmap(hdc,
		rect.right - rect.left, rect.bottom - rect.top);

	SelectObject(hdcmem, hbmMem);

	hpen = CreatePen(PS_SOLID, 1, RGB(5, 5, 5));
	hPenBoundary = CreatePen(0, 5, COLOR_BOUNDARY);
	hBrushBackground = CreateSolidBrush(COLOR_BACKG);
	hBrushBoard= CreateSolidBrush(RGB(0xa0,0x52,0x2d));
	hBrushBall = CreateSolidBrush(RGB(0xcd, 0x33, 0x33));
	FillRect(hdcmem, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

	hOldBrush = (HBRUSH)SelectObject(hdcmem, hBrushBackground);
	hOldPen = (HPEN)SelectObject(hdcmem, hpen);

	//������
	HBG1 = LoadImage(NULL, "title.bmp",
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (HBG1 == NULL)
	{
		MessageBox(hwnd, "bmp file not find", "ERROR!",
			MB_OK | MB_ICONERROR);
		//ExitProcess(0);
	}
	SelectObject(hdcmem, HBG1);
	GetObject(HBG1, sizeof(BITMAP), &bmp);
	SetStretchBltMode(hdcmem, STRETCH_HALFTONE);//���ͼƬ����ʧ��

	StretchBlt(hdcmem,
		rectBoundary.left, rectBoundary.top,
		rectBoundary.right - rectBoundary.left, rectBoundary.bottom - rectBoundary.top,
		hdcmem,
		0, 0,
		bmp.bmWidth, bmp.bmHeight,
		SRCCOPY);

	//������
	Rectangle(hdcmem,
	rectBoundary.left, rectBoundary.top,
	rectBoundary.right, rectBoundary.bottom
	);

	//��ľ��
	SelectObject(hdcmem, hpen);



	
	SelectObject(hdcmem, hBrushBoard);
	DeleteObject(hBrushBoard);
	for (int i = 0; i < GetBoardNum(); i++)
	{
		pBoardBody = (PGAME_COORD)board_get_at(i);
		if (pBoardBody->type == bWithTrap)
		{
			
			hBrushBoard = CreateSolidBrush(RGB(0x40, 0x40, 0x40));
			hOldBrush = (HBRUSH)SelectObject(hdcmem, hBrushBoard);
			for (int j = 0; j < 5; j++)
			{
				POINT a[3] = { (pBoardBody->x + j)*CELL_PIXEL + rectBoundary.left,pBoardBody->y*CELL_PIXEL + rectBoundary.top,
					(pBoardBody->x + j)*CELL_PIXEL + rectBoundary.left,(pBoardBody->y + 1)*CELL_PIXEL + rectBoundary.top,
					(pBoardBody->x + j + 1)*CELL_PIXEL + rectBoundary.left,(pBoardBody->y + 1)*CELL_PIXEL + rectBoundary.top
				};
				Polygon(hdcmem, a, 3);
			}
		}
		else
		{
			switch (pBoardBody->type) {
			case bNormal:
				hBrushBoard = CreateSolidBrush(RGB(0xa0, 0x52, 0x2d));
				hOldBrush = (HBRUSH)SelectObject(hdcmem, hBrushBoard);
				break;
			case bLMoving:
				hBrushBoard = CreateSolidBrush(RGB(0x2b, 0x2b, 0x2b));
				hOldBrush = (HBRUSH)SelectObject(hdcmem, hBrushBoard);
				break;
			case bRmoving:
				hBrushBoard = CreateSolidBrush(RGB(0x2b, 0x2b, 0x2b));
				hOldBrush = (HBRUSH)SelectObject(hdcmem, hBrushBoard);
				break;
			case bJumping:
				hBrushBoard = CreateSolidBrush(RGB(0xcd, 0x85, 0x3f));
				hOldBrush = (HBRUSH)SelectObject(hdcmem, hBrushBoard);
				break;
			case bBroken:
				hBrushBoard = CreateSolidBrush(RGB(0xBF, 0xEF, 0xFF));
				hOldBrush = (HBRUSH)SelectObject(hdcmem, hBrushBoard);
				break;
			default:
				break;
			}
			Rectangle(hdcmem,
				pBoardBody->x*CELL_PIXEL + rectBoundary.left,
				pBoardBody->y*CELL_PIXEL + rectBoundary.top,
				(pBoardBody->x + 5)*CELL_PIXEL + rectBoundary.left,
				(pBoardBody->y + 1)*CELL_PIXEL + rectBoundary.top
			);
		}
	}
	//��С��
	SelectObject(hdcmem, hpen);
	SelectObject(hdcmem, hBrushBall);
	Ellipse(hdcmem,
		lpBall->x * CELL_PIXEL + rectBoundary.left,
		lpBall->y * CELL_PIXEL + rectBoundary.top,
		(lpBall->x + 1)*CELL_PIXEL + rectBoundary.left,
		(lpBall->y + 1)*CELL_PIXEL + rectBoundary.top);
	//����������
	SelectObject(hdcmem, hBrushBoard);
	SelectObject(hdcmem, hpen);
	DeleteObject(hBrushBoard);
	DeleteObject(hpen);
	hpen = CreatePen(PS_NULL, 1, RGB(5, 5, 5));
	hBrushBoard = CreateSolidBrush(RGB(0x40, 0x40, 0x40));
	hOldBrush = (HBRUSH)SelectObject(hdcmem, hBrushBoard);
	hOldPen = (HPEN)SelectObject(hdcmem, hpen);
	for (int j = 0; j < 16; j++)
	{
		POINT a[3] = {  j*CELL_PIXEL + rectBoundary.left,0*CELL_PIXEL + rectBoundary.top,
			(j+1)*CELL_PIXEL + rectBoundary.left,0*CELL_PIXEL + rectBoundary.top,
			(j + 1)*CELL_PIXEL + rectBoundary.left,CELL_PIXEL + rectBoundary.top
		};
		Polygon(hdcmem, a, 3);
	}


	// ������һ���������
	hFont = CreateFont(-30, -15, 0, 0, 400, 0, 0, 0, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("΢���ź�"));

	// �����FONT�������DC��
	if (hOldFont = (HFONT)SelectObject(hdcmem, hFont))
	{
		CHAR szSourceInfo[1024];
		wsprintf(szSourceInfo, "Sorce %d   Life %d", GetScore(), GetLife());
		// ���������ɫ
		SetTextColor(hdcmem, RGB(0X68, 0X22, 0X8b));
		//�������屳��͸��
		SetBkMode(hdcmem, TRANSPARENT);
		// ����ַ�����
		TextOut(hdcmem, rectBoundary.left + 3, rectBoundary.bottom + 3,
			szSourceInfo, lstrlen(szSourceInfo));
		// �����ɣ���ԭ�����������Ż�DC��
		SelectObject(hdcmem, hOldFont);
	}
	

	BitBlt(hdc,
		0, 0, rect.right - rect.left, rect.bottom - rect.top,
		hdcmem, 0, 0, SRCCOPY);
	//��������
	DeleteObject(hbmMem);
	DeleteObject(hdcmem);
	DeleteObject(hpen);
	DeleteObject(hPenBoundary);
	DeleteObject(hFont);
	DeleteObject(hBrushBackground);
	DeleteDC(hdcmem);
	ReleaseDC(hwnd, hdc);


}
void MainPaint(HWND hwnd)
{
	HPEN hpen;
	HDC hdc, hdcmem;
	HBITMAP hbmMem;

	HPEN hPenBoundary;
	HPEN hOldPen;

	HBRUSH hOldBrush;
	HBRUSH hBrushBackground;
	HFONT hFont, hOldFont;

	//HBITMAP hTitle;
	BITMAP bmp;

	RECT rect;

	GetClientRect(hwnd, &rect);

	hdc = GetDC(hwnd);

	hdcmem = CreateCompatibleDC(hdc);
	hbmMem = CreateCompatibleBitmap(hdc,
		rect.right - rect.left, rect.bottom - rect.top);

	SelectObject(hdcmem, hbmMem);

	hpen = CreatePen(PS_SOLID, 1, RGB(5, 5, 5));
	hPenBoundary = CreatePen(0, 5, COLOR_BOUNDARY);
	hBrushBackground = CreateSolidBrush(COLOR_BACKG);

	FillRect(hdcmem, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

	hOldBrush = (HBRUSH)SelectObject(hdcmem, hBrushBackground);
	hOldPen = (HPEN)SelectObject(hdcmem, hpen);

	
	//������
	HBG1= LoadImage(NULL, "title.bmp",
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	
	if (HBG1 == NULL)
	{
		MessageBox(hwnd, "bmp file not find", "ERROR!",
			MB_OK | MB_ICONERROR);
		//ExitProcess(0);
	}
	SelectObject(hdcmem, HBG1);
	GetObject(HBG1, sizeof(BITMAP), &bmp);
	SetStretchBltMode(hdcmem, STRETCH_HALFTONE);//���ͼƬ����ʧ��

	StretchBlt(hdcmem,
		rectBoundary.left, rectBoundary.top,
		rectBoundary.right - rectBoundary.left, rectBoundary.bottom - rectBoundary.top,
		hdcmem,
		0, 0,
		bmp.bmWidth, bmp.bmHeight,
		SRCCOPY);
	
	//������

	/*Rectangle(hdcmem,
		rectBoundary.left, rectBoundary.top,
		rectBoundary.right, rectBoundary.bottom
	);*/
	//���߽�

	SelectObject(hdcmem, hPenBoundary);
	MoveToEx(hdcmem, rectBoundary.left, rectBoundary.top, NULL);
	// ����һ��������ʾLineTo����
	LineTo(hdcmem, rectBoundary.left, rectBoundary.bottom);
	LineTo(hdcmem, rectBoundary.right, rectBoundary.bottom);
	LineTo(hdcmem, rectBoundary.right, rectBoundary.top);
	LineTo(hdcmem, rectBoundary.left, rectBoundary.top);
	//д��

	hFont = CreateFont(-30, 0, 0, 0, 400, 0, 0, 0, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("΢���ź�"));

	// �����FONT�������DC��
	if (hOldFont = (HFONT)SelectObject(hdcmem, hFont))
	{
		CHAR szSourceInfo[1024];
		wsprintf(szSourceInfo, "��ʼ��Ϸ");
		// ���������ɫ
		SetTextColor(hdcmem, COLOR_TEXT);
		//�������屳��͸��
		SetBkMode(hdcmem, TRANSPARENT);
		// ����ַ�����
		TextOut(hdcmem, (rectBoundary.left + rectBoundary.right)/2-60, (rectBoundary.bottom+rectBoundary.top)/2 - 100,
			szSourceInfo, lstrlen(szSourceInfo));
		// �����ɣ���ԭ�����������Ż�DC��
		SelectObject(hdcmem, hOldFont);
	}
	if (hOldFont = (HFONT)SelectObject(hdcmem, hFont))
	{
		CHAR szSourceInfo[1024];
		wsprintf(szSourceInfo, "��Ϸ����");
		// ���������ɫ
		SetTextColor(hdcmem, COLOR_TEXT);
		//�������屳��͸��
		SetBkMode(hdcmem, TRANSPARENT);
		// ����ַ�����
		TextOut(hdcmem, (rectBoundary.left + rectBoundary.right) / 2 - 60, (rectBoundary.bottom + rectBoundary.top) / 2 - 40,
			szSourceInfo, lstrlen(szSourceInfo));
		// �����ɣ���ԭ�����������Ż�DC��
		SelectObject(hdcmem, hOldFont);
	}
	if (hOldFont = (HFONT)SelectObject(hdcmem, hFont))
	{
		CHAR szSourceInfo[1024];
		wsprintf(szSourceInfo, "�˳���Ϸ");
		// ���������ɫ
		SetTextColor(hdcmem, COLOR_TEXT);
		//�������屳��͸��
		SetBkMode(hdcmem,TRANSPARENT);
		// ����ַ�����
		TextOut(hdcmem, (rectBoundary.left + rectBoundary.right) / 2 - 60, (rectBoundary.bottom + rectBoundary.top) / 2+20 ,
			szSourceInfo, lstrlen(szSourceInfo));
		// �����ɣ���ԭ�����������Ż�DC��
		SelectObject(hdcmem, hOldFont);
	}

	//��ѡ���
	switch (page) {
	case MainPage1:
		MoveToEx(hdcmem, (rectBoundary.left + rectBoundary.right) / 2 - 65, (rectBoundary.bottom + rectBoundary.top) / 2 - 100, NULL);
		LineTo(hdcmem, (rectBoundary.left + rectBoundary.right) / 2 - 65, (rectBoundary.bottom + rectBoundary.top) / 2 - 100+40);
		LineTo(hdcmem, (rectBoundary.left + rectBoundary.right) / 2 + 65, (rectBoundary.bottom + rectBoundary.top) / 2 - 100 + 40);
		LineTo(hdcmem, (rectBoundary.left + rectBoundary.right) / 2 + 65, (rectBoundary.bottom + rectBoundary.top) / 2 - 100);
		LineTo(hdcmem, (rectBoundary.left + rectBoundary.right) / 2 - 65, (rectBoundary.bottom + rectBoundary.top) / 2 - 100 );
		break;
	case MainPage2:
		MoveToEx(hdcmem, (rectBoundary.left + rectBoundary.right) / 2 - 65, (rectBoundary.bottom + rectBoundary.top) / 2 - 40, NULL);
		LineTo(hdcmem, (rectBoundary.left + rectBoundary.right) / 2 - 65, (rectBoundary.bottom + rectBoundary.top) / 2 - 40 + 40);
		LineTo(hdcmem, (rectBoundary.left + rectBoundary.right) / 2 + 65, (rectBoundary.bottom + rectBoundary.top) / 2 - 40 + 40);
		LineTo(hdcmem, (rectBoundary.left + rectBoundary.right) / 2 + 65, (rectBoundary.bottom + rectBoundary.top) / 2 - 40);
		LineTo(hdcmem, (rectBoundary.left + rectBoundary.right) / 2 - 65, (rectBoundary.bottom + rectBoundary.top) / 2 - 40);
		break;
	case MainPage3:
		MoveToEx(hdcmem, (rectBoundary.left + rectBoundary.right) / 2 - 65, (rectBoundary.bottom + rectBoundary.top) / 2 +20, NULL);
		LineTo(hdcmem, (rectBoundary.left + rectBoundary.right) / 2 - 65, (rectBoundary.bottom + rectBoundary.top) / 2 +20 + 40);
		LineTo(hdcmem, (rectBoundary.left + rectBoundary.right) / 2 + 65, (rectBoundary.bottom + rectBoundary.top) / 2 +20 + 40);
		LineTo(hdcmem, (rectBoundary.left + rectBoundary.right) / 2 + 65, (rectBoundary.bottom + rectBoundary.top) / 2 +20);
		LineTo(hdcmem, (rectBoundary.left + rectBoundary.right) / 2 - 65, (rectBoundary.bottom + rectBoundary.top) / 2 +20);
		break;
	}

	BitBlt(hdc,
		0, 0, rect.right - rect.left, rect.bottom - rect.top,
		hdcmem, 0, 0, SRCCOPY);

	DeleteObject(hbmMem);
	DeleteObject(hdcmem);
	DeleteObject(hpen);
	DeleteObject(hPenBoundary);
	DeleteObject(hFont);
	DeleteObject(hBrushBackground);
	DeleteDC(hdcmem);
	ReleaseDC(hwnd, hdc);
}
void OptionPaint(HWND hwnd)
{

}

LONG CALLBACK MainWndProc(
	HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
	)
{
	switch(msg)
	{
	    case WM_CREATE:
			CreateMain(hwnd, 15, 30);
			ReSizeGameWnd(hwnd);
			Bg(hwnd);//���뱳��
			break;
		case WM_KEYDOWN:
			OnKeyDown(wParam,hwnd);
			break;
		case WM_DESTROY://������Ͻǲ�棬���׹رճ��򣬲�д�ز���
			ExitProcess(0);
			break;
		case WM_PAINT:
			if (page <= MainPage3)
			{
				MainPaint(hwnd);
			}
			else if (page <= GamePage)
			{
				GamePaint(hwnd);
			}
			else
			{
				OptionPaint(hwnd);
			}
			
			break;
		case WM_TIMER:
			
			OnTimer(hwnd);//��ʱ��������������
			if (isGameOver())
			{
				KillTimer(hwnd, 12340);
				MessageBox(hwnd, "f*CK", "SHIT", MB_OK);
				Destory();
				page = MainPage1;
				MainPaint(hwnd);
			}
			else
			{
				GamePaint(hwnd);
			}
			/*KillTimer(hwnd, 12340);
			MessageBox(hwnd, "f*CK", "SHIT", MB_OK| MB_OKCANCEL);
			page = MainPage3;
			MainPaint(hwnd);*/
			break;
		default:
			break;
	}
	return DefWindowProc(hwnd,msg,wParam,lParam);
}
void OnKeyDown(WPARAM P,HWND hwnd)
{
	switch (P) {
	case VK_UP:
		switch (page) {
		case MainPage1:
			page = MainPage3;
			MainPaint(hwnd);
			break;
		case MainPage2:
			page = MainPage1;
			MainPaint(hwnd);
			break;
		case MainPage3:
			page = MainPage2;
			MainPaint(hwnd);
			break;
		default:
			break;
		}
		break;
	case VK_DOWN:
		switch (page) {
		case MainPage1:
			page = MainPage2;
			MainPaint(hwnd);
			break;
		case MainPage2:
			page = MainPage3;
			MainPaint(hwnd);
			break;
		case MainPage3:
			page = MainPage1;
			MainPaint(hwnd);
			break;
		default:
			break;
		}
		break;
	case VK_RETURN:
		switch (page) {
		case MainPage1:
			CreateGame(hwnd, Color, Diffculty, ballType);
			GamePaint(hwnd);
			page = GamePage;
			break;
		default:
			break;
		}
	case VK_LEFT:
		switch (page)
		{
		case GamePage:
			MoveLBall();
			GamePaint(hwnd);
			break;
		default:
			break;
		}
		break;
	case VK_RIGHT:
		switch (page)
		{
		case GamePage:
			MoveRBall();
			GamePaint(hwnd);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	//MessageBox(hwnd,"f*CK","SHIT",MB_OK);
	//if (page == MainPage2) { SetTimer(hwnd, 12340, 300, NULL); }
}







void ReSizeGameWnd(HWND hwnd)
{
	POINT ptLeftTop;		// ���Ͻ�
	POINT ptRightBottom;	// ���½�
	RECT rectWindow;
	PGAME_COORD pCoordBoundary = GetBoundary();

	// ������Ϸ�߽�
	rectBoundary.left = 10;
	rectBoundary.top = 10;
	rectBoundary.right = 10 + CELL_PIXEL*(pCoordBoundary->x + 1);
	rectBoundary.bottom =  10+CELL_PIXEL*(pCoordBoundary->y + 1);

	// �����������ҽǵ�λ��
	ptLeftTop.x = rectBoundary.left;
	ptLeftTop.y = rectBoundary.top;
	ptRightBottom.x = rectBoundary.right;
	ptRightBottom.y = rectBoundary.bottom;
	ClientToScreen(hwnd, &ptLeftTop);
	ClientToScreen(hwnd, &ptRightBottom);

	GetWindowRect(hwnd, &rectWindow);
	// ������ˣ����ô��ڴ�С��
	MoveWindow(hwnd,
		0,
		0,
		ptLeftTop.x - rectWindow.left + ptRightBottom.x - rectWindow.left, // ����߽���������߱߿���ȡ�
		rectBoundary.bottom+80, //��������Ϣ������ʾ�ռ䡣
		TRUE);
}
LONG Bg(HWND hwnd)
{
	//HBG1= LoadImage(NULL, "title.bmp",
	//	IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	//
	//if (HBG1 == NULL)
	//{
	//	MessageBox(hwnd, "bmp file not find", "ERROR!",
	//		MB_OK | MB_ICONERROR);
	//	//ExitProcess(0);
	//}
	//SetFocus(hwnd);
	return 0;
}

