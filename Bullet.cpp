#include "Bullet.h"

Bullet::Bullet() {
	transform_.pos = { 0,0 };
	transform_.radius = 5;
	direction_ = { 1,0 };
	speed_ = 8;
	isShot_ = 0;
	isHit_ = 0;
	color_ = 0xFF0000FF;
}

Bullet::~Bullet() {

}

void Bullet::Update(Vector2 playerPos, Vector2 playerDir, Vector2 boxPos, bool isAlive, int mapChipSize, MapChipNum a, const char* keys, const char* preKeys, bool clear) {
	if (playerDir.x > 0) {
		direction_.x = 1.0f;
	}
	if (playerDir.x < 0) {
		direction_.x = -1.0f;
	}
	if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == 0 && isShot_ == 0 && clear == 0) {
		transform_.pos = playerPos;
		isShot_ = 1;
	}
	if (isShot_ == 0) {
		isHit_ = 0;
	}
	if (isShot_ == 1) {
		transform_.pos.x += speed_ * direction_.x;
		if (a.mapData[static_cast <int>(transform_.pos.y / mapChipSize)][static_cast <int>(transform_.pos.x / mapChipSize)] == 1) {
			isShot_ = 0;
		}
		if (static_cast <int>(boxPos.x / mapChipSize) == static_cast <int>(transform_.pos.x / mapChipSize) &&
			static_cast <int>(boxPos.y / mapChipSize) == static_cast <int>(transform_.pos.y / mapChipSize) && isAlive == 1) {
			isHit_ = 1;
			isShot_ = 0;
		}
	}
}

void Bullet::Draw() {
	if (isShot_ == 1) {
		Novice::DrawEllipse(int(transform_.pos.x), int(transform_.pos.y), transform_.radius, transform_.radius, 0.0f, color_, kFillModeSolid);
	}
}