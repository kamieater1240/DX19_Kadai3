#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "polygons.h"

void fillCircle(Vertex2d circle[], float pivotX, float pivotY, float radius, D3DCOLOR color) {

	for (int i = 0; i < CIRCLE_RESOLUTION + 1; i++) {
		circle[i].position.x = pivotX + radius * cos(D3DX_PI*(i / (CIRCLE_RESOLUTION / 2.0f)));
		circle[i].position.y = pivotY + radius * sin(D3DX_PI*(i / (CIRCLE_RESOLUTION / 2.0f)));
		circle[i].position.z = 0.0f;
		circle[i].position.w = 1.0f;
		circle[i].color = color;
	}
}

void fillStar(float pivotX, float pivotY) {
	//Vertex2d tmp;

	//star[0] = { D3DXVECTOR4(0.0f + pivotX, -tan(D3DX_PI * 3.0f / 10.0f) * 10.0f + pivotY, 0.0f, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255) };

	//for (int i = 1; i < 12; i++) {
	//	if (i == 1) {
	//		star[i].position.x = 0.0f + pivotX;
	//		star[i].position.y = tan(D3DX_PI / 3.0f) * 10.0f + pivotY;

	//	}
	//	else if (i == 2) {
	//		star[i].position.x = -1.0f * 10.0f + pivotX;
	//		star[i].position.y = 0.0f + pivotY;
	//	}
	//	else {
	//		//平移到原點
	//		tmp.position.x = star[i - 2].position.x - star[0].position.x;
	//		tmp.position.y = star[i - 2].position.y - star[0].position.y;

	//		//計算後的位置
	//		star[i].position.x = tmp.position.x * cos(D3DX_PI * 2.0f / 5.0f) - tmp.position.y * sin(D3DX_PI * 2.0f / 5.0f);
	//		star[i].position.y = tmp.position.x * sin(D3DX_PI * 2.0f / 5.0f) + tmp.position.y * cos(D3DX_PI * 2.0f / 5.0f);

	//		//平移回原本的軸心
	//		star[i].position.x = star[i].position.x + star[0].position.x;
	//		star[i].position.y = star[i].position.y + star[0].position.y;
	//	}

	//	star[i].position.z = 0.0f;
	//	star[i].position.w = 1.0f;
	//	star[i].color = D3DCOLOR_RGBA(255, 255, 0, 255);
	//	star[i].uv = D3DXVECTOR2(0.0f, 0.0f);
	//}
}