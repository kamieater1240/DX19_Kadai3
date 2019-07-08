#include "sprite.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "texture.h"
#include "SpriteAnim.h"

int FrameCount = 0;

void SpriteAnim_Init() {

	Texture_SetLoadFile("Asset/Texture/runningman100.png", 700, 400);			//ID 0
	Texture_SetLoadFile("Asset/Texture/runningman100_flip.png", 700, 400);	  	//ID 1
	Texture_SetLoadFile("Asset/Texture/runningman101.png", 700, 400);		  	//ID 2
	Texture_SetLoadFile("Asset/Texture/runningman101_flip.png", 700, 400);	  	//ID 3
	Texture_SetLoadFile("Asset/Texture/runningman102.png", 700, 400);		  	//ID 4
	Texture_SetLoadFile("Asset/Texture/runningman102_flip.png", 700, 400);	  	//ID 5
	Texture_Load();
}

void SpriteAnim_Update() {
	FrameCount++;
}

void SpriteAnim_Draw(int textureID, int playSpeed, float dx, float dy) {
	//cut_x = pattern % 5 * 140		140x200
	//cut_y = pattern / 5 * 200

	int patternX = FrameCount / playSpeed % 5;
	int patternY = FrameCount / playSpeed / 5;
	Sprite_Draw(textureID, dx, dy, patternX * 140, patternY * 200, 140, 200);
}

void SpriteAnim_Draw_Flip(int textureID, int playSpeed, float dx, float dy) {
	int patternX = 4 - (FrameCount / playSpeed % 5);
	int patternY = 4 - (FrameCount / playSpeed / 5);
	Sprite_Draw(textureID, dx, dy, patternX * 140, patternY * 200, 140, 200);
}

void SpriteAnim_Uninit(){

}