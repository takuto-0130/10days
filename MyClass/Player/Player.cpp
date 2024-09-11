#include "Player.h"
#include "Input.h"
#include <algorithm>
#include <cmath>
#include "MyClass/GlobalVariables/GlobalVariables.h"

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
	topPosition = kResetPos;

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
	playerTex = Novice::LoadTexture("./Resources/leg.png");
	playerTex2 = Novice::LoadTexture("./Resources/body.png");

	start1 = {};
	stop1 = {};
	t1 = 0;
	kabe = false;

	direction = {};
	velocity = {};
	speed = {};

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	GlobalVariables::GetInstance()->LoadFiles();
	globalVariables->AddItem(groupName, "playerSpeed", player_.moveSpeed);
	globalVariables->AddItem(groupName, "playerReturn T", tIncrease);
	globalVariables->AddItem(groupName, "playerScroll T", tReturn);
	globalVariables->AddItem(groupName, "playerRefrect Distance", kMoveDistance);
	globalVariables->AddItem(groupName, "playerShot DistanceLimit", kPlayerLimitDistance);
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
	topPosition = kResetPos;

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

	start1 = {};
	stop1 = {};
	t1 = 0;
	kabe = false;

	direction = {};
	velocity = {};
	speed = {};
	ApplyGlobalVariables();
}

void PlayerClass::ApplyGlobalVariables() {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	player_.moveSpeed = globalVariables->GetVector2Value(groupName, "playerSpeed");
	tIncrease = globalVariables->GetFloatValue(groupName, "playerReturn T");
	tReturn = globalVariables->GetFloatValue(groupName, "playerScroll T");
	kMoveDistance = globalVariables->GetFloatValue(groupName, "playerRefrect Distance");
	kPlayerLimitDistance = globalVariables->GetFloatValue(groupName, "playerShot DistanceLimit");
}

void PlayerClass::Update(const char* keys, const char* preKeys)
{
	ApplyGlobalVariables();

	AngleSet(keys, preKeys);

	Shooting();
	
	Operation(keys);

	ScreenScroll();

	player_.normalize = Normalize(player_.direction);

	FindVertex(player_.worldPos, player_.len.x + player_.sizeChange.x, player_.len.y + player_.sizeChange.y, &player_.lt, &player_.rt, &player_.lb, &player_.rb);

	PlayerMapCollision(*map_, player_, playerShotDir);

	player_.velocity.y = player_.tempVelo.y;
	MovePlayer(player_);
	player_.center = player_.worldPos;

	FindVertex(player_.center, player_.len.x + player_.sizeChange.x, player_.len.y + player_.sizeChange.y, &player_.lt, &player_.rt, &player_.lb, &player_.rb);


//#ifdef _DEBUG
//	ImGui::Begin("window");
//	ImGui::Text("%f", playerShotAngle);
//	ImGui::InputFloat2("return", &startPosReturn.x);
//	ImGui::InputFloat2("startpos", &startPosition.x);
//	ImGui::End();
//#endif // _DEBUG
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
		int(topPosition.x - (player_.len.x + player_.sizeChange.x) / 2) + int(*scroll_), int(topPosition.y - (player_.len.y + player_.sizeChange.y) / 2 - blockSize),
		int(topPosition.x + (player_.len.x + player_.sizeChange.x) / 2) + int(*scroll_), int(topPosition.y - (player_.len.y + player_.sizeChange.y) / 2 - blockSize),
		int(topPosition.x - (player_.len.x + player_.sizeChange.x) / 2) + int(*scroll_), int(topPosition.y + (player_.len.y + player_.sizeChange.y) / 2 - blockSize),
		int(topPosition.x + (player_.len.x + player_.sizeChange.x) / 2) + int(*scroll_), int(topPosition.y + (player_.len.y + player_.sizeChange.y) / 2 - blockSize),
		0, 0, blockSize, blockSize, playerTex2, 0xFFFFFFFF);
	Novice::DrawQuad(
		int(player_.center.x - (player_.len.x + player_.sizeChange.x) / 2) + int(*scroll_), int(player_.center.y - (player_.len.y + player_.sizeChange.y) / 2),
		int(player_.center.x + (player_.len.x + player_.sizeChange.x) / 2) + int(*scroll_), int(player_.center.y - (player_.len.y + player_.sizeChange.y) / 2),
		int(player_.center.x - (player_.len.x + player_.sizeChange.x) / 2) + int(*scroll_), int(player_.center.y + (player_.len.y + player_.sizeChange.y) / 2),
		int(player_.center.x + (player_.len.x + player_.sizeChange.x) / 2) + int(*scroll_), int(player_.center.y + (player_.len.y + player_.sizeChange.y) / 2),
		0, 0, blockSize, blockSize, playerTex, 0xFFFFFFFF);
}

void PlayerClass::AngleSet(const char* keys, const char* preKeys)
{
	XINPUT_STATE joyState;
	// 射出方向を決定
	if (!isShot && !isMove && !isReturn) {
		player_.direction = { 0,0 };
		player_.resistance = 0;

		kabe = false;

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

void PlayerClass::RefrectShooting() {

	TestBlockLeftHit(player_, *map_, start1, stop1, kabe, kMoveDistance, kHitMisalignment);

	TestBlockRightHit(player_, *map_, start1, stop1, kabe, kMoveDistance, kHitMisalignment);

	TestBlocUnderHit(player_, *map_, start1, stop1, kabe, kMoveDistance);

	if (kabe)
	{

		TestBlockLerp(player_, start1, stop1, t1);

		if (t1 < 1.0f)
		{
			t1 += tIncrease1;
		}
		else {

			isShot = false;
			kabe = false;
			t1 = 0;
			t = 0;
			isMove = true;

			stopPosition = player_.worldPos;

			playerShotAngle = 0;
			tReturnNow = 0;
		}
	}

}

void PlayerClass::RefrectMoving()
{
	MoveBlocUnderHit(player_, *map_, start1, stop1, kabe, kMoveDistance, velocity);

	MoveBlockLeftHit(player_, *map_, start1, stop1, kabe, kMoveDistance, velocity);

	MoveBlockRightHit(player_, *map_, start1, stop1, kabe, kMoveDistance, velocity);
	
	if (kabe)
	{

		TestBlockLerp(player_, start1, stop1, t1);

		if (t1 <= 1.0f)
		{
			t1 += tIncrease1;
		}
		else {
			kabe = false;
			t1 = 0;

			stopPosition = player_.worldPos;

			playerShotAngle = 0;
			tReturnNow = 0;
		}
	}
}

void PlayerClass::Shooting()
{
	if (isShot) {
		Matrix3x3 rotate = MakeRotateMatrix(playerShotAngle);
		if (playerShotDir.y - player_.resistance > kResistPower * 2.0f && Length(startPosition - player_.worldPos) < kPlayerLimitDistance) {
			player_.direction = Transform({ playerShotDir.x, playerShotDir.y - player_.resistance }, rotate);
		}
		else {
			isShot = false;
			isMove = true;
			stopPosition = player_.worldPos;
			playerShotAngle = 0;
			t = 0;
			tReturnNow = 0;

			kabe = false;
		}
		RefrectShooting();
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

			kabe = false;
		}

		RefrectMoving();


		if (!kabe) {
			GetSpeed(player_, speed, startPosition, kMoveChangeAngle);
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
				if (playerShotAngle < angle) {
					playerShotAngle += kMoveChangeAngle;
					LeftHitMove(player_, speed, startPosition, direction, velocity);
				}
				if (playerShotAngle > angle) {
					playerShotAngle -= kMoveChangeAngle;
					RightHitMove(player_, speed, startPosition, direction, velocity);
				}
			}


			if (keys[DIK_LEFT] || keys[DIK_A]) {
				playerShotAngle += kMoveChangeAngle;
				LeftHitMove(player_, speed, startPosition, direction, velocity);
			}
			if (keys[DIK_RIGHT] || keys[DIK_D]) {
				playerShotAngle -= kMoveChangeAngle;
				RightHitMove(player_, speed, startPosition, direction, velocity);
			}
			Matrix3x3 rotate = MakeRotateMatrix(playerShotAngle);
			Vector2 vec = Transform(stopPosition - startPosition, rotate);


			Vector2 stopPositionNow = vec + startPosition;
			ReturnPosition(player_, startPosition, stopPositionNow, *map_, t);
		}
	}
#ifdef _DEBUG
	float a = Length(startPosition - player_.worldPos);
	ImGui::Begin("window");
	ImGui::Text("%d", isMove);
	ImGui::Text("%f", t);
	ImGui::InputFloat2("return", &startPosReturn.x);
	ImGui::InputFloat2("startpos", &startPosition.x);
	ImGui::InputFloat("speed", &speed);
	ImGui::InputFloat("a", &a);

	ImGui::Checkbox("kabe", &kabe);
	ImGui::Checkbox("isShot", &isShot);
	ImGui::Text("%f", t1);
	ImGui::End();
#endif // _DEBUG
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
		ScrollPosition(kResetPos, startPosReturn, startPosition, scroll_, tReturnNow, topPosition);
	}
}
