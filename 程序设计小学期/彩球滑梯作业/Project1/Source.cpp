#include <Windows.h>
#include"Board.h"

LONG CALLBACK MainWndProc(
	HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
	);
void OnKeyDown(WPARAM P,HWND hwnd);
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
	RegisterClass(&wc);//WHY?
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
		hinstance,//��һ��������ֻ��ע��һ�����ڣ���
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

	HBRUSH hBrushBoard;//ˢľ��
	HBRUSH hBrushBall;//ˢС��
	HBRUSH hBrushTrap;//������
	HBRUSH hOldBrush;

	HFONT hFont;
	HFONT hOldFont;

	RECT rect;

	PGAME_COORD pBoardBody;
	PGAME_COORD lpBall;
	int i;

	GetClientRect(hwnd, &rect);
}

LONG CALLBACK MainWndProc(//callback���Բ�д�𣺲���
	HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam//������������ô�ã�ʲô����16\32
	)
{
	switch(msg)
	{
		case WM_KEYDOWN:
			OnKeyDown(wParam,hwnd);
			break;
		case WM_DESTROY://������Ͻǲ�棬���׹رճ��򣬲�д�ز���
			ExitProcess(0);
			break;
		default:
			break;
	}
	return DefWindowProc(hwnd,msg,wParam,lParam);
}
void OnKeyDown(WPARAM P,HWND hwnd)
{
	MessageBox(hwnd,"f*CK","SHIT",MB_OK);
}

