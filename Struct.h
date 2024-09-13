#pragma once
#include "Vector2.h"
#include <stdint.h>

struct Transform1 {
	Vector2 pos;
	int radius;
};

const int mapChipWidth = 100;
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

struct Player
{
	Vector2 worldPos;
	Vector2 center;
	Vector2 len;
	Vector2 sizeChange;
	Vector2 direction;
	Vector2 moveSpeed;
	Vector2 normalize;
	Vector2 velocity;
	Vector2 tempVelo;
	float jump;
	float gravity;
	float fallSpeedMax;
	bool isAir;
	Vector2 lt;
	Vector2 rt;
	Vector2 lb;
	Vector2 rb;
	bool isAlive;
	int deadTimer;
	int clearTimer;
	float resistance;
};

struct Enemy
{
	Vector2 worldPos;
	Vector2 center;
	Vector2 len;
	Vector2 lt;
	Vector2 rt;
	Vector2 lb;
	Vector2 rb;
	bool isAlive;
	float e;
	int deadTimer;
};

const int kParticleNumber = 300;

struct Particle {
	Vector2 position[kParticleNumber];
	Vector2 velocity[kParticleNumber];
	float life[kParticleNumber];
	float radius[kParticleNumber];
	unsigned int color[kParticleNumber];
};

struct Emitter {
	Vector2 position;
	Vector2 radius;
	float life;
};

const int kDeadParticleNumber = 100;

struct ParticleDead {
	Vector2 position[kDeadParticleNumber];
	Vector2 startPos[kDeadParticleNumber];
	Vector2 moveAngle[kDeadParticleNumber];
	Vector2 velocity[kDeadParticleNumber];
	float life[kDeadParticleNumber];
	float radius[kDeadParticleNumber];
	float theta[kDeadParticleNumber];
	unsigned int color[kDeadParticleNumber];
};

struct EmitterDead {
	Vector2 position;
	float radius;
	Vector2 moveSpeed;
	float life;
	float startLen;
};



struct Timedisp {
	uint32_t seconds[2];
};

struct ScoreDisp {
	uint32_t num[4];
};

enum class Stage {
	Stage1,
	Stage2,
	Stage3,
	Stage4,
	Stage5,
	Stage6,
	Stage7,
	Stage8,
	Stage9,
	Stage10,
	Stage11,
	Stage12
};