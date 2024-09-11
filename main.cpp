#include "Function.h"
#include <memory>
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG
#include "MyClass/Player/Player.h"
#include "MyClass/GlobalVariables/GlobalVariables.h"


const char kWindowTitle[] = "10daysTest";

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
	int pinkBlock = Novice::LoadTexture("./Resources/Block.png");
	int bgTex = Novice::LoadTexture("./Resources/bg.png");
	int block = Novice::LoadTexture("white1x1.png");
	//int titleTex = Novice::LoadTexture("./Resources/title.png");

#ifndef _DEBUG
	int numberTex = Novice::LoadTexture("./Resources/number.png");
	int timeFrame = Novice::LoadTexture("./Resources/timeFrame.png");
	int scoreFrame = Novice::LoadTexture("./Resources/scoreFrame.png");
#endif // !_DEBUG

	// グローバル変数の読み込み
	GlobalVariables::GetInstance()->LoadFiles();


	MapChipNum map{};	// マップチップのデータを保存

	LoadMap(map, "./Resources/testMap.csv");

	MapChipNum startMap = map;	// ステージスタート時のマップを保存


	const float kResistPower = 0.15f;	// ブロックの抵抗力

	bool isClear = 0;

	int kHitStopFrame = 5;	// ブロックに衝突したときのヒットストップのフレーム数
	int hitStopTimer = 0;	// ヒットストップのタイマー

	float scroll = 0;	// 画面のスクロール

	std::unique_ptr<BrockEmitter> blockParticle = nullptr;	// ブロック破壊時のパーティクル
	blockParticle.reset(new BrockEmitter());
	blockParticle->Initialize();

	uint32_t kPlayTime = 5999;	// ステージの最大プレイ時間
	uint32_t playTimer = kPlayTime;	// ステージの残りプレイ時間
	Timedisp timeDisplay{};	// プレイ時間表示用

	uint32_t breakCount = 0;	// 壊したブロックの数
	ScoreDisp score{};	// 壊したブロックの数表示用

	std::unique_ptr<PlayerClass> playerClass = std::make_unique<PlayerClass>(&map, &scroll);	// プレイヤー処理

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
		
		// グローバル変数の更新処理
		GlobalVariables::GetInstance()->Update();
#ifdef _DEBUG
		ImGui::Begin("window");
		ImGui::DragFloat("scroll", &scroll, 1.0f);
		ImGui::End();
#endif // _DEBUG

		isParticle = 0;
		isClear = 0;

		// HitStop
		if (playTimer > 0) {
			playTimer--;
		}
		TimeDisplay(playTimer, timeDisplay);


		if (playTimer > 0)
		{
			if (hitStopTimer <= 0) {
				//					
				playerClass->Update(keys, preKeys);

				CollisionBlock(map, playerClass->GetPlayer(), kResistPower, kHitStopFrame, hitStopTimer, blockParticle.get(), breakCount);

				ScoreDisplay(breakCount, score);

			}
			if (hitStopTimer >= 0) {
				hitStopTimer--;
			}
			blockParticle->Update();


		}

//#ifdef _DEBUG

		if (preKeys[DIK_R] == 0 && keys[DIK_R] != 0)
		{

			map = startMap;
			playTimer = kPlayTime;
			breakCount = 0;
			playerClass->Initialize();
			breakCount = 0;
			playTimer = kPlayTime;
		}
//#endif // _DEBUG


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		Novice::DrawSprite(0, 0, bgTex, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);

		for (int i = 0; i < mapChipHeight; i++)
		{
			for (int j = 0; j < mapChipWidth; j++)
			{
				if (((j * blockSize) + scroll > -blockSize) && ((j * blockSize) + scroll < 1312))
				{
					if (map.mapData[i][j] == 1)
					{
						Novice::DrawSpriteRect(int(blockSize * j) + int(scroll), blockSize * i, 0, 0, blockSize, blockSize, mapTex, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
					}
					if (map.mapData[i][j] == 3)
					{
						Novice::DrawSpriteRect(int(blockSize * j) + int(scroll), blockSize * i, 0, 0, blockSize, blockSize, pinkBlock, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
					}
					if (map.mapData[i][j] == 4)
					{
						Novice::DrawSpriteRect(int(blockSize * j) + int(scroll), blockSize * i, 0, 0, blockSize, blockSize, pinkBlock, 1.0f, 1.0f, 0.0f, 0x9999FFFF);
					}
					if (map.mapData[i][j] == 2)
					{
						Novice::DrawSpriteRect(int(blockSize * j) + int(scroll), blockSize * i, 0, 0, 1, 1, block, 32.0f, 32.0f, 0.0f, 0x55AA55FF);
					}
				}
			}
		}

		blockParticle->Draw(scroll);

		playerClass->Draw();

#ifndef _DEBUG
		Novice::DrawSprite(0, 0, timeFrame, 1.0f, 1.0f, 0.0f, WHITE);
		for (int i = 0; i < 2; i++)
		{
			Novice::DrawSpriteRect(/*118*/5 + 40 * i, 5, 64 * timeDisplay.seconds[i], 0, 64, 64, numberTex, 1.0f / 12.0f, 1.0f / 1.2f, 0.0f, 0xDD69B4FF);
		}
		Novice::DrawSprite(524, 640, scoreFrame, 1.0f, 1.0f, 0.0f, WHITE);;
		for (int i = 0; i < 3; i++)
		{
			Novice::DrawSpriteRect(555 + 40 * i, 643, 64 * score.num[i], 0, 64, 64, numberTex, 1.0f / 12.0f, 1.0f / 1.2f, 0.0f, 0xDD69B4FF);
		}
#endif // !_DEBUG

		

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
