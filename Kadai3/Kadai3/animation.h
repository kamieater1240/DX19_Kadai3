#pragma once
#ifndef ANIMATION_H_
#define ANIMATION_H_

void animationInit();
void animationUninit();
void animationUpdate();
void animationDraw();

typedef struct runner {
	float dx, dy;
	float speedx, speedy;
	int id;
	int playSpeed;
	bool faceRight;
	float scaleX, scaleY;
};

#endif // !ANIMATION_H_