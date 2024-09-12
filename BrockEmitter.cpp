#include "BrockEmitter.h"
#include <time.h>
#include <stdio.h>

BrockEmitter::~BrockEmitter(){}

void BrockEmitter::Initialize()
{
	unsigned int currenttime = (unsigned int)time(nullptr);
	srand(currenttime);

	laserTexture_ = Novice::LoadTexture("./Resources/white1x1.png");
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
	LaserUpdate();
}

void BrockEmitter::Draw(float scroll)
{
	for (auto& particle : particleList) {
		particle.Draw(scroll);
	}
	LaserDraw(scroll);
}

void BrockEmitter::Emit(Vector2 pos)
{
	
	for (int i = 0; i <= kParticleNum_; i++)
	{
		float size = static_cast<float>(rand() % 20 + 40) * 0.1f;
		pos_.x = float(rand() % int(kBlockParticleSize) + int(pos.x - kBlockParticleSize /2));
		pos_.y = float(rand() % int(kBlockParticleSize) + int(pos.y - kBlockParticleSize / 2));
		

		BrockParticle newParticle = BrockParticle(pos_, size);
		particleList.push_back(newParticle);
	}
	LaserPop(pos, {kBlockParticleSize,kBlockParticleSize});
}

void BrockEmitter::LaserPop(Vector2 pos, Vector2 blockSize)
{
	laserTime_ = 0.0f;

	laserStartRad_[0] = { blockSize.x / 2,200.0f };
	laserStopRad_[0] = { 0.0f,550.0f };

	laser_[0].center = pos;
	laser_[0].rad = laserStartRad_[0];
	QuadVer(laser_[0].center, laser_[0].rad.x, laser_[0].rad.y, laser_[0].LT, laser_[0].RT, laser_[0].LB, laser_[0].RB);



	laserStartRad_[1] = { 200.0f, blockSize.y / 2 };
	laserStopRad_[1] = { 550.0f, 0.0f };

	laser_[1].center = pos;
	laser_[1].rad = laserStartRad_[1];
	QuadVer(laser_[1].center, laser_[1].rad.x, laser_[1].rad.y, laser_[1].LT, laser_[1].RT, laser_[1].LB, laser_[1].RB);
}

void BrockEmitter::LaserUpdate()
{
	if (laserTime_ <= 1.0f)
	{
		laserTime_ += kLaserTimeCount;

		for (int i = 0; i < 2; i++)
		{
			laser_[i].rad = Lerp(laserStartRad_[i], laserStopRad_[i], laserTime_);

			QuadVer(laser_[i].center, laser_[i].rad.x, laser_[i].rad.y, laser_[i].LT, laser_[i].RT, laser_[i].LB, laser_[i].RB);
		}
	}
	else if (laserTime_ > 1.0f)
	{
		laserTime_ = 100.0f;
	}
}

void BrockEmitter::LaserDraw(float scroll)
{
	for (int i = 0; i < 2; i++)
	{
		Novice::DrawQuad(
			(int)(laser_[i].LT.x + scroll), (int)laser_[i].LT.y,
			(int)(laser_[i].RT.x + scroll), (int)laser_[i].RT.y,
			(int)(laser_[i].LB.x + scroll), (int)laser_[i].LB.y,
			(int)(laser_[i].RB.x + scroll), (int)laser_[i].RB.y,
			0, 0, 66, 66,
			laserTexture_, 0xffd700af);
	}
}

void BrockEmitter::QuadVer(Vector2 pos, float width, float height, Vector2& lt, Vector2& rt, Vector2& lb, Vector2& rb)
{
	float halfwidth = width / 2;
	float halfheight = height / 2;

	lt = { pos.x - halfwidth, pos.y - halfheight };
	rt = { pos.x + halfwidth, pos.y - halfheight };
	lb = { pos.x - halfwidth, pos.y + halfheight };
	rb = { pos.x + halfwidth, pos.y + halfheight };
}

Vector2 BrockEmitter::Lerp(const Vector2& v1, const Vector2& v2, float t)
{
	Vector2 v = { v2.x - v1.x,v2.y - v1.y };

	v = { v.x * t,v.y * t };

	Vector2 result = { v1.x + v.x,v1.y + v.y };

	return result;
}
