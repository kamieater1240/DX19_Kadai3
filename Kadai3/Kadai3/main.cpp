﻿//===============================//
//         Main Program          //
//===============================//
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "main.h"
#include "mydirect3d.h"
#include "texture.h"
#include "sprite.h"
#include "SpriteAnim.h"
#include "system_timer.h"
#include "input.h"
#include "game.h"

#define CLASS_NAME		"GameWindow"
#define WINDOW_CAPTION	"ランニングマン動画"

LPDIRECT3DDEVICE9 myDevice;
double g_StaticFrameTime = 0.0;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	//オリジナルウインドウ
	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;	//ウインドウプロシージャコールバック関数の登録
	wc.lpszClassName = CLASS_NAME;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); //Cursorのタイプが変えられる
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1); //背景の色

	RegisterClass(&wc);			//オリジナルウインドウは自分でクラスをシステムに登録(RegisterClass)する必要がある

	//CreateWindowに指定する幅、高さは、スクリーンサイズに他の部分をたした長さで指定する
	RECT window_rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };	//左 上 右下
	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;
	int window_style = WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX);
	AdjustWindowRect(&window_rect, window_style, FALSE);

	//Windowを真ん中に置く
	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);
	int window_x = max((desktop_width - window_width) / 2, 0);
	int window_y = max((desktop_height - window_height) / 2, 0);

	HWND hWnd = CreateWindow(
		CLASS_NAME,
		WINDOW_CAPTION,
		window_style,
		window_x,												//最初のX座標
		window_y,												//最初のY座標
		window_width,											//Width
		window_height,											//Height
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	Init(hWnd);
	Keyboard_Initialize(hInstance, hWnd);	//Input Initialize

	//メッセージループ-->メッセージを受け取るまで、プログラムをブロックする
	//如果hWnd为NULL，则GetMessage接收属于调用线程的窗口的消息，线程消息由函数PostThreadMessage寄送给调用线程。
	//GetMessage不接收属于其他线程或其他线程的窗口的消息，即使hWnd为NULL。由PostThreadMessage寄送的线程消息，其消息hWnd值为NULL。
	//如果wMsgFilterMin和wMsgFilterMax都为零，GetMessage返回所有可得的消息（即，无范围过滤）。
	//lpMsg：指向MSG结构的指针，该结构从线程的消息队列里接收消息信息。
	//wMsgFilterMin：指定被检索的最小消息值的整数。
	//wMsgFilterMax：指定被检索的最大消息值的整数。
	MSG msg = {};
	while (WM_QUIT != msg.message) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			//ゲーム処理
			double time = SystemTimer_GetTime();

			if (time - g_StaticFrameTime < 1.0 / 60.0) {
				Sleep(0);
			}
			else {
				g_StaticFrameTime = time;
				Update();
				Draw();
			}
		}
	}
	Uninit();

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		}
		break;

	case WM_CLOSE:
		if (MessageBox(hWnd, "本当に終了してよろしいですか？", "確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK) {
			DestroyWindow(hWnd);
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	};

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool Init(HWND hWnd) {

	MyDirect3D_Init(hWnd);
	myDevice = MyDirect3D_GetDevice();

	gameInit();

	return true;
}

void Uninit(void) {
	if (myDevice != NULL) {
		myDevice->Release();
	}
}

void Update(void) {

	SpriteAnim_Update();
	Keyboard_Update();

	gameUpdate();
}

void Draw(void) {
	//画面のクリア,           　　　　 クリアしたいターゲット　　　　　　　　　　色　　　　　　　　　　　　　Z　　ステンシル
	myDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(152, 251, 152, 255), 1.0f, 0);

	//ポリゴン描画 1.頂点構造体を作ります　2.デバイスに頂点の形を伝えるためのFVFを宣言する
	//3.頂点データを作る　
	//4.ポリゴン描画命令, ※BeginScene~EndScene間に書く
	myDevice->BeginScene();	//BeginScene後一定要接EndScene
	myDevice->SetFVF(FVF_VERTEX2D);

	gameDraw();

	myDevice->EndScene();		//在Call下一個BeginScene之前一定要接EndScene
	myDevice->Present(NULL, NULL, NULL, NULL);

}