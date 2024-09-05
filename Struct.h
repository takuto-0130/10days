#pragma once
#include "Vector2.h"

struct Transform1 {
	Vector2 pos;
	int radius;
};

const int mapChipWidth = 40;
const int mapChipHeight = 23;
const int blockSize = 32;
const int kWindowWidth = 1280;
const int kWorldEnd = mapChipWidth * blockSize;

struct MapChipNum {
	int mapData[mapChipHeight][mapChipWidth];
};

const int kWindowMapWidth = 32;
const int kWindowMapHeight = 18;

struct Vertex {
	Vector2 leftTop;
	Vector2 rightTop;
	Vector2 leftBottom;
	Vector2 rightBottom;
};

struct Matrix3x3 {
	float m[3][3];
};


struct WindowMap {
	//int num[kWindowMapHeight][kWindowMapWidth];
	Matrix3x3 affineMatrix[kWindowMapHeight][kWindowMapWidth];
	Vertex rect[kWindowMapHeight][kWindowMapWidth];
	Vertex localRect[kWindowMapHeight][kWindowMapWidth];
	Vector2 pos[kWindowMapHeight][kWindowMapWidth];
	Vector2 scale[kWindowMapHeight][kWindowMapWidth];
	bool isScaleChange[kWindowMapHeight][kWindowMapWidth];
	float scaleChange;
	const int tileSize = 40;
	int timer;
	const int kMaxTimer = 3;
	int i;
	bool isScaleChanging;
	bool isChangeStart;
};
