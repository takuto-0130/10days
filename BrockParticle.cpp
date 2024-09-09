#include "BrockParticle.h"
#define _USE_MATH_DEFINES
#include <math.h>


BrockParticle::~BrockParticle(){}

BrockParticle::BrockParticle(Vector2 pos, float rad)
{
	pos_ = pos;
	rad_ = rad;

	moveDir_ = { 0,0 };
	acc_ = { 0,0.1f };
	vel_ = { 0,0 };

	isDeath_ = true;
	timer_ = 0;

	leftTop_ = {0,0};
	rightTop_ = {0,0};
	leftBottom_ = {0,0};
	rightBottom_ = {0,0};

	color_ = 0xffffff90;
	alpha_ = 0x00000003;
}


void BrockParticle::Update()
{
	if (isDeath_)
	{
		moveDir_.x = float(rand() % 500 - 250);
		moveDir_.y = float(rand() % 500 - 250);

		normalize(moveDir_);

		color_ = 0xfffffff0;

		isDeath_ = false;
		timer_ = kParticleTime;

		vel_.y = -1.5f;
	}


	if (!isDeath_)
	{
		if (timer_ > 0)
		{
			timer_--;
		}
		else
		{
			isDeath_ = true;
			timer_ = kParticleTime;
		}


		pos_.x += moveDir_.x * 3;

		vel_.y += acc_.y;
		pos_.y += vel_.y;

	
		QuadVer(pos_, rad_, rad_, leftTop_, rightTop_, leftBottom_, rightBottom_);

		color_ -= alpha_;
	}
}

void BrockParticle::Draw(float scroll)
{

	Novice::DrawQuad((int)(leftTop_.x + scroll), (int)leftTop_.y,
		(int)(rightTop_.x + scroll), (int)rightTop_.y,
		(int)(leftBottom_.x + scroll), (int)leftBottom_.y,
		(int)(rightBottom_.x + scroll), (int)rightBottom_.y,
		0, 0, 80, 80, particleTexture_, color_);
}

void BrockParticle::normalize(Vector2& vec)
{
	float length = sqrtf(vec.x * vec.x + vec.y * vec.y);

	if (length != 0)
	{
		vec.x = vec.x / length;
		vec.y = vec.y / length;
	}
}

void BrockParticle::QuadVer(Vector2 pos, float width, float height, Vector2& lt, Vector2& rt, Vector2& lb, Vector2& rb)
{
	float halfwidth = width / 2;
	float halfheight = height / 2;

	lt = { pos.x - halfwidth, pos.y - halfheight };
	rt = { pos.x + halfwidth, pos.y - halfheight };
	lb = { pos.x - halfwidth, pos.y + halfheight };
	rb = { pos.x + halfwidth, pos.y + halfheight };
};

