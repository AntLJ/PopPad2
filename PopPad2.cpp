// PopPad2.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "PopPad2.h"

#define MAX_LOADSTRING 100
#define ID_EDIT 1

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。

    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_POPPAD2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーション初期化の実行:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, szWindowClass);

    MSG msg;

    // メイン メッセージ ループ:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

int AskConfirmation(HWND hwnd)
{
	return MessageBox(hwnd, TEXT("Really want to close PopPad2?"), szWindowClass, MB_YESNO | MB_ICONQUESTION);
}

//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_POPPAD2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_POPPAD2);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	   GetSystemMetrics(SM_CXSCREEN) / 4,
	   GetSystemMetrics(SM_CYSCREEN) / 4,
	   GetSystemMetrics(SM_CXSCREEN) / 2,
	   GetSystemMetrics(SM_CYSCREEN) / 2,
	   nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hwndEdit;
	int iSelect, iEnable;

    switch (message)
    {
	case WM_CREATE:
		hwndEdit = CreateWindow(TEXT("edit"), NULL,
			WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL |
			WS_BORDER | ES_LEFT | ES_MULTILINE |
			ES_AUTOHSCROLL | ES_AUTOVSCROLL,
			0, 0, 0, 0, hwnd, (HMENU)ID_EDIT,
			((LPCREATESTRUCT)lParam)->hInstance, NULL);
		return 0;

	case WM_SETFOCUS:
		SetFocus(hwndEdit);
		return 0;

	case WM_SIZE:
		MoveWindow(hwndEdit, 0, 0, LOWORD(lParam), HIWORD(lParam),
			TRUE);
		return 0;

	case WM_INITMENUPOPUP:
		if (lParam == 1)
		{
			EnableMenuItem((HMENU)wParam,
				IDM_EDIT_UNDO,
				SendMessage(hwndEdit, EM_CANUNDO, 0, 0) ? MF_ENABLED : MF_GRAYED);

			EnableMenuItem((HMENU)wParam,
				IDM_EDIT_PASTE,
				IsClipboardFormatAvailable(CF_TEXT) ? MF_ENABLED : MF_GRAYED);

			iSelect = SendMessage(hwndEdit, EM_GETSEL, 0, 0);
			if (HIWORD(iSelect) == LOWORD(iSelect))
				iEnable = MF_GRAYED;
			else
				iEnable = MF_ENABLED;

			EnableMenuItem((HMENU)wParam, IDM_EDIT_CUT, iEnable);
			EnableMenuItem((HMENU)wParam, IDM_EDIT_COPY, iEnable);
			EnableMenuItem((HMENU)wParam, IDM_EDIT_CLEAR, iEnable);
			return 0;
		}
		break;
	case WM_COMMAND:
		if (lParam)
		{
			if (LOWORD(lParam) == ID_EDIT && (HIWORD(wParam) == EN_ERRSPACE || HIWORD(wParam) == EN_MAXTEXT))
				MessageBox(hwnd, TEXT("Edit control out of space."), szWindowClass, MB_OK | MB_ICONSTOP);
			return 0;
		}
		else switch (LOWORD(wParam))
		{
		case IDM_FILE_NEW:
		case IDM_FILE_OPEN:
		case IDM_FILE_SAVE:
		case IDM_FILE_SAVE_AS:
		case IDM_FILE_PRINT:
			MessageBeep(0);
			return 0;

		case IDM_APP_EXIT:
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			return 0;
		case IDM_EDIT_UNDO:
			SendMessage(hwndEdit, WM_UNDO, 0, 0);
			return 0;

		case IDM_EDIT_CUT:
			SendMessage(hwndEdit, WM_CUT, 0, 0);
			return 0;

		case IDM_EDIT_COPY:
			SendMessage(hwndEdit, WM_COPY, 0, 0);
			return 0;
		case IDM_EDIT_PASTE:
			SendMessage(hwndEdit, WM_PASTE, 0, 0);
			return 0;

		case IDM_EDIT_CLEAR:
			SendMessage(hwndEdit, WM_CLEAR, 0, 0);
			return 0;
		case IDM_EDIT_SELECT_ALL:
			SendMessage(hwndEdit, EM_SETSEL, 0, -1);
			return 0;

		case IDM_HELP_HELP:
			MessageBox(hwnd, TEXT("Help not yet implemented!"), szWindowClass, MB_OK | MB_ICONEXCLAMATION);
			return 0;

		case IDM_APP_ABOUT:
			MessageBox(hwnd, TEXT("POPPAD2 (c) Charles Petzold, 1998"), szWindowClass, MB_OK | MB_ICONINFORMATION);
			return 0;
		}
		break;

	case WM_CLOSE:
		if (IDYES == AskConfirmation(hwnd))
			DestroyWindow(hwnd);
		return 0;

	case WM_QUERYENDSESSION:
		if (IDYES == AskConfirmation(hwnd))
			return 1;
		else
			return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
