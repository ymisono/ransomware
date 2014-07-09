/*
ゲーム用のウィンドウをクラス化したもの
*/
#pragma once
#include<Windows.h>
#include<tchar.h>
#include"Input.h"

namespace Inferno
{
class SimpleWindow
{
public:
	//定数
	enum WindowPostion
	{
		Center,
		Left,
		Right,
	};

public:
	SimpleWindow();
	~SimpleWindow();
	virtual bool Initialize();
	void Resize(const int width, const int height);
	void Show() const; //現在の位置とサイズでウィンドウをアクティブにする
	void SetTitleText(LPCTSTR str) const;
	void SetPos(const WindowPostion x, const WindowPostion y);

	HWND GetHWND() const;
	int GetWindowWidth() const;
	int GetWindowHeight() const;
	int GetClientWidth() const;
	int GetClientHeight() const;

	virtual bool Terminate();

private:
	WNDCLASSEX m_wc;
	HWND m_hWnd; //自身を指すウィンドウハンドル

	//ウィンドウ枠を含めた領域（ウィンドウ領域）
	int m_windowWidth;
	int m_windowHeight;

	//ウィンドウ枠を除いた領域(クライアント領域)
	int m_clientWidth;
	int m_clientHeight;
	


private:
	SimpleWindow(SimpleWindow& w) = delete;
	SimpleWindow operator=(SimpleWindow& w) = delete;
};
}