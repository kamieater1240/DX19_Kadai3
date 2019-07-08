#pragma once
#ifndef MAIN_H_
#define MAIN_H_

#define SCREEN_WIDTH	1280
#define SCREEN_HEIGHT	720
//position       color            texture
#define FVF_VERTEX2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

#define FPS_MEASUREMENT_TIME 1.0

typedef struct Vertex2d_tag {
	// x, y, z, w			���W�ϊ��ςݒ��_�̗��p�@=> RHW��1.0f��ݒ�
	D3DXVECTOR4 position;	//���W, include in d3dx9.h
	//Color
	D3DCOLOR color;
	//uv
	D3DXVECTOR2 uv;			//texture coordinate
}Vertex2d;

static bool Init(HWND hWnd);
static void Uninit(void);
static void Update(void);
static void Draw(void);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);



#endif // !MAIN_H_