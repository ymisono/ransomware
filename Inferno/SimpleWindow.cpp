#include"SimpleWindow.h"
#include"Input.h"
#include<Windows.h>
#include<tchar.h>
#include"MyException.h"
#include"..\TString.h"
#include"MyTypes.h"
#include"..\resource.h"

extern Inferno::Input* g_GlobalInput; //とりあえず

namespace
{
LRESULT WINAPI SimpleWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
}
LPCTSTR className = TEXT("SimpleWindow");

namespace Inferno
{
SimpleWindow::SimpleWindow()
{
}

SimpleWindow::~SimpleWindow()
{
	UnregisterClass(className, m_wc.hInstance);
}

bool SimpleWindow::Initialize()
{
	//WindowClass設定
	m_wc.cbSize = sizeof(WNDCLASSEX);
	m_wc.style = CS_CLASSDC | CS_HREDRAW | CS_VREDRAW; //CS_CLASSDC:全てのウィンドウが同じデバイスコンテキストを共有する
	m_wc.lpfnWndProc = SimpleWindowProc;
	m_wc.cbClsExtra = 0L;
	m_wc.cbWndExtra = 0L;
	m_wc.hInstance = GetModuleHandle(NULL);
	m_wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	m_wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	m_wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	m_wc.lpszMenuName = NULL;
	m_wc.lpszClassName = className;
	m_wc.hIconSm = NULL;
	//WindowClass登録
	if (!RegisterClassEx(&m_wc))
	{
		throw Inferno::CreationFailed(TEXT("ウィンドウクラス"));
	}

	//ウィンドウを生成
	m_hWnd = CreateWindow(className, TEXT("SimpleWindow"),
		WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX, //|WS_MAXIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, //ウィンドウ位置
		CW_USEDEFAULT, CW_USEDEFAULT, //ウィンドウサイズ
		GetDesktopWindow(), //ここは注意
		NULL, m_wc.hInstance, NULL);
	if (m_hWnd == NULL)
	{
		throw Inferno::CreationFailed(TEXT("ウィンドウ"));
	}

	this->Resize(800, 600);

	//::SetProcessDPIAware();

	return true;
}

void SimpleWindow::Resize(const int width, const int height)
{
	// ウィンドウサイズを再設定する
	RECT rect;
	int ww, wh;
	int cw, ch;
	// クライアント領域の外の幅を計算
	GetClientRect(m_hWnd, &rect);		// クライアント部分のサイズの取得
	cw = rect.right - rect.left;	// クライアント領域外の横幅を計算
	ch = rect.bottom - rect.top;	// クライアント領域外の縦幅を計算

	// ウインドウ全体の横幅の幅を計算
	GetWindowRect(m_hWnd, &rect);		// ウインドウ全体のサイズ取得
	ww = rect.right - rect.left;	// ウインドウ全体の幅の横幅を計算
	wh = rect.bottom - rect.top;	// ウインドウ全体の幅の縦幅を計算
	ww = ww - cw;					// クライアント領域以外に必要な幅
	wh = wh - ch;					// クライアント領域以外に必要な高さ

	// ウィンドウサイズの再計算
	ww = width + ww;			// 必要なウインドウの幅
	wh = height + wh;		// 必要なウインドウの高さ

	// ウインドウサイズの再設定
	SetWindowPos(m_hWnd, HWND_TOP, 0, 0, ww, wh, SWP_NOMOVE);

	m_windowHeight = wh;
	m_windowWidth = ww;
	m_clientHeight = height;
	m_clientWidth = width;
}

void SimpleWindow::SetPos(const WindowPostion x, const WindowPostion y)
{
	// ウインドウ全体の横幅の幅を計算
	RECT rect;
	GetWindowRect(m_hWnd, &rect);		// ウインドウ全体のサイズ取得
	const int ww = rect.right - rect.left;	// ウインドウ全体の幅の横幅を計算
	const int wh = rect.bottom - rect.top;	// ウインドウ全体の幅の縦幅を計算

	//result
	int rx = 0;
	int ry = 0;

	//位置を座標に直す
	switch (x)
	{
	case Center:
		rx = (GetSystemMetrics(SM_CXSCREEN) - ww) / 2;
		if (rx < 0) rx=0; //画面が小さすぎた場合
		break;
	default:
		break;
	}

	switch (y)
	{
	case Center:
		ry = (GetSystemMetrics(SM_CYSCREEN) - wh) / 2;
		if (ry < 0) ry = 0; //画面が小さすぎた場合
		break;
	default:
		break;
	}

	//adjust (offset)
	int ax = 0;
	int ay = 0;

	//タスクバーがあった場合調整する
	//タスクバーの位置を矩形で入手
	APPBARDATA sBarInfo;
	ZeroMemory(&sBarInfo, sizeof(APPBARDATA));
	sBarInfo.cbSize = sizeof(APPBARDATA);
	sBarInfo.hWnd = m_hWnd;
	SHAppBarMessage(ABM_GETTASKBARPOS, &sBarInfo);
	RECT r = sBarInfo.rc;
	//タスクバーが左にあった時の対処
	if (r.left <= 0 && r.top <= 0 &&//タスクバーの左端と上端が0以下で
		r.right <= r.bottom //右辺より下辺が長い場合
		)
	{
		ax = r.right; //タスクバー分だけ領域を減らす
	}
	//もしデスクトップをオーバーした場合は
	if (rx + ax + ww > GetSystemMetrics(SM_CXSCREEN))
	{
		//タスクバー、もしくはデスクトップ端にジャストくっつようにする
		rx = ax - (ww - m_clientWidth)/2;  //後ろの値はウィンドウ枠だけズラしている
		ax = 0;
	}

	//タスクバーが上にあった時の対処
	if (r.left <= 0 && r.top <= 0 &&//タスクバーの左端と上端が0以下で
		r.bottom <= r.right //下辺より右辺が長い場合
		)
	{
		ay = r.bottom;//タスクバー分だけ領域を減らす
	}
	//もしデスクトップをオーバーした場合は
	if (ry + ay + wh > GetSystemMetrics(SM_CYSCREEN))
	{
		ry = ay - (wh - m_clientHeight)/2; //タスクバー、もしくはデスクトップ端にジャストくっつようにする
		ay = 0;
	}
	

	::SetWindowPos(m_hWnd, HWND_TOP, rx+ax, ry+ay, 0, 0, SWP_NOSIZE);
}

void SimpleWindow::Show() const
{
	::ValidateRect(m_hWnd, 0);
	::ShowWindow(m_hWnd, SW_SHOW);
	::UpdateWindow(m_hWnd); //WM_PAINTメッセージを出して、領域を更新
}

void SimpleWindow::SetTitleText(LPCTSTR str) const
{
	::SetWindowText(m_hWnd, str);
}


HWND SimpleWindow::GetHWND() const
{
	return m_hWnd;
}

int SimpleWindow::GetWindowWidth() const
{
	return m_windowWidth;
}

int SimpleWindow::GetWindowHeight() const
{
	return m_windowHeight;
}

int SimpleWindow::GetClientWidth() const
{
	return m_clientWidth;
}

int SimpleWindow::GetClientHeight() const
{
	return m_clientHeight;
}

bool SimpleWindow::Terminate()
{
	DestroyWindow(m_hWnd);
	return true;
}

}

namespace
{
LRESULT WINAPI SimpleWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, hBuffer;
	PAINTSTRUCT ps;
	static HBITMAP hBitmap;

	switch (msg) {
	case WM_DESTROY:
		DeleteObject(hBitmap);
		PostQuitMessage(0);
		return 0;
	case WM_CREATE:
		hBitmap = LoadBitmap(
			((LPCREATESTRUCT)lParam)->hInstance,
			(LPCWSTR)IDB_BITMAP1
			);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		hBuffer = CreateCompatibleDC(hdc);
		SelectObject(hBuffer, hBitmap);
		
		BITMAP bmp;
		::GetObject(hBitmap, sizeof(BITMAP), &bmp);

		BitBlt(hdc, 0, 0, bmp.bmWidth, bmp.bmHeight, hBuffer, 0, 0, SRCCOPY);

		DeleteDC(hBuffer);
		EndPaint(hWnd, &ps);
		return 0;
	}
	return (DefWindowProc(hWnd, msg, wParam, lParam));
}
}

