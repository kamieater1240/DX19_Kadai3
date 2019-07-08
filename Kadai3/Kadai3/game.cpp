#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "game.h"
#include "main.h"
#include "mydirect3d.h"
#include "animation.h"
#include "input.h"
#include "system_timer.h"
#include "debug_font.h"

int g_FrameCount;				//フレームカウンター
int g_FPSBaseFrameCount;		//FPS計測用フレームカウンター
double g_FPSBaseTime;			//FPS計測用時間
float g_FPS;					//FPS

//BackGround vertices
Vertex2d sportsField[] = {
	{D3DXVECTOR4(		 0.0f,        450.0f, 0.0f, 1.0f), D3DCOLOR_RGBA(194, 69, 45, 255)},
	{D3DXVECTOR4(SCREEN_WIDTH,        450.0f, 0.0f, 1.0f), D3DCOLOR_RGBA(194, 69, 45, 255)},
	{D3DXVECTOR4(		 0.0f, SCREEN_HEIGHT, 0.0f, 1.0f), D3DCOLOR_RGBA(194, 69, 45, 255)},
	{D3DXVECTOR4(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f), D3DCOLOR_RGBA(194, 69, 45, 255)}
};

//Track Lines
Vertex2d trackLines[6][4] = {
	//Line 1
	{
		{D3DXVECTOR4(0.0f, 450.0f, 0.0f, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)}, {D3DXVECTOR4(SCREEN_WIDTH, 450.0f, 0.0f, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
		{D3DXVECTOR4(0.0f, 455.0f, 0.0f, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)}, {D3DXVECTOR4(SCREEN_WIDTH, 455.0f, 0.0f, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)}
	},

	//Line 2
	{
		{D3DXVECTOR4(0.0f, 503.0f, 0.0f, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)}, {D3DXVECTOR4(SCREEN_WIDTH, 503.0f, 0.0f, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
		{D3DXVECTOR4(0.0f, 508.0f, 0.0f, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)}, {D3DXVECTOR4(SCREEN_WIDTH, 508.0f, 0.0f, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)}
	},

	//Line 3
	{
		{D3DXVECTOR4(0.0f, 556.0f, 0.0f, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)}, {D3DXVECTOR4(SCREEN_WIDTH, 556.0f, 0.0f, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
		{D3DXVECTOR4(0.0f, 561.0f, 0.0f, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)}, {D3DXVECTOR4(SCREEN_WIDTH, 561.0f, 0.0f, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)}
	},

	//Line 4
	{
		{D3DXVECTOR4(0.0f, 609.0f, 0.0f, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)}, {D3DXVECTOR4(SCREEN_WIDTH, 609.0f, 0.0f, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
		{D3DXVECTOR4(0.0f, 614.0f, 0.0f, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)}, {D3DXVECTOR4(SCREEN_WIDTH, 614.0f, 0.0f, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)}
	},

	//Line 5
	{
		{D3DXVECTOR4(0.0f, 662.0f, 0.0f, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)}, {D3DXVECTOR4(SCREEN_WIDTH, 662.0f, 0.0f, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
		{D3DXVECTOR4(0.0f, 667.0f, 0.0f, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)}, {D3DXVECTOR4(SCREEN_WIDTH, 667.0f, 0.0f, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)}
	},

	//Line 6
	{
		{D3DXVECTOR4(0.0f, 715.0f, 0.0f, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)}, {D3DXVECTOR4(SCREEN_WIDTH, 715.0f, 0.0f, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)},
		{D3DXVECTOR4(0.0f, 720.0f, 0.0f, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)}, {D3DXVECTOR4(SCREEN_WIDTH, 720.0f, 0.0f, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255)}
	}
};

void gameInit() {
	DebugFont_Initialize();
	SystemTimer_Initialize();
	SystemTimer_Start();

	g_FrameCount = g_FPSBaseFrameCount = 0;
	g_FPSBaseTime = SystemTimer_GetTime();
	g_FPS = 0.0;

	LPDIRECT3DDEVICE9 myDevice = MyDirect3D_GetDevice();

	//Texture setting
	myDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	//可以控制polygon vertex的alpha值來讓texture變透明
	myDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	myDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	//g_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);	//可以讓texture只顯示polygon color

	//透明を設定できるセッティング
	myDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	myDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	myDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	animationInit();
}

void gameUninit() {

}

void gameUpdate() {

	g_FrameCount++;
	double time = SystemTimer_GetTime();

	if (time - g_FPSBaseTime >= FPS_MEASUREMENT_TIME) {
		g_FPS = (float)((g_FrameCount - g_FPSBaseFrameCount) / (time - g_FPSBaseTime));
		g_FPSBaseTime = time;
		g_FPSBaseFrameCount = g_FrameCount;
	}

	animationUpdate();
}

void gameDraw() {
	LPDIRECT3DDEVICE9 myDevice = MyDirect3D_GetDevice();
	
	//Draw field
	myDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, sportsField, sizeof(Vertex2d));
	for (int i = 0; i < 6; i++) {
		myDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, trackLines[i], sizeof(Vertex2d));
	}

	animationDraw();

}