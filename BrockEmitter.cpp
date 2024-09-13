#include "BrockEmitter.h"
#include <time.h>
#include <stdio.h>

BrockEmitter::~BrockEmitter(){}

void BrockEmitter::Initialize()
{
	unsigned int currenttime = (unsigned int)time(nullptr);
	srand(currenttime);

	blockSize_ = { kBlockParticleSize ,kBlockParticleSize };
}

//BrockEmitter::BrockEmitter(){}

void BrockEmitter::Update()
{
	for (auto itr = particleList.begin(); itr != particleList.end();) {
		itr->Update();

		
		if (itr->GetDeathFlag()) {
			itr = particleList.erase(itr);
		}
		else {
			itr++; 
		}
	}

	for (auto itr = laserList.begin(); itr != laserList.end();) {
		itr->Update();


		if (itr->GetDeathFlag()) {
			itr = laserList.erase(itr);
		}
		else {
			itr++;
		}
	}
}

void BrockEmitter::Draw(float scroll)
{
	for (auto& particle : particleList) {
		particle.Draw(scroll);
	}

	for (auto& laser : laserList) {
		laser.Draw(scroll);
	}
}

void BrockEmitter::Emit(Vector2 pos)
{
	for (int i = 0; i <= kParticleNum_; i++)
	{
		float size = static_cast<float>(rand() % 20 + 40) * 0.1f;
		pos_.x = float(rand() % int(kBlockParticleSize) + int(pos.x - kBlockParticleSize / 2));
		pos_.y = float(rand() % int(kBlockParticleSize) + int(pos.y - kBlockParticleSize / 2));


		BrockParticle newParticle = BrockParticle(pos_, size);
		particleList.push_back(newParticle);
	}

	Laser newLaser = Laser(pos, blockSize_);
	laserList.push_back(newLaser);
	
}