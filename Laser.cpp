#include "Laser.h"

Laser::~Laser(){}

Laser::Laser(Vector2 pos ,Vector2 blockSize)
{
	laserTime_ = 0.0f;

	laserStartRad_[0] = { blockSize.x / 2,200.0f };
	laserStopRad_[0] = { 0.0f,550.0f };

	//laser_[0].center = { pos.x,float(rand() % int(blockSize.y +( pos.y - blockSize.y / 2)))};
	laser_[0].center = pos;
	laser_[0].rad = laserStartRad_[0];
	QuadVer(laser_[0].center, laser_[0].rad.x, laser_[0].rad.y, laser_[0].LT, laser_[0].RT, laser_[0].LB, laser_[0].RB);



	laserStartRad_[1] = { 200.0f, blockSize.y / 2 };
	laserStopRad_[1] = { 550.0f, 0.0f };

	//laser_[1].center = { float(rand() % int(blockSize.x + (pos.x - blockSize.x / 2))),pos.y };
	laser_[1].center = pos;
	laser_[1].rad = laserStartRad_[1];
	QuadVer(laser_[1].center, laser_[1].rad.x, laser_[1].rad.y, laser_[1].LT, laser_[1].RT, laser_[1].LB, laser_[1].RB);

	isDeath_ = false;
}

void Laser::Update()
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
		isDeath_ = true;
		laserTime_ = 100.0f;
	}
}

void Laser::Draw(float scroll)
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

void Laser::QuadVer(Vector2 pos, float width, float height, Vector2& lt, Vector2& rt, Vector2& lb, Vector2& rb)
{
	float halfwidth = width / 2;
	float halfheight = height / 2;

	lt = { pos.x - halfwidth, pos.y - halfheight };
	rt = { pos.x + halfwidth, pos.y - halfheight };
	lb = { pos.x - halfwidth, pos.y + halfheight };
	rb = { pos.x + halfwidth, pos.y + halfheight };
}

Vector2 Laser::Lerp(const Vector2& v1, const Vector2& v2, float t)
{
	Vector2 v = { v2.x - v1.x,v2.y - v1.y };

	v = { v.x * t,v.y * t };

	Vector2 result = { v1.x + v.x,v1.y + v.y };

	return result;
}