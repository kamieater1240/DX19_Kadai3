#pragma once
#ifndef ANIMATION_H_
#define ANIMATION_H_

void animationInit();
void animationUninit();
void animationUpdate();
void animationDraw();

typedef struct runner {
	float dx, dy;
	float speed;
	int playSpeed;
	bool faceRight;
};

#endif // !ANIMATION_H_