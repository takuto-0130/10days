#pragma once
#include "Struct.h"
#include <Novice.h>

class Bullet {

public:
	Bullet();
	~Bullet();

	void Update(Vector2 playerPos, Vector2 playerDir, Vector2 bosPos, bool isAlive, int blockSize, MapChipNum a, const char* keys, const char* preKeys, bool clear);

	void Draw();

	bool GetIsHit() { return isHit_; };

	bool GetIsShot() { return isShot_; };

	Vector2 GetPos() { return transform_.pos; };

	float GetDir() { return direction_.x; };

public:

	Transform1 transform_;

	Vector2 direction_;

	int speed_;

	bool isShot_;

	bool isHit_;

	unsigned int color_;
};