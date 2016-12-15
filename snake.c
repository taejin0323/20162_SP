#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DigProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("AstroBoy Window");

struct rt
{
	int x;
	int y;
};

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW|CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,908,550,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;

	static struct rt snake[50] = {0};
	static int tempSnakeLen = 50;

	static int xAdd, yAdd;
	static BOOL start = FALSE;
	int count;
	int testX, testY;

	switch(iMessage)
	{
	case WM_CREATE:
		GetClientRect(hWnd, &rect);
		snake[0].x = (rect.right / 2) - 5;
		snake[0].y = (rect.bottom / 2) - 5;
		for (count = tempSnakeLen - 1; count > 0; count--)	snake[count] = snake[0];
		return 0;
	case WM_KEYDOWN:
		if(start == FALSE)
		{
			SetTimer(hWnd, 0, 100, NULL);
			start = TRUE;
		}
		switch(wParam)
		{
		case VK_LEFT:
			xAdd = -9;
			yAdd = 0;
			break;
		case VK_UP:
			xAdd = 0;
			yAdd = -9;
			break;
		case VK_RIGHT:
			xAdd = 9;
			yAdd = 0;
			break;
		case VK_DOWN:
			xAdd = 0;
			yAdd = 9;
			break;
		}
		return 0;
	case WM_TIMER:
		for (count = tempSnakeLen - 1; count > 0; count--)	snake[count] = snake[count - 1];
		snake[0].x += xAdd;
		snake[0].y += yAdd;
		//충돌 
		for (count = tempSnakeLen - 1; count > 0; count--)
		{
			if ((snake[0].x == snake[count].x && snake[0].y == snake[count].y) || (snake[0].x+10 <= 0 || snake[0].y+10 <= 0 || snake[0].x >= 908 || snake[0].y >= 550))
			{
				KillTimer(hWnd, 0);
				DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DigProc);
				SendMessage(hWnd, WM_DESTROY, 0, 0);
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		for (count = 0; count < tempSnakeLen; count++)
		{
			testX = snake[count].x;
			testY = snake[count].y;
			Rectangle(hdc, testX, testY, testX + 10, testY + 10);
		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		KillTimer(hWnd, 0);
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

BOOL CALLBACK DigProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch(iMessage)
	{
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hDlg, IDOK);
			return TRUE;
		}
		break;
	}
	return FALSE;
}
