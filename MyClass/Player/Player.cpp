#include "Player.h"
#include "Input.h"
#include <algorithm>
#include <cmath>

PlayerClass::PlayerClass(MapChipNum* map, float* scroll)
{
	player_.worldPos = kResetPos;
	player_.center = player_.worldPos;
	player_.len = { blockSize,blockSize };
	player_.sizeChange = { 0,0 };
	player_.direction = { 0,0 };
	player_.moveSpeed = { 7,7 };
	player_.normalize = { 0,0 };
	player_.isAir = 0;
	player_.velocity = { 0,0 };
	player_.tempVelo = { 0,0 };
	player_.gravity = 0.0f;
	player_.fallSpeedMax = 12;
	player_.jump = -8;
	player_.isAlive = 1;
	FindVertex(player_.center, player_.len.x, player_.len.y, &player_.lt, &player_.rt, &player_.lb, &player_.rb);
	startPosition = kResetPos;
	startPosReturn = kResetPos;
	stopPosition = kResetPos;

	playerShotAngle = 0;

	playerShotDir = { 0,2 };
	viewDir = playerShotDir;

	isShot = false;
	isMove = false;
	isReturn = false;

	t = 0;
	tIncrease = 0.01f;

	tReturnNow = 0;
	tReturn = 0.02f;
	SetMap(map);
	SetScroll(scroll);
	playerTex = Novice::LoadTexture("./Resources/playerRect.png");
}

PlayerClass::~PlayerClass()
{
}

void PlayerClass::Initialize()
{
	player_.worldPos = kResetPos;
	player_.center = player_.worldPos;
	player_.len = { blockSize,blockSize };
	player_.sizeChange = { 0,0 };
	player_.direction = { 0,0 };
	player_.moveSpeed = { 7,7 };
	player_.normalize = { 0,0 };
	player_.isAir = 0;
	player_.velocity = { 0,0 };
	player_.tempVelo = { 0,0 };
	player_.gravity = 0.0f;
	player_.fallSpeedMax = 12;
	player_.jump = -8;
	player_.isAlive = 1;
	FindVertex(player_.center, player_.len.x, player_.len.y, &player_.lt, &player_.rt, &player_.lb, &player_.rb);
	startPosition = kResetPos;
	startPosReturn = kResetPos;
	stopPosition = kResetPos;

	playerShotAngle = 0;

	playerShotDir = { 0,2 };
	viewDir = playerShotDir;

	isShot = false;
	isMove = false;
	isReturn = false;

	t = 0;
	tIncrease = 0.01f;

	tReturnNow = 0;
	tReturn = 0.02f;

	*scroll_ = 0;
}

void PlayerClass::Update(const char* keys, const char* preKeys)
{
	AngleSet(keys, preKeys);

	Shooting();
	
	Operation(keys);

	ScreenScroll();

	player_.normalize = Normalize(player_.direction);

	FindVertex(player_.worldPos, player_.len.x + player_.sizeChange.x, player_.len.y + player_.sizeChange.y, &player_.lt, &player_.rt, &player_.lb, &player_.rb);

	PlayerMapCollision(*map_, player_);

	player_.velocity.y = player_.tempVelo.y;
	MovePlayer(player_);
	player_.center = player_.worldPos;

	FindVertex(player_.center, player_.len.x + player_.sizeChange.x, player_.len.y + player_.sizeChange.y, &player_.lt, &player_.rt, &player_.lb, &player_.rb);


#ifdef _DEBUG
	ImGui::Begin("window");
	ImGui::Text("%f", playerShotAngle);
	ImGui::InputFloat2("return", &startPosReturn.x);
	ImGui::InputFloat2("startpos", &startPosition.x);
	ImGui::End();
#endif // _DEBUG
#ifdef _DEBUG
	/*ImGui::Begin("window");
	ImGui::Text("%f, %f", vec.x, vec.y);
	ImGui::End();*/
#endif // _DEBUG
}

void PlayerClass::Draw()
{
	if (!isShot && !isMove) {
		Novice::DrawLine(int(player_.worldPos.x) + int(*scroll_), int(player_.worldPos.y), int(player_.worldPos.x + (viewDir.x * 40.0f)) + int(*scroll_), int(player_.worldPos.y + (viewDir.y * 40.0f)), 0xFFFFFFFF);
	}
	if (isMove) {
		Novice::DrawLine(int(startPosition.x) + int(*scroll_), int(startPosition.y), int(player_.worldPos.x) + int(*scroll_), int(player_.worldPos.y), 0xFFFFFFFF);
	}
	Novice::DrawQuad(
		int(player_.center.x - (player_.len.x + player_.sizeChange.x) / 2) + int(*scroll_), int(player_.center.y - (player_.len.y + player_.sizeChange.y) / 2),
		int(player_.center.x + (player_.len.x + player_.sizeChange.x) / 2) + int(*scroll_), int(player_.center.y - (player_.len.y + player_.sizeChange.y) / 2),
		int(player_.center.x - (player_.len.x + player_.sizeChange.x) / 2) + int(*scroll_), int(player_.center.y + (player_.len.y + player_.sizeChange.y) / 2),
		int(player_.center.x + (player_.len.x + player_.sizeChange.x) / 2) + int(*scroll_), int(player_.center.y + (player_.len.y + player_.sizeChange.y) / 2),
		32, 0, blockSize, blockSize, playerTex, 0xFFFFFFFF);
}

void PlayerClass::AngleSet(const char* keys, const char* preKeys)
{
	XINPUT_STATE joyState;
	// 射出方向を決定
	if (!isShot && !isMove && !isReturn) {
		player_.direction = { 0,0 };
		player_.resistance = 0;

		if (keys[DIK_LEFT] || keys[DIK_A]) {
			playerShotAngle += kPlayerChangeAngle;
		}
		if (keys[DIK_RIGHT] || keys[DIK_D]) {
			playerShotAngle -= kPlayerChangeAngle;
		}

		Vector2 move{ 0, 0 };
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			const float threshold = 0.7f;
			bool isMoving = false;
			move = { float(joyState.Gamepad.sThumbLX) / SHRT_MAX, float(joyState.Gamepad.sThumbLY) / SHRT_MAX };
			if (Length(move) > threshold) {
				isMoving = true;
			}
			if (isMoving) {
				move.x += float(joyState.Gamepad.sThumbLX) / SHRT_MAX;
				move.y += float(joyState.Gamepad.sThumbLY) / SHRT_MAX;
				//move = Normalize(move);
				// Y軸周り角度(θy)
				playerShotAngle = std::atan2(-move.x, -move.y);
			}
		}

		Matrix3x3 rotate = MakeRotateMatrix(playerShotAngle);
		viewDir = Transform(playerShotDir, rotate);
	}
	else {
		playerShotDir = { 0,2 };
	}

	// 射出
	if (preKeys[DIK_SPACE] == 0 && keys[DIK_SPACE] != 0 && !isShot && !isMove && !isReturn) {
		isShot = true;
		startPosReturn = player_.worldPos;
	}
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A && !isShot && !isMove && !isReturn) {
			isShot = true;
			startPosReturn = player_.worldPos;
		}
	}
}

void PlayerClass::Shooting()
{
	if (isShot) {
		Matrix3x3 rotate = MakeRotateMatrix(playerShotAngle);
		if (playerShotDir.y - player_.resistance > 0) {
			player_.direction = Transform({ playerShotDir.x, playerShotDir.y - player_.resistance }, rotate);
		}
		else {
			isShot = false;
			isMove = true;
			stopPosition = player_.worldPos;
			playerShotAngle = 0;
			t = 0;
			tReturnNow = 0;
		}
	}
}

void PlayerClass::Operation(const char* keys)
{
	if (!isShot && isMove) {
		if (t <= 1.0f) {
			t += tIncrease;
		}
		else {
			isMove = false;
			isReturn = true;
			playerShotAngle = 0;
			t = 0;
		}

		XINPUT_STATE joyState;
		Vector2 move{ 0, 0 };
		float angle = playerShotAngle;
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			const float threshold = 0.7f;
			bool isMoving = false;
			move = { float(joyState.Gamepad.sThumbLX) / SHRT_MAX, float(joyState.Gamepad.sThumbLY) / SHRT_MAX };
			if (Length(move) > threshold) {
				isMoving = true;
			}
			if (isMoving) {
				move.x += float(joyState.Gamepad.sThumbLX) / SHRT_MAX;
				move.y += float(joyState.Gamepad.sThumbLY) / SHRT_MAX;
				//move = Normalize(move);
				// Y軸周り角度(θy)
				angle = std::atan2(-move.x, -move.y);
			}
		}


		if (keys[DIK_LEFT] || keys[DIK_A] || playerShotAngle < angle) {
			playerShotAngle += kMoveChangeAngle;
		}
		if (keys[DIK_RIGHT] || keys[DIK_D] || playerShotAngle > angle) {
			playerShotAngle -= kMoveChangeAngle;
		}
		Matrix3x3 rotate = MakeRotateMatrix(playerShotAngle);
		Vector2 vec = Transform(stopPosition - startPosition, rotate);


		Vector2 stopPositionNow = vec + startPosition;
		ReturnPosition(player_, startPosition, stopPositionNow, *map_, t);
	}
}

void PlayerClass::ScreenScroll()
{
	if (isReturn) {
		player_.direction = { 0,0 };
		player_.resistance = 0;
		if (tReturnNow <= 1.0f) {
			tReturnNow += tReturn;
		}
		else {
			isReturn = false;
		}
		ScrollPosition(kResetPos, startPosReturn, startPosition, scroll_, tReturnNow);
	}
}
