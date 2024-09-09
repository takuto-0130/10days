#include "Function.h"
#include <memory>
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG


const char kWindowTitle[] = "10daysTest";

void ReturnPosition(Player& player, const Vector2& startPos, const MapChipNum map) {
	player.direction = { 0,0 };
	if (map.mapData[int(player.lt.y - 1.0f) / blockSize][int(player.lt.x) / blockSize] != 1 &&
		map.mapData[int(player.rt.y - 1.0f) / blockSize][int(player.rt.x) / blockSize] != 1)
	{
		player.direction = Normalize(player.worldPos - startPos);
		if (player.worldPos.x - startPos.x < 0)
		{
			player.direction = { player.direction.x * 0.5f, player.direction.y * -0.5f };
		}
		else {
			player.direction = { player.direction.x * -0.5f, player.direction.y * -0.5f };
		}
	}
	else {
		player.direction = { 0,0 };
	}
}
void ReturnPosition(Player& player, Vector2& startPos, Vector2& stopPos, const MapChipNum map, float& t) {
	map;
	if (map.mapData[int(player.lt.y - 3.0f) / blockSize][int(player.lt.x) / blockSize] != 1 &&
		map.mapData[int(player.rt.y - 3.0f) / blockSize][int(player.rt.x) / blockSize] != 1)
	{
		player.worldPos = Lerp(stopPos, startPos, t * t * t);
	}
	else {
		player.worldPos.y = startPos.y;
		t = 1.01f;
	}
	if (t >= 1.0f) {
		startPos = player.worldPos;
		//t = 0;
	}
}


void ScrollPosition(const Vector2& kResetPos, const Vector2& returnPos, const Vector2& startPos, float& scroll, const float& t) {
	Vector2 pos = Lerp(returnPos, startPos, t);
	if (kResetPos.x < pos.x) {
		scroll = kResetPos.x - pos.x;
	}
	else {
		scroll = 0;
	}
}


void Refrect(Player& player, const MapChipNum map) {
	if (map.mapData[int(player.rt.y) / blockSize][int(player.rt.x + (player.moveSpeed.x * player.direction.x)) / blockSize] == 1 ||
		map.mapData[int(player.rb.y) / blockSize][int(player.rb.x + (player.moveSpeed.x * player.direction.x)) / blockSize] == 1) {
		player.worldPos.x = float(int(player.rt.x + (player.moveSpeed.x * player.direction.x)) / blockSize) * blockSize - (player.len.x + player.sizeChange.x) / 2;
		player.direction.x = -2.0f;
	}
	if (map.mapData[int(player.lt.y) / blockSize][int(player.lt.x + (player.moveSpeed.x * player.direction.x)) / blockSize] == 1 ||
		map.mapData[int(player.lb.y) / blockSize][int(player.lb.x + (player.moveSpeed.x * player.direction.x)) / blockSize] == 1) {
		player.worldPos.x = float(int(player.lb.x + (player.moveSpeed.x * player.direction.x)) / blockSize) * blockSize + (player.len.x + player.sizeChange.x) / 2 + blockSize;
		player.direction.x = 2.0f;
	}
}

// キー入力結果を受け取る箱
char keys[256] = { 0 };
char preKeys[256] = { 0 };

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, 720);

	//float gravity = 0.02f;

	bool isParticle = 0;

	int mapTex = Novice::LoadTexture("./Resources/mapBlock.png");
	int playerTex = Novice::LoadTexture("./Resources/playerRect.png");
	int bgTex = Novice::LoadTexture("./Resources/bg.png");
	int block = Novice::LoadTexture("white1x1.png");
	int titleTex = Novice::LoadTexture("./Resources/title.png");

	int numberTex = Novice::LoadTexture("./Resources/number.png");
	int timeFrame = Novice::LoadTexture("./Resources/timeFrame.png");
	int scoreFrame = Novice::LoadTexture("./Resources/scoreFrame.png");


	MapChipNum map{};

	LoadMap(map, "./Resources/testMapLarge.csv");

	MapChipNum startMap = map;

	const Vector2 kResetPos = { 19 * blockSize + blockSize / 2, 1 * blockSize + blockSize / 2 };
	Vector2 startPosition = kResetPos;
	Vector2 startPosReturn = kResetPos;
	Vector2 stopPosition = kResetPos;
	Player player;
	player.worldPos = kResetPos;
	player.center = player.worldPos;
	player.len = { blockSize,blockSize };
	player.sizeChange = { 0,0 };
	player.direction = { 0,0 };
	player.moveSpeed = { 7,7 };
	player.normalize = { 0,0 };
	player.isAir = 0;
	player.velocity = { 0,0 };
	player.tempVelo = { 0,0 };
	player.gravity = 0.0f;
	player.fallSpeedMax = 12;
	player.jump = -8;
	player.isAlive = 1;
	FindVertex(player.center, player.len.x, player.len.y, &player.lt, &player.rt, &player.lb, &player.rb);

	Player oldPlayer1 = player;

	float playerShotAngle = 0;
	const float kPlayerChangeAngle = float(M_PI) / 90;
	Vector2 playerShotDir = { 0,2 };
	Vector2 viewDir = playerShotDir;
	const float resistPower = 0.15f;


	enum Scene {
		TITLE,
		GAME
	};

	int scene = GAME;
	//bool isScaleDown = 0;

	bool isClear = 0;

	bool isShot = false;
	bool isMove = false;
	bool isReturn = false;

	//プレイヤーが戻る時間
	float t = 0;
	float tIncrease = 0.01f;

	float tReturnNow = 0;
	float tReturn = 0.02f;

	int kHitStopFrame = 5;
	int hitStopTimer = 0;
	const float kMoveChangeAngle = float(M_PI) / 90;

	float scroll = 0;

	std::unique_ptr<BrockEmitter> blockParticle = nullptr;
	blockParticle.reset(new BrockEmitter());
	blockParticle->Initialize();

	const uint32_t kPlayTime = 5999;
	uint32_t playTimer = kPlayTime;
	Timedisp timeDisplay{};

	uint32_t breakCount = 0;
	ScoreDisp score{};

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

#ifdef _DEBUG
				ImGui::Begin("window");
				ImGui::DragFloat("scroll", &scroll, 1.0f);
				ImGui::End();
#endif // _DEBUG

		switch (scene) {
		case TITLE:
			if (preKeys[DIK_RETURN] == 0 && keys[DIK_RETURN] != 0) {
				scene = GAME;
			}
			break;
		case GAME:
			isParticle = 0;
			isClear = 0;

			oldPlayer1.worldPos = player.worldPos;
			oldPlayer1.center = player.center;
			// HitStop

			if (playTimer > 0) {
				playTimer--;
			}
			TimeDisplay(playTimer, timeDisplay);


			if (playTimer > 0)
			{
				if (hitStopTimer <= 0) {
					if (!isShot && !isMove && !isReturn) {
						player.direction = { 0,0 };
						player.resistance = 0;
						/*if (keys[DIK_A]) {
							player.direction.x--;
						}
						if (keys[DIK_D]) {
							player.direction.x++;
						}*/

						/*if (keys[DIK_W] || keys[DIK_UP]) {
							player.direction.y--;
						}
						if (keys[DIK_S] || keys[DIK_DOWN]) {
							player.direction.y++;
						}*/

						if (keys[DIK_LEFT] || keys[DIK_A]) {
							playerShotAngle += kPlayerChangeAngle;
						}
						if (keys[DIK_RIGHT] || keys[DIK_D]) {
							playerShotAngle -= kPlayerChangeAngle;
						}
						Matrix3x3 rotate = MakeRotateMatrix(playerShotAngle);
						viewDir = Transform(playerShotDir, rotate);
					}
					else {
						playerShotDir = { 0,2 };
					}

					if (preKeys[DIK_SPACE] == 0 && keys[DIK_SPACE] != 0 && !isShot && !isMove && !isReturn) {
						isShot = true;
						startPosReturn = player.worldPos;
					}
					if (isShot) {
						Matrix3x3 rotate = MakeRotateMatrix(playerShotAngle);
						if (playerShotDir.y - player.resistance > 0) {
							player.direction = Transform({ playerShotDir.x, playerShotDir.y - player.resistance }, rotate);
						}
						else {
							isShot = false;
							isMove = true;
							stopPosition = player.worldPos;
							playerShotAngle = 0;
							t = 0;
							tReturnNow = 0;
						}
					}
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
						if (keys[DIK_LEFT] || keys[DIK_A]) {
							playerShotAngle += kMoveChangeAngle;
						}
						if (keys[DIK_RIGHT] || keys[DIK_D]) {
							playerShotAngle -= kMoveChangeAngle;
						}
						Matrix3x3 rotate = MakeRotateMatrix(playerShotAngle);
						Vector2 vec = Transform(stopPosition - startPosition, rotate);
#ifdef _DEBUG
						/*ImGui::Begin("window");
						ImGui::Text("%f, %f", vec.x, vec.y);
						ImGui::End();*/
#endif // _DEBUG

						Refrect(player, map);

						Vector2 stopPositionNow = vec + startPosition;
						ReturnPosition(player, startPosition, stopPositionNow, map, t);
					}

					if (isReturn) {
						player.direction = { 0,0 };
						player.resistance = 0;
						if (tReturnNow <= 1.0f) {
							tReturnNow += tReturn;
						}
						else {
							isReturn = false;
						}
						ScrollPosition(kResetPos, startPosReturn, startPosition, scroll, tReturnNow);
					}

					player.normalize = Normalize(player.direction);

					FindVertex(player.worldPos, player.len.x + player.sizeChange.x, player.len.y + player.sizeChange.y, &player.lt, &player.rt, &player.lb, &player.rb);
					FindVertex(oldPlayer1.worldPos, oldPlayer1.len.x + oldPlayer1.sizeChange.x, oldPlayer1.len.y + oldPlayer1.sizeChange.y, &oldPlayer1.lt, &oldPlayer1.rt, &oldPlayer1.lb, &oldPlayer1.rb);
					CollisionBlock(map, player, resistPower, kHitStopFrame, hitStopTimer, blockParticle.get(), breakCount);
					ScoreDisplay(breakCount, score);
					///////////////////////
					PlayerMapCollision(map, player);

					player.velocity.y = player.tempVelo.y;
					MovePlayer(player);
				}
				if (hitStopTimer >= 0) {
					hitStopTimer--;
				}
				blockParticle->Update();

				/*if (kResetPos.x < startPosition.x) {
					scroll = kResetPos.x - startPosition.x;
				}
				else {
					scroll = 0;
				}*/

#ifdef _DEBUG
				ImGui::Begin("window");
				ImGui::Text("%d", hitStopTimer);
				ImGui::Text("%d", isMove);
				ImGui::Text("%f", t);
				ImGui::InputFloat2("return", &startPosReturn.x);
				ImGui::InputFloat2("startpos", &startPosition.x);
				ImGui::End();
#endif // _DEBUG
				player.center = player.worldPos;

				FindVertex(player.center, player.len.x + player.sizeChange.x, player.len.y + player.sizeChange.y, &player.lt, &player.rt, &player.lb, &player.rb);

			}



			if (isClear == 1) {
				player.clearTimer++;
				if (player.clearTimer > 125) {
					if (keys[DIK_RETURN]) {
						scene = TITLE;
					}
				}
			}
			if (player.clearTimer % 2 == 0 && isClear == 1) {

			}
			break;
		}

		for (int i = 0; i < kDeadParticleNumber; i++)
		{
		}

		if (preKeys[DIK_R] == 0 && keys[DIK_R] != 0 || scene == TITLE)
		{
			player.clearTimer = 0;
			player.deadTimer = 0;
			isClear = 0;
			player.worldPos = kResetPos;
			startPosition = kResetPos;
			stopPosition = kResetPos;
			player.center = player.worldPos;
			player.direction = { 1,0 };
			player.normalize = { 0,0 };
			player.isAir = 0;
			player.velocity = { 0,0 };
			player.tempVelo = { 0,0 };
			player.isAlive = 1;
			isShot = false;
			isMove = false;
			isReturn = false;
			playerShotAngle = 0;
			FindVertex(player.center, player.len.x, player.len.y, &player.lt, &player.rt, &player.lb, &player.rb);
			map = startMap;
			playTimer = kPlayTime;
			breakCount = 0;
			t = 0;
			tReturnNow = 0;
			scroll = 0;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///


		switch (scene) {
		case TITLE:
			Novice::DrawSprite(0, 0, titleTex, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
			break;
		case GAME:
			Novice::DrawSprite(0/* + int(scroll)*/, 0, bgTex, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);

			for (int i = 0; i < mapChipHeight; i++)
			{
				for (int j = 0; j < mapChipWidth; j++)
				{
					//if(j * blockSize)
					if (map.mapData[i][j] == 1)
					{
						Novice::DrawSpriteRect(int(blockSize * j) + int(scroll), blockSize * i, 0, 0, blockSize, blockSize, mapTex, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
					}
					if (map.mapData[i][j] == 3)
					{
						Novice::DrawSpriteRect(int(blockSize * j) + int(scroll), blockSize * i, 0, 0, blockSize, blockSize, mapTex, 1.0f, 1.0f, 0.0f, 0xAAAAFFFF);
					}
					if (map.mapData[i][j] == 2)
					{
						Novice::DrawSpriteRect(int(blockSize * j) + int(scroll), blockSize * i, 0, 0, 1, 1, block, 32.0f, 32.0f, 0.0f, 0x55AA55FF);
					}
				}
			}

			if (!isShot && !isMove) {
				Novice::DrawLine(int(player.worldPos.x) + int(scroll), int(player.worldPos.y), int(player.worldPos.x + (viewDir.x * 40.0f)) + int(scroll), int(player.worldPos.y + (viewDir.y * 40.0f)), 0xFFFFFFFF);
			}
			if (isMove) {
				Novice::DrawLine(int(startPosition.x) + int(scroll), int(startPosition.y), int(player.worldPos.x) + int(scroll), int(player.worldPos.y), 0xFFFFFFFF);
			}
			blockParticle->Draw(scroll);

			if (player.isAlive == 1) {
				{
					Novice::DrawQuad(
						int(player.center.x - (player.len.x + player.sizeChange.x) / 2) + int(scroll), int(player.center.y - (player.len.y + player.sizeChange.y) / 2),
						int(player.center.x + (player.len.x + player.sizeChange.x) / 2) + int(scroll), int(player.center.y - (player.len.y + player.sizeChange.y) / 2),
						int(player.center.x - (player.len.x + player.sizeChange.x) / 2) + int(scroll), int(player.center.y + (player.len.y + player.sizeChange.y) / 2),
						int(player.center.x + (player.len.x + player.sizeChange.x) / 2) + int(scroll), int(player.center.y + (player.len.y + player.sizeChange.y) / 2),
						32, 0, blockSize, blockSize, playerTex, 0xFFFFFFFF);
				}
			}
			//Novice::DrawSprite(0, 0, timeFrame, 1.0f, 1.0f, 0.0f, WHITE);
			//for (int i = 0; i < 2; i++)
			//{
			//	Novice::DrawSpriteRect(/*118*/5+ 40 * i, 5, 64 * timeDisplay.seconds[i], 0, 64, 64, numberTex, 1.0f / 12.0f, 1.0f / 1.2f, 0.0f, 0xDD69B4FF);
			//}
			//Novice::DrawSprite(524, 640, scoreFrame, 1.0f, 1.0f, 0.0f, WHITE);;
			//for (int i = 0; i < 3; i++)
			//{
			//	Novice::DrawSpriteRect(555 + 40 * i, 643, 64 * score.num[i], 0, 64, 64, numberTex, 1.0f / 12.0f, 1.0f / 1.2f, 0.0f, 0xDD69B4FF);
			//}
			break;
		}

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
