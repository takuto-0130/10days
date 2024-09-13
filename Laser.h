#pragma once
#include <Novice.h>
#include "Vector2.h"
#include "Easing.h"

class Laser
{
public:
	~Laser();
	Laser(Vector2 pos, Vector2 blockSize);

	void Update();
	void Draw(float scroll);

	bool GetDeathFlag() { return isDeath_; }

	void QuadVer(Vector2 pos, float width, float height, Vector2& lt, Vector2& rt, Vector2& lb, Vector2& rb);

	Vector2 Lerp(const Vector2& v1, const Vector2& v2, float t);

private:

	float kBlockParticleSize = 50.0f;

	Quad laser_[2];
	Vector2 laserStartRad_[2];
	Vector2 laserStopRad_[2];

	int laserTexture_ = Novice::LoadTexture("./Resources/white1x1.png");

	const float kLaserTimeCount = 0.05f;
	float laserTime_ = 100;

	bool isDeath_ = false;
};

