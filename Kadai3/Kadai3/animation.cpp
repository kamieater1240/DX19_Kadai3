#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "animation.h"
#include "main.h"
#include "sprite.h"
#include "SpriteAnim.h"
#include "texture.h"
#include "mydirect3d.h"
#include "input.h"
#include "polygons.h"

//Circle's parameter
Vertex2d circle[3][CIRCLE_RESOLUTION + 1];

//FrameCounter;
int FrameCounter;

runner Runners[6];

void animationInit() {

	SpriteAnim_Init();

	FrameCounter = 0;

	for (int i = 0; i < 6; i++) {
		Runners[i].dx = -200.0f;
		Runners[i].dy = 400.0f + 50.0f * i;
		Runners[i].faceRight = true;
		Runners[i].id = 2 * ((i % 3) + 1) - 1;
		Runners[i].playSpeed = 5;
		Runners[i].speedx = 1.0f;
		Runners[i].scaleX = 1.0f;
		Runners[i].scaleY = 1.0f;
		Runners[i].angle = 0.0f;
	}

	for (int i = 0; i < 3; i++) {
		fillCircle(circle[i], 440.0f + i * 200.0f, 100.0f, 20.0f, D3DCOLOR_RGBA(0, 0, 0, 255));
	}
}

void animationUninit() {

}

void animationUpdate() {
	FrameCounter++;

	//Runner 0
	Runners[0].dx += Runners[0].speedx;
	if (FrameCounter == 1000) {
		Runners[0].playSpeed = 1;
		Runners[0].speedx = 2.5f;
	}
	else if (FrameCounter == 1150) {
		Runners[0].playSpeed = 100;
		Runners[0].speedx = 0.1f;
	}
	else if (FrameCounter == 1400) {
		Runners[0].playSpeed = 100000;
		Runners[0].speedx = 0.0f;
	}

	//Runner 1
	if (FrameCounter >= 500) {
		if (FrameCounter == 500) {
			Runners[1].playSpeed = 3;
			Runners[1].speedx = 2.0f;
		}
		else if (FrameCounter == 1000) {
			Runners[1].playSpeed = 1;
			Runners[1].speedx = 2.5f;
		}
		else if (FrameCounter == 1150) {
			Runners[1].playSpeed = 100;
			Runners[1].speedx = 0.1f;
		}
		else if (FrameCounter == 1400) {
			Runners[1].playSpeed = 100000;
			Runners[1].speedx = 0.0f;
		}

		Runners[1].dx += Runners[1].speedx;
	}

	//Runner 2
	if (FrameCounter == 1) {
		Runners[2].speedx = 50.0f;
		Runners[2].playSpeed = 1;
	}
	if (FrameCounter > 1400) {
		if (FrameCounter == 1450) {
			Runners[2].speedx = -100.0f;
			Runners[2].scaleX = 0.25f;
			Runners[2].scaleY = 0.25f;
			Runners[2].dx = 5500.0f;
			Runners[2].dy = 1200.0f;
			Runners[2].id--;
			Runners[2].faceRight = false;
		}
		else if (FrameCounter == 1484) {
			Runners[2].speedx = 5.0f;
			Runners[2].dx = 540.0f;
			Runners[2].dy = 300.0f;
		}

		if (FrameCounter > 1484) {
			Runners[2].dy -= 5.0f;
			Runners[2].angle += 0.5f;
		}

		Runners[2].dx += Runners[2].speedx;
	}

	//Runner 3
	if (FrameCounter == 1) {
		Runners[3].speedx = 1.3f;
		Runners[3].speedy = 48.0f;
		Runners[3].playSpeed = 3;
	}
	if (FrameCounter > 100 && FrameCounter <= 1400) {
		Runners[3].dx += Runners[3].speedx;

		if (FrameCounter % 50 != 0 && FrameCounter <= 1150) {
			if ((FrameCounter / 50) % 2 == 0) {
				Runners[3].dy++;
			}
			else {
				Runners[3].dy--;
			}
		}
		else if (FrameCounter % 500 != 0 && FrameCounter > 1150) {
			if ((FrameCounter / 500) % 2 == 0) {
				Runners[3].dy -= 0.1f;
			}
			else {
				Runners[3].dy += 0.1f;
			}
		}

		if (FrameCounter == 1150) {
			Runners[3].playSpeed = 100;
			Runners[3].speedx = 0.1f;
		}
		else if (FrameCounter == 1400) {
			Runners[3].playSpeed = 100000;
			Runners[3].speedx = 0.0f;
		}
	}

	//Runner 4
	if (FrameCounter == 1) {
		Runners[4].speedx = 1.5f;
		Runners[4].speedy = 48.0f;
		Runners[4].playSpeed = 2;
	}

	if (FrameCounter > 300 && FrameCounter <= 1400) {
		Runners[4].dx += Runners[4].speedx;

		if (FrameCounter % 50 != 0 && FrameCounter <= 1150) {
			if ((FrameCounter / 50) % 2 == 0) {
				Runners[4].dy--;
			}
			else {
				Runners[4].dy++;
			}
		}
		else if (FrameCounter % 500 != 0 && FrameCounter > 1150) {
			if ((FrameCounter / 500) % 2 == 0) {
				Runners[4].dy += 0.1f;
			}
			else {
				Runners[4].dy -= 0.1f;
			}
		}

		if (FrameCounter == 1150) {
			Runners[4].playSpeed = 100;
			Runners[4].speedx = 0.1f;
		}
		else if (FrameCounter == 1400) {
			Runners[4].playSpeed = 100000;
			Runners[4].speedx = 0.0f;
		}
	}

	//Runner 5
	if (FrameCounter >= 1420) {
		if (FrameCounter == 1420) {
			Runners[5].speedx = 100.0f;
			Runners[5].scaleX = 0.25f;
			Runners[5].scaleY = 0.25f;
			Runners[5].dx = -4220.0f;
			Runners[5].dy = 1200.0f;
			Runners[5].faceRight = true;
		}
		else if (FrameCounter == 1484) {
			Runners[5].speedx = -5.0f;
			Runners[5].dx = 540.0f;
			Runners[5].dy = 300.0f;
		}

		if (FrameCounter > 1484) {
			Runners[5].dy -= 5.0f;
			Runners[5].angle -= 0.5f;
		}

		Runners[5].dx += Runners[5].speedx;
	}
}

void animationDraw() {
	LPDIRECT3DDEVICE9 myDevice = MyDirect3D_GetDevice();

	for (int i = 0; i < 6; i++) {
		if (Runners[i].faceRight) {
			if (i == 5 && FrameCounter >= 1484)
				SpriteAnim_Draw_Flip_Rotate(Runners[i].id, Runners[i].playSpeed, Runners[i].dx, Runners[i].dy, Runners[i].angle, Runners[i].scaleX, Runners[i].scaleY);
			else
				SpriteAnim_Draw_Flip(Runners[i].id, Runners[i].playSpeed, Runners[i].dx, Runners[i].dy, Runners[i].scaleX, Runners[i].scaleY);
		}
		else {
			if (i == 2 && FrameCounter >= 1484)
				SpriteAnim_Draw_Rotate(Runners[i].id, Runners[i].playSpeed, Runners[i].dx, Runners[i].dy, Runners[i].angle, Runners[i].scaleX, Runners[i].scaleY);
			else
				SpriteAnim_Draw(Runners[i].id, Runners[i].playSpeed, Runners[i].dx, Runners[i].dy, Runners[i].scaleX, Runners[i].scaleY);
		}
	}

	//Draw dot dot dot
	if (FrameCounter >= 1600) {
		if (FrameCounter >= 1600)
			myDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, CIRCLE_RESOLUTION - 1, circle[0], sizeof(Vertex2d));
		if (FrameCounter >= 1650)
			myDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, CIRCLE_RESOLUTION - 1, circle[1], sizeof(Vertex2d));
		if (FrameCounter >= 1700)
			myDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, CIRCLE_RESOLUTION - 1, circle[2], sizeof(Vertex2d));
	}
}
