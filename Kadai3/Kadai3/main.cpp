//===============================//
//         Main Program          //
//===============================//
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <time.h> 
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

//==============================================Direct3D==============================================//
//
// Classes and Structures
//
struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float z,
		float nx, float ny, float nz,
		float u, float v)
	{
		_x = x;  _y = y;  _z = z;
		_nx = nx; _ny = ny; _nz = nz;
		_u = u;  _v = v;
	}
	float _x, _y, _z;
	float _nx, _ny, _nz;
	float _u, _v;

	static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

const int Width = 640;
const int Height = 480;

IDirect3DVertexBuffer9* VB = 0;

IDirect3DTexture9* FloorTex = 0;
IDirect3DTexture9* WallTex = 0;
IDirect3DTexture9* MirrorTex = 0;

D3DMATERIAL9 FloorMtrl = WHITE_MTRL;
D3DMATERIAL9 WallMtrl = WHITE_MTRL;
D3DMATERIAL9 MirrorMtrl = WHITE_MTRL;

ID3DXMesh* Teapot = 0;
D3DXVECTOR3 TeapotPosition(0.0f, 3.0f, -7.5f);
D3DMATERIAL9 TeapotMtrl = YELLOW_MTRL;

void RenderScene();
void RenderMirror();
//====================================================================================================//


LPDIRECT3DDEVICE9 Device;
double g_StaticFrameTime = 0.0;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	/*
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
	*/
	//
	// Create the main application window.
	//

	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = "Direct3D9App";

	if (!RegisterClass(&wc))
	{
		::MessageBox(0, "RegisterClass() - FAILED", 0, 0);
		return false;
	}

	HWND hWnd = 0;
	hWnd = CreateWindow("Direct3D9App", "Direct3D9App",
		WS_EX_TOPMOST,
		0, 0, 640, 480,
		0 /*parent hwnd*/, 0 /* menu */, hInstance, 0 /*extra*/);

	if (!hWnd)
	{
		::MessageBox(0, "CreateWindow() - FAILED", 0, 0);
		return false;
	}

	::ShowWindow(hWnd, SW_SHOW);
	::UpdateWindow(hWnd);


	Init(hWnd);
	//Keyboard_Initialize(hInstance, hWnd);	//Input Initialize

	//メッセージループ-->メッセージを受け取るまで、プログラムをブロックする
	//如果hWnd为NULL，则GetMessage接收属于调用线程的窗口的消息，线程消息由函数PostThreadMessage寄送给调用线程。
	//GetMessage不接收属于其他线程或其他线程的窗口的消息，即使hWnd为NULL。由PostThreadMessage寄送的线程消息，其消息hWnd值为NULL。
	//如果wMsgFilterMin和wMsgFilterMax都为零，GetMessage返回所有可得的消息（即，无范围过滤）。
	//lpMsg：指向MSG结构的指针，该结构从线程的消息队列里接收消息信息。
	//wMsgFilterMin：指定被检索的最小消息值的整数。
	//wMsgFilterMax：指定被检索的最大消息值的整数。
	MSG msg = {};

	static float lastTime = SystemTimer_GetTime();

	while (WM_QUIT != msg.message) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			//ゲーム処理
			double currTime = SystemTimer_GetTime();

			if (currTime - g_StaticFrameTime < 1.0 / 60.0) {
				Sleep(0);
			}
			else {
				float timeDelta = (currTime - lastTime)*0.001f;

				g_StaticFrameTime = currTime;
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

D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient = a;
	mtrl.Diffuse = d;
	mtrl.Specular = s;
	mtrl.Emissive = e;
	mtrl.Power = p;
	return mtrl;
}

bool Setup()
{
	Device = MyDirect3D_GetDevice();
	//
	// Make walls have low specular reflectance - 20%.
	//

	WallMtrl.Specular = WHITE * 0.2f;

	//
	// Create the teapot.
	//

	D3DXCreateTeapot(Device, &Teapot, 0);

	//
	// Create and specify geometry.  For this sample we draw a floor
	// and a wall with a mirror on it.  We put the floor, wall, and
	// mirror geometry in one vertex buffer.
	//
    //   |----|----|----|
    //   |Wall|Mirr|Wall|
	//   |    | or |    |
    //   /--------------/
    //  /   Floor      /
	// /--------------/
	//
	Device->CreateVertexBuffer(
		24 * sizeof(Vertex),
		0, // usage
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&VB,
		0);

	Vertex* v = 0;
	VB->Lock(0, 0, (void**)&v, 0);

	// floor
	v[0] = Vertex(-7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[1] = Vertex(-7.5f, 0.0f,   0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	v[2] = Vertex( 7.5f, 0.0f,   0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	
	v[3] = Vertex(-7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[4] = Vertex( 7.5f, 0.0f,   0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	v[5] = Vertex( 7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	// wall
	v[6]  = Vertex(-7.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[7]  = Vertex(-7.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[8]  = Vertex(-2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	
	v[9]  = Vertex(-7.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[10] = Vertex(-2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[11] = Vertex(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Note: We leave gap in middle of walls for mirror

	v[12] = Vertex(2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[13] = Vertex(2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[14] = Vertex(7.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	
	v[15] = Vertex(2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[16] = Vertex(7.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[17] = Vertex(7.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// mirror
	v[18] = Vertex(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[19] = Vertex(-2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[20] = Vertex( 2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	
	v[21] = Vertex(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[22] = Vertex( 2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[23] = Vertex( 2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	VB->Unlock();
	
	//
	// Load Textures, set filters.
	//

	D3DXCreateTextureFromFile(Device, "Asset/Texture/checker.jpg", &FloorTex);
	D3DXCreateTextureFromFile(Device, "Asset/Texture/brick0.jpg", &WallTex);
	D3DXCreateTextureFromFile(Device, "Asset/Texture/ice.bmp", &MirrorTex);

	Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	//
	// Lights.
	//

	D3DXVECTOR3 lightDir(0.707f, -0.707f, 0.707f);
	D3DXCOLOR color(1.0f, 1.0f, 1.0f, 1.0f);
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = color * 0.4f;
	light.Diffuse = color;
	light.Specular = color * 0.6f;
	light.Direction = lightDir;

	Device->SetLight(0, &light);
	Device->LightEnable(0, true);

	Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	Device->SetRenderState(D3DRS_SPECULARENABLE, true);

	//
	// Set Camera.
	//

	D3DXVECTOR3    pos(-10.0f, 3.0f, -15.0f);
	D3DXVECTOR3 target(0.0, 0.0f, 0.0f);
	D3DXVECTOR3     up(0.0f, 1.0f, 0.0f);

	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &pos, &target, &up);

	Device->SetTransform(D3DTS_VIEW, &V);

	//
	// Set projection matrix.
	//
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
			&proj,
			D3DX_PI / 4.0f, // 45 - degree
			(float)Width / (float)Height,
			1.0f,
			1000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);

	return true;
}

bool Init(HWND hWnd) {

	srand((unsigned)time(NULL));

	MyDirect3D_Init(hWnd);
	Device = MyDirect3D_GetDevice();

	Setup();

	//gameInit();

	return true;
}

void Uninit(void) {
	if (Device != NULL) {
		Device->Release();
	}
}

void Update() {

	//SpriteAnim_Update();
	//Keyboard_Update();

	//gameUpdate();

	static float radius = 20.0f;

	if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
		TeapotPosition.x -= 0.001f;

	if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
		TeapotPosition.x += 0.001f;

	if (::GetAsyncKeyState(VK_UP) & 0x8000f)
		radius -= 0.001f;

	if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
		radius += 0.001f;


	static float angle = (3.0f * D3DX_PI) / 2.0f;

	if (::GetAsyncKeyState('A') & 0x8000f)
		angle -= 0.0005f;

	if (::GetAsyncKeyState('S') & 0x8000f)
		angle += 0.0005f;

	D3DXVECTOR3 position(cosf(angle) * radius, 3.0f, sinf(angle) * radius);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &position, &target, &up);
	Device->SetTransform(D3DTS_VIEW, &V);
}

void Draw(void) {

	//画面のクリア,           　　　　 クリアしたいターゲット　　　　　　　　　　色　　　　　　　　　　　　　Z　　ステンシル
	//Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(135, 206, 235, 255), 1.0f, 0);
	Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0xff000000, 1.0f, 0L);

	//ID3DXMesh *mesh = 0;
	//D3DXCreateTeapot(myDevice, &mesh, 0);

	//ポリゴン描画 1.頂点構造体を作ります　2.デバイスに頂点の形を伝えるためのFVFを宣言する
	//3.頂点データを作る　
	//4.ポリゴン描画命令, ※BeginScene~EndScene間に書く
	Device->BeginScene();	//BeginScene後一定要接EndScene
	Device->SetFVF(FVF_VERTEX2D);

	RenderScene();
	RenderMirror();

	//gameDraw();

	Device->EndScene();		//在Call下一個BeginScene之前一定要接EndScene
	Device->Present(NULL, NULL, NULL, NULL);

	//mesh->Release();
	//mesh = 0;
}

void RenderScene()
{
	// draw teapot
	Device->SetMaterial(&TeapotMtrl);
	Device->SetTexture(0, 0);
	D3DXMATRIX W;
	D3DXMatrixTranslation(&W,
		TeapotPosition.x,
		TeapotPosition.y,
		TeapotPosition.z);

	Device->SetTransform(D3DTS_WORLD, &W);
	Teapot->DrawSubset(0);

	D3DXMATRIX I;
	D3DXMatrixIdentity(&I);
	Device->SetTransform(D3DTS_WORLD, &I);

	Device->SetStreamSource(0, VB, 0, sizeof(Vertex));
	Device->SetFVF(Vertex::FVF);

	// draw the floor
	Device->SetMaterial(&FloorMtrl);
	Device->SetTexture(0, FloorTex);
	Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

	// draw the walls
	Device->SetMaterial(&WallMtrl);
	Device->SetTexture(0, WallTex);
	Device->DrawPrimitive(D3DPT_TRIANGLELIST, 6, 4);

	// draw the mirror
	Device->SetMaterial(&MirrorMtrl);
	Device->SetTexture(0, MirrorTex);
	Device->DrawPrimitive(D3DPT_TRIANGLELIST, 18, 2);
}

void RenderMirror()
{
	//
	// Draw Mirror quad to stencil buffer ONLY.  In this way
	// only the stencil bits that correspond to the mirror will
	// be on.  Therefore, the reflected teapot can only be rendered
	// where the stencil bits are turned on, and thus on the mirror 
	// only.
	//

	Device->SetRenderState(D3DRS_STENCILENABLE, true);
	Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	Device->SetRenderState(D3DRS_STENCILREF, 0x1);
	Device->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
	Device->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
	Device->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	Device->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);

	// disable writes to the depth and back buffers
	Device->SetRenderState(D3DRS_ZWRITEENABLE, false);
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// draw the mirror to the stencil buffer
	Device->SetStreamSource(0, VB, 0, sizeof(Vertex));
	Device->SetFVF(Vertex::FVF);
	Device->SetMaterial(&MirrorMtrl);
	Device->SetTexture(0, MirrorTex);
	D3DXMATRIX I;
	D3DXMatrixIdentity(&I);
	Device->SetTransform(D3DTS_WORLD, &I);
	Device->DrawPrimitive(D3DPT_TRIANGLELIST, 18, 2);

	// re-enable depth writes
	Device->SetRenderState(D3DRS_ZWRITEENABLE, true);

	// only draw reflected teapot to the pixels where the mirror
	// was drawn to.
	Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

	// position reflection
	D3DXMATRIX W, T, R;
	D3DXPLANE plane(0.0f, 0.0f, 1.0f, 0.0f); // xy plane
	D3DXMatrixReflect(&R, &plane);

	D3DXMatrixTranslation(&T,
		TeapotPosition.x,
		TeapotPosition.y,
		TeapotPosition.z);

	W = T * R;

	// clear depth buffer and blend the reflected teapot with the mirror
	Device->Clear(0, 0, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	// Finally, draw the reflected teapot
	Device->SetTransform(D3DTS_WORLD, &W);
	Device->SetMaterial(&TeapotMtrl);
	Device->SetTexture(0, 0);

	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	Teapot->DrawSubset(0);

	// Restore render states.
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	Device->SetRenderState(D3DRS_STENCILENABLE, false);
	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}