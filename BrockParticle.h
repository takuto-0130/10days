#pragma once
#include <Novice.h>
#include "Vector2.h"

class BrockParticle
{
public:

	~BrockParticle();
	BrockParticle(Vector2 pos,float rad);

	void Update();
	void Draw(float scroll);

	bool GetDeathFlag() { return isDeath_; }

	 
	void normalize(Vector2& vec);
	
	
	void QuadVer(Vector2 pos, float width, float height, Vector2& lt, Vector2& rt, Vector2& lb, Vector2& rb);

private:

	int particleTexture_ = Novice::LoadTexture("./NoviceResources/kamata.ico");

	Vector2 pos_ = {};
	float rad_ = 0;

	Vector2 moveDir_ = {};
	Vector2 acc_ = {};
	Vector2 vel_ = {};

	bool isDeath_ = true;
	int timer_ = 0;

	Vector2 leftTop_ = {};
	Vector2 rightTop_ = {};
	Vector2 leftBottom_ = {};
	Vector2 rightBottom_ = {};

	unsigned int color_ = 0;
	unsigned int alpha_ = 0;

	int kParticleTime = 50;
};

