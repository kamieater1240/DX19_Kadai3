#pragma once
#ifndef POLYGONS_H_
#define POLYGONS_H_

#define CIRCLE_RESOLUTION 128

#include "main.h"

void fillCircle(Vertex2d circle[], float pivotX, float pivotY, float radius, D3DCOLOR color);
void fillStar(float pivotX, float pivotY);

#endif // !POLYGONS_H_
