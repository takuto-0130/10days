#include "PositionSwap.h"

PositionSwap::PositionSwap() {
	playerPos_ = { 0,0 };
	boxPos_ = { 0,0 };
}

PositionSwap::~PositionSwap(){}


void PositionSwap::Update(bool isHit, Vector2 playerPos, Vector2 boxPos) {
	if (isHit == 1) {
		boxPos_ = playerPos;
		playerPos_ = boxPos;
	}
}