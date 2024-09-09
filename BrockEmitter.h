#pragma once
#include <Novice.h>
#include "BrockParticle.h"
#include <list>
#define _USE_MATH_DEFINES
#include <math.h>

class BrockEmitter{
public:
	
	~BrockEmitter();
	void Initialize();

	void Update();
	void Draw(float scroll);

	void Emit(Vector2 pos);


private:

	std::list<BrockParticle> particleList;

	float kBlockParticleSize = 50.0f;

	const int kParticleNum_ = 3;
	Vector2 pos_ = {};
};

