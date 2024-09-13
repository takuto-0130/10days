#pragma once
#include <Novice.h>
#include "BrockParticle.h"
#include "Laser.h"
#include <list>
#define _USE_MATH_DEFINES
#include <math.h>
#include <memory>

class BrockEmitter{
public:
	
	~BrockEmitter();
	void Initialize();

	void Update();
	void Draw(float scroll);

	void Emit(Vector2 pos);

private:

	std::list<BrockParticle> particleList;

	std::list<Laser> laserList;

	float kBlockParticleSize = 50.0f;

	const int kParticleNum_ = 3;
	Vector2 pos_ = {};

	Vector2 blockSize_{};
};

