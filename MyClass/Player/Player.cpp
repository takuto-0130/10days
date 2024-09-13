#include "Player.h"
#include "Input.h"
#include <algorithm>
#include <cmath>
#include "MyClass/GlobalVariables/GlobalVariables.h"

PlayerClass::PlayerClass(MapChipNum* map, float* scroll, Stage* stage)
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
	playerTex = Novice::LoadTexture("./Resources/stage/leg.png");
	playerTex2 = Novice::LoadTexture("./Resources/stage/body.png");
	tyouTex = Novice::LoadTexture("./Resources/stage/long_tyo.png");
	arrowTex = Novice::LoadTexture("./Resources/stage/arrow.png");

	start1 = {};
	stop1 = {};
	t1 = 0;
	kabe = false;

	direction = {};
	velocity = {};
	speed = {};

	SetStage(stage);

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	GlobalVariables::GetInstance()->LoadFiles();
	globalVariables->AddItem(groupName, "playerSpeed", player_.moveSpeed);
	globalVariables->AddItem(groupName, "playerReturn T", tIncrease);
	globalVariables->AddItem(groupName, "playerScroll T", tReturn);
	globalVariables->AddItem(groupName, "playerRefrect Distance", kMoveDistance);
	globalVariables->AddItem(groupName, "playerShot DistanceLimit", kPlayerLimitDistance);
	globalVariables->AddItem(groupName, "blockResistPower default(0.15f)", kResistPower);
	globalVariables->AddItem(groupName, "hanekaerujikann", tIncrease1);
}

PlayerClass::~PlayerClass()
{
}

void PlayerClass::Initialize()
{
	switch (*stage_) {
	case Stage::Stage1:
		kResetPos = { 22 * blockSize + blockSize / 2, 2 * blockSize + blockSize / 2 };
		break;
	case Stage::Stage2:
		kResetPos = { 22 * blockSize + blockSize / 2, 2 * blockSize + blockSize / 2 };
		break;
	case Stage::Stage3:
		kResetPos = { 22 * blockSize + blockSize / 2, 2 * blockSize + blockSize / 2 };
		break;
	case Stage::Stage4:
		kResetPos = { 6 * blockSize + blockSize / 2, 2 * blockSize + blockSize / 2 };
		break;
	case Stage::Stage5:
		kResetPos = { 22 * blockSize + blockSize / 2, 2 * blockSize + blockSize / 2 };
		break;
	case Stage::Stage6:
		kResetPos = { 26 * blockSize + blockSize / 2, 2 * blockSize + blockSize / 2 };
		break;
	case Stage::Stage7:
		kResetPos = { 36 * blockSize + blockSize / 2, 2 * blockSize + blockSize / 2 };
		break;
	case Stage::Stage8:
		kResetPos = { 15 * blockSize + blockSize / 2, 2 * blockSize + blockSize / 2 };
		break;
	case Stage::Stage9:
		kResetPos = { 22 * blockSize + blockSize / 2, 2 * blockSize + blockSize / 2 };
		break;
	case Stage::Stage10:
		kResetPos = { 22 * blockSize + blockSize / 2, 2 * blockSize + blockSize / 2 };
		break;
	case Stage::Stage11:
		kResetPos = { 21 * blockSize + blockSize / 2, 2 * blockSize + blockSize / 2 };
		break;
	case Stage::Stage12:
		kResetPos = { 38 * blockSize + blockSize / 2, 2 * blockSize + blockSize / 2 };
		break;
	}
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
	ScrollPosition(kResetPos, startPosReturn, startPosition, scroll_, 1, topPosition);
}

void PlayerClass::ApplyGlobalVariables() {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	player_.moveSpeed = globalVariables->GetVector2Value(groupName, "playerSpeed");
	tIncrease = globalVariables->GetFloatValue(groupName, "playerReturn T");
	tReturn = globalVariables->GetFloatValue(groupName, "playerScroll T");
	kMoveDistance = globalVariables->GetFloatValue(groupName, "playerRefrect Distance");
	kPlayerLimitDistance = globalVariables->GetFloatValue(groupName, "playerShot DistanceLimit");
	kResistPower = globalVariables->GetFloatValue(groupName, "blockResistPower default(0.15f)");
	tIncrease1 = globalVariables->GetFloatValue(groupName, "hanekaerujikann");
}

void PlayerClass::Update(const char* keys, const char* preKeys, XINPUT_STATE& joyState, XINPUT_STATE& beforeJpyState)
{
	ApplyGlobalVariables();

	AngleSet(keys, preKeys, joyState, beforeJpyState);

	Shooting();
	
	Operation(keys, joyState);

	ScreenScroll();

	player_.normalize = Normalize(player_.direction);

	FindVertex(player_.worldPos, player_.len.x + player_.sizeChange.x, player_.len.y + player_.sizeChange.y, &player_.lt, &player_.rt, &player_.lb, &player_.rb);

	if(!isMove)
	{
		PlayerMapCollision(*map_, player_, playerShotDir);
	}

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
	if (!isShot && !isMove && !isReturn) {
		Novice::DrawSprite(int(player_.worldPos.x - 5.0f) + int(*scroll_), int(player_.worldPos.y), arrowTex, 1.0f, 1.0f, playerShotAngle, WHITE);
	}
	Vector2 a = startPosition - player_.worldPos;
	float angle = std::atan2(-a.x, a.y);
	if (isShot) {
		Novice::DrawSpriteRect(int(player_.worldPos.x + 5.0f) + int(*scroll_), int(player_.worldPos.y) - 16, 0, 0, 10, int(Length(startPosition - player_.worldPos)), tyouTex, 1.0f, Length(startPosition - player_.worldPos) / 720.0f, angle, WHITE);
		//Novice::DrawLine(int(startPosition.x) + int(*scroll_), int(startPosition.y - 16.0f), int(player_.worldPos.x) + int(*scroll_), int(player_.worldPos.y - 16.0f), 0xFFFFFFFF);
	}
	/*Vector2 a = startPosition - player_.worldPos;
	float angle = std::atan2(a.x, -a.y);*/
	if (isMove) {
		Novice::DrawSpriteRect(int(player_.worldPos.x + 5.0f) + int(*scroll_), int(player_.worldPos.y) - 16, 0, 0, 10, int(Length(startPosition - player_.worldPos)), tyouTex, 1.0f, Length(startPosition - player_.worldPos) / 720.0f, angle, WHITE);
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

void PlayerClass::AngleSet(const char* keys, const char* preKeys, XINPUT_STATE& joyState, XINPUT_STATE& beforeJpyState)
{
	// 射出方向を決定
	if (!isShot && !isMove && !isReturn) {
		player_.direction = { 0,0 };
		player_.resistance = 0;

		kabe = false;

		

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
		else {
			if (keys[DIK_LEFT] || keys[DIK_A]) {
				playerShotAngle += kPlayerChangeAngle;
			}
			if (keys[DIK_RIGHT] || keys[DIK_D]) {
				playerShotAngle -= kPlayerChangeAngle;
			}
		}

		Matrix3x3 rotate = MakeRotateMatrix(playerShotAngle);
		viewDir = Transform(playerShotDir, rotate);
	}
	else {
		playerShotDir = { 0,2 };
	}

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A && beforeJpyState.Gamepad.wButtons ^ XINPUT_GAMEPAD_A && !isShot && !isMove && !isReturn) {
			isShot = true;
			startPosReturn = player_.worldPos;
			beforeJpyState = joyState;
			shootAngle = playerShotAngle;
		}
	}
	else {
		// 射出
		if (preKeys[DIK_SPACE] == 0 && keys[DIK_SPACE] != 0 && !isShot && !isMove && !isReturn) {
			isShot = true;
			startPosReturn = player_.worldPos;
			shootAngle = playerShotAngle;
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
			Vector2 a = startPosition - player_.worldPos;
			playerShotAngle = std::atan2(a.x, -a.y);
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
			player_.direction = { 0,0 };
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

void PlayerClass::Operation(const char* keys, XINPUT_STATE& joyState)
{
	if (!isShot && isMove) {

		RefrectMoving();


		if (!kabe) {
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
			GetSpeed(player_, speed, startPosition, kMoveChangeAngle);
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
			else {
				if (keys[DIK_LEFT] || keys[DIK_A]) {
					playerShotAngle += kMoveChangeAngle;
					LeftHitMove(player_, speed, startPosition, direction, velocity);
				}
				if (keys[DIK_RIGHT] || keys[DIK_D]) {
					playerShotAngle -= kMoveChangeAngle;
					RightHitMove(player_, speed, startPosition, direction, velocity);
				}
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
