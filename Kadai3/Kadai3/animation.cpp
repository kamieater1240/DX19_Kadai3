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

//Texture IDs
int Texture_IDs[100];
int textureNum;

//FrameCounter;
int FrameCounter;

runner Runners[5];

void animationInit() {

	SpriteAnim_Init();
	Texture_Load();

	FrameCounter = 0;

	for (int i = 0; i < 5; i++) {
		Runners[i].dx = 100.0f;
		Runners[i].dy = 404.0f + 48.0f * i;
		Runners[i].faceRight = true;
		Runners[i].playSpeed = 5;
		Runners[i].speed = 10.0f;
	}
}

void animationUninit() {

}

void animationUpdate() {
	FrameCounter++;

}

void animationDraw() {

	//SpriteAnim_Draw(0, 10, 900, 500);
	for (int i = 0; i < 5; i++) {
		if (i % 2 == 0)
			SpriteAnim_Draw_Flip(i + 1, 5, Runners[i].dx, Runners[i].dy);
		else
			SpriteAnim_Draw_Flip(i, 5, Runners[i].dx, Runners[i].dy);
	}

}
