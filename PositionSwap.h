#pragma once
#include "Vector2.h"


class PositionSwap {

public:
	PositionSwap();
	~PositionSwap();

	void Update(bool isHit, Vector2 playerPos, Vector2 boxPos);

	Vector2 GetSwapPlayerPos() { return playerPos_; };

	Vector2 GetSwapBoxPos() { return boxPos_; };

private:
	Vector2 playerPos_;
	Vector2 boxPos_;
};