#include "Function.h"
#include <memory>
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG
#include "MyClass/Player/Player.h"
#include "MyClass/GlobalVariables/GlobalVariables.h"
#include "MyClass/MapLoad/MapLoad.h"
#include "MyClass/StageSelect/Select.h"

const char kWindowTitle[] = "10daysTest";

// キー入力結果を受け取る箱
char keys[256] = { 0 };
char preKeys[256] = { 0 };

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, 720);

	//float gravity = 0.02f;

#pragma region // 画像読み込み
	int mapTex = Novice::LoadTexture("./Resources/mapBlock.png");
	int pinkBlock = Novice::LoadTexture("./Resources/stage/Block.png");
	int bgTex = Novice::LoadTexture("./Resources/bg.png");
	int block = Novice::LoadTexture("white1x1.png");
	//int titleTex = Novice::LoadTexture("./Resources/title.png");

#ifndef _DEBUG
	int numberTex = Novice::LoadTexture("./Resources/numberTex.png");
	int blueNumberTex = Novice::LoadTexture("./Resources/number/namber_green.png");
	int timeFrame = Novice::LoadTexture("./Resources/stage/time_frame.png");
	int scoreFukidasi = Novice::LoadTexture("./Resources/stage/score_fukidasi.png");
#endif // !_DEBUG
	int scoreFrame = Novice::LoadTexture("./Resources/stage/score_frame.png");
	int scoreGauge = Novice::LoadTexture("./Resources/stage/score.png");

#pragma endregion

	// グローバル変数の読み込み
	GlobalVariables::GetInstance()->LoadFiles();


	MapChipNum map{};	// マップチップのデータを保存
	LoadMap(map, "./Resources/testMap.csv");
	MapChipNum startMap = map;	// ステージスタート時のマップを保存


	const float kResistPower = 0.15f;	// ブロックの抵抗力

	//bool isClear = 0;

	int kHitStopFrame = 5;	// ブロックに衝突したときのヒットストップのフレーム数
	int hitStopTimer = 0;	// ヒットストップのタイマー

	float scroll = 0;	// 画面のスクロール

	std::unique_ptr<BrockEmitter> blockParticle = nullptr;	// ブロック破壊時のパーティクル
	blockParticle.reset(new BrockEmitter());
	blockParticle->Initialize();

	uint32_t kPlayTime = 800;	// ステージの最大プレイ時間
	uint32_t playTimer = kPlayTime;	// ステージの残りプレイ時間
	Timedisp timeDisplay{};	// プレイ時間表示用

	uint32_t breakCount = 0;	// 壊したブロックの数
	ScoreDisp score{};	// 壊したブロックの数表示用
	ScoreDisp beforeScore{};
	beforeScore = score;
	uint32_t maxBreakCount = 0;
	MaxBreakCountSearch(maxBreakCount, startMap);

	std::unique_ptr<PlayerClass> playerClass = std::make_unique<PlayerClass>(&map, &scroll);	// プレイヤー処理
	std::unique_ptr<MapLoad> mapLoad = std::make_unique<MapLoad>(&map);
	mapLoad->Initialize();

	std::unique_ptr<Select> stargeSelect = std::make_unique<Select>();
	stargeSelect->Initialize();

	enum class Scene {
		Title,
		Select,
		Game,
		Clear
	};

	Scene scene = Scene::Game;

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

		switch (scene) {
		case Scene::Title:
			if (keys[(DIK_SPACE)] && !preKeys[(DIK_SPACE)])
			{
				scene = Scene::Select;
			}
			break;
		case Scene::Select:
			stargeSelect->Update(keys, preKeys);
			if (keys[(DIK_SPACE)] && !preKeys[(DIK_SPACE)])
			{
				// スペース押したらSutageNum（ステージ番号）取得
				mapLoad->Update(stargeSelect->GetStageNum());
				startMap = map;
				scene = Scene::Game;
				playTimer = kPlayTime;
				breakCount = 0;
				playerClass->Initialize();
				MaxBreakCountSearch(maxBreakCount, startMap);
			}
			if (keys[(DIK_BACKSPACE)] && !preKeys[(DIK_BACKSPACE)])
			{
				scene = Scene::Title;
			}
			break;
		case Scene::Game:

#pragma region // ゲームシーンアップデート
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
			}
			blockParticle->Update();
#pragma endregion
			if (keys[(DIK_BACKSPACE)] && !preKeys[(DIK_BACKSPACE)])
			{
				scene = Scene::Select;
			}

			break;
		case Scene::Clear:
			break;
		}



#ifdef _DEBUG

		if (preKeys[DIK_R] == 0 && keys[DIK_R] != 0)
		{

			map = startMap;
			playTimer = kPlayTime;
			breakCount = 0;
			playerClass->Initialize();
			breakCount = 0;
			playTimer = kPlayTime;
		}
#endif // _DEBUG


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///


		switch (scene) {
		case Scene::Title:
			break;
		case Scene::Select:
			stargeSelect->Draw();
			break;
		case Scene::Game:
#pragma region // ゲームシーン描画
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
			DrawScoreGauge(breakCount, maxBreakCount, scoreFrame, scoreGauge);
#ifndef _DEBUG
			Novice::DrawSprite(0, 0, timeFrame, 0.7f,0.7f, 0.0f, WHITE);
			if (playTimer > 599) {
				for (int i = 0; i < 2; i++)
				{
					Novice::DrawSpriteRect(13 + 43 * i, 18, 64 * timeDisplay.seconds[i], 0, 64, 64, blueNumberTex, 1.0f / 12.0f, 1.0f / 1.2f, 0.0f, 0xFFFFFFFF);
				}
			}
			else if(playTimer > 359)
			{
				Novice::DrawSpriteRect(33, 18, 64 * timeDisplay.seconds[1], 0, 64, 64, numberTex, 1.0f / 12.0f, 1.0f / 1.2f, 0.0f, 0xFFFFFFFF);
			}
			else
			{
				float a = float(playTimer % 60);
				Novice::DrawSpriteRect(int(33.0f - (12.0f * (a / 60.0f))), int(18.0f - (12.0f * (a / 60.0f))), 64 * timeDisplay.seconds[1], 0, 64, 64, numberTex, (1.0f + (a / 120.0f)) / 12.0f, (1.0f + (a / 120.0f)) / 1.2f, 0.0f, 0xFFFFFFFF);
			}
			Novice::DrawSprite(490, 620, scoreFukidasi, 1.0f, 0.8f, 0.0f, WHITE);;
			for (int i = 0; i < 4; i++)
			{
				if(score.num[i] == beforeScore.num[i])
				{
					Novice::DrawSpriteRect(548 + 43 * i, 632, 64 * score.num[i], 0, 64, 64, numberTex, 1.0f / 12.0f, 1.0f / 1.2f, 0.0f, 0xFFFFFFFF);
				}
				else {
					Novice::DrawSpriteRect(548 + 43 * i, 622, 64 * score.num[i], 0, 64, 64, numberTex, 1.0f / 12.0f, 1.0f / 1.2f, 0.0f, 0xFFFFFFFF);
				}
			}
#endif // !_DEBUG
			beforeScore = score;
#pragma endregion
			break;
		case Scene::Clear:
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
