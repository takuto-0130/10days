#include "Function.h"
#include <memory>
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG
#include "MyClass/Player/Player.h"
#include "MyClass/GlobalVariables/GlobalVariables.h"
#include "MyClass/MapLoad/MapLoad.h"
#include "MyClass/StageSelect/Select.h"
#include "MyClass/Result/Result.h"
#include "MyClass/ScoreLibrary//ScoreLibrary.h"
#include <Input.h>
#include <Audio.h>

const char kWindowTitle[] = "10daysTest";

// キー入力結果を受け取る箱
char keys[256] = { 0 };
char preKeys[256] = { 0 };

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, 720);

	Audio* audio = Audio::GetInstance();

	uint32_t BGM = audio->LoadWave("./Resources/Sound/BGM_1.mp3");
	audio->PlayWave(BGM, true);
	audio->SetVolume(BGM, 1.0f);

	uint32_t SE_block = audio->LoadWave("./Resources/Sound/SE_block_2.mp3");
	uint32_t SE_click = audio->LoadWave("./Resources/Sound/SE_click_1.mp3");

#pragma region // 画像読み込み
	int mapTex = Novice::LoadTexture("./Resources/stage/playerBlock.png");
	int pinkBlock = Novice::LoadTexture("./Resources/stage/Block.png");
	int bgTex = Novice::LoadTexture("./Resources/stage/backGround.png");
	int block = Novice::LoadTexture("white1x1.png");

#ifndef _DEBUG
	int numberTex = Novice::LoadTexture("./Resources/numberTex.png");
	int blueNumberTex = Novice::LoadTexture("./Resources/number/namber_green.png");
	int timeFrame = Novice::LoadTexture("./Resources/stage/time_frame.png");
	int scoreFukidasi = Novice::LoadTexture("./Resources/stage/score_fukidasi.png");
#endif // !_DEBUG
	int scoreFrame = Novice::LoadTexture("./Resources/stage/score_frame.png");
	int scoreGauge = Novice::LoadTexture("./Resources/stage/score.png");
	int titleTex = Novice::LoadTexture("./Resources/title.png");
	int controllerTex = Novice::LoadTexture("./Resources/controller.png");
	int aTex = Novice::LoadTexture("./Resources/StageSelect/A.png");
#pragma endregion

	// グローバル変数の読み込み
	GlobalVariables::GetInstance()->LoadFiles();


	MapChipNum map{};	// マップチップのデータを保存
	LoadMap(map, "./Resources/csvMap/stage_1.csv");
	MapChipNum startMap = map;	// ステージスタート時のマップを保存


	const float kResistPower = 0.15f;	// ブロックの抵抗力

	//bool isClear = 0;

	int kHitStopFrame = 5;	// ブロックに衝突したときのヒットストップのフレーム数
	int hitStopTimer = 0;	// ヒットストップのタイマー

	float scroll = 0;	// 画面のスクロール

	std::unique_ptr<BrockEmitter> blockParticle = nullptr;	// ブロック破壊時のパーティクル
	blockParticle.reset(new BrockEmitter());
	blockParticle->Initialize();

	uint32_t kPlayTime = 5999;	// ステージの最大プレイ時間（5999）
	uint32_t playTimer = kPlayTime;	// ステージの残りプレイ時間
	Timedisp timeDisplay{};	// プレイ時間表示用

	uint32_t breakCount = 0;	// 壊したブロックの数
	ScoreDisp score{};	// 壊したブロックの数表示用
	ScoreDisp beforeScore{};
	beforeScore = score;
	uint32_t maxBreakCount = 0;
	MaxBreakCountSearch(maxBreakCount, startMap);

	std::unique_ptr<MapLoad> mapLoad = std::make_unique<MapLoad>(&map);
	mapLoad->Initialize();

	std::unique_ptr<PlayerClass> playerClass = std::make_unique<PlayerClass>(&map, &scroll, &mapLoad->GetNowStage());	// プレイヤー処理

	std::unique_ptr<Select> stageSelect = std::make_unique<Select>();
	stageSelect->Initialize();

	std::unique_ptr<Result> result = std::make_unique<Result>();
	result->Initialize();

	std::unique_ptr<ScoreLibrary> historyScore = std::make_unique<ScoreLibrary>();
	historyScore->SetStage(&mapLoad->GetNowStage());

	stageSelect->SetHighScore();

	enum class Scene {
		Title,
		Select,
		Game,
		Clear
	};

	Scene scene = Scene::Title;

	XINPUT_STATE joyState;
	Input::GetInstance()->GetJoystickState(0, joyState);
	XINPUT_STATE beforeJoyState = joyState;

	const int kEndTime = 90;
	const int kEndAnimationTime = 60;
	int endTimer = 0;

	bool isController = true;

	const int kTitleSelectFadeTimer = 90;
	int fadeTimer = 0;

	bool next = false;

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
		audio->SetVolume(BGM, 1.0f);
		switch (scene) {
		case Scene::Title:
			if (Input::GetInstance()->GetJoystickState(0, joyState)) {
				if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) && (beforeJoyState.Gamepad.wButtons ^ XINPUT_GAMEPAD_A) && !next) {
					if (!isController) {
						stageSelect->Initialize();
						stageSelect->SetHighScore();
						next = true;
						beforeJoyState = joyState;
						audio->PlayWave(SE_click);
						fadeTimer = 0;
					}
					if (isController) {
						isController = false;
					}
				}
			}
			else if (keys[(DIK_SPACE)] && !preKeys[(DIK_SPACE)] && !next)
			{
				stageSelect->Initialize();
				stageSelect->SetHighScore();
				next = true;
				audio->PlayWave(SE_click);
			}

			if (fadeTimer < kTitleSelectFadeTimer) {
				fadeTimer++;
			}
			else if (next == true) {
				scene = Scene::Select;
				fadeTimer = 0;
			}
			break;
		case Scene::Select:
			if (fadeTimer < kTitleSelectFadeTimer) {
				fadeTimer++;
			}
			else {
				stageSelect->Update(keys, preKeys);
				if (Input::GetInstance()->GetJoystickState(0, joyState)) {
					if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) && (beforeJoyState.Gamepad.wButtons ^ XINPUT_GAMEPAD_A)) {
						mapLoad->Update(stageSelect->GetStageNum());
						startMap = map;
						scene = Scene::Game;
						playTimer = kPlayTime;
						breakCount = 0;
						playerClass->Initialize();
						MaxBreakCountSearch(maxBreakCount, startMap);
						beforeJoyState = joyState;
						audio->PlayWave(SE_click);
					}
				}
				else if (keys[(DIK_SPACE)] && !preKeys[(DIK_SPACE)])
				{
					// スペース押したらSutageNum（ステージ番号）取得
					mapLoad->Update(stageSelect->GetStageNum());
					startMap = map;
					scene = Scene::Game;
					playTimer = kPlayTime;
					breakCount = 0;
					playerClass->Initialize();
					MaxBreakCountSearch(maxBreakCount, startMap);
					audio->PlayWave(SE_click);
				}


				if (Input::GetInstance()->GetJoystickState(0, joyState)) {
					if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) && (beforeJoyState.Gamepad.wButtons ^ XINPUT_GAMEPAD_B)) {
						scene = Scene::Title;
						beforeJoyState = joyState;
						audio->PlayWave(SE_click);
						next = false;
					}
				}
				else if (keys[(DIK_BACKSPACE)] && !preKeys[(DIK_BACKSPACE)])
				{
					scene = Scene::Title;
					audio->PlayWave(SE_click);
					next = false;
				}
			}
			break;
		case Scene::Game:

#pragma region // ゲームシーンアップデート
			// HitStop
			if (playTimer > 0 && maxBreakCount > breakCount) {
				playTimer--;
				endTimer = 0;
			}
			else {
#pragma region // ステージ終了時
				audio->SetVolume(BGM, 0.5f);
				if (endTimer < kEndTime) {
					endTimer++;
				}
				else {
					result->Initialize();
					result->SetScore(breakCount);
					result->SetStage(static_cast<int>(mapLoad->GetNowStage()) + 1);
					historyScore->Update(breakCount);
					float scorePercent = float(breakCount) / float(maxBreakCount);
					if (scorePercent <= 0.5f) {
						result->SetEvaluation(0);
					}
					else if (scorePercent <= 0.8f) {
						result->SetEvaluation(1);
					}
					else {
						result->SetEvaluation(2);
					}
					result->UIUpdate();
					scene = Scene::Clear;
					beforeJoyState = joyState;
				}
#pragma endregion
			}
			TimeDisplay(playTimer, timeDisplay);


			if (playTimer > 0 && maxBreakCount > breakCount)
			{
				if (hitStopTimer <= 0) {
					//					
					playerClass->Update(keys, preKeys, joyState, beforeJoyState);

					CollisionBlock(map, playerClass->GetPlayer(), kResistPower, kHitStopFrame, hitStopTimer, blockParticle.get(), breakCount);

					ScoreDisplay(breakCount, score);

				}
				if (hitStopTimer == kHitStopFrame) {
					audio->PlayWave(SE_block);
				}
				if (hitStopTimer >= 0) {
					hitStopTimer--;
				}
			}
			blockParticle->Update();
#pragma endregion
			if (Input::GetInstance()->GetJoystickState(0, joyState)) {
				if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) && (beforeJoyState.Gamepad.wButtons ^ XINPUT_GAMEPAD_B)) {
					historyScore->Update(breakCount);
					stageSelect->SetHighScore();
					scene = Scene::Select;
					beforeJoyState = joyState;
				}
			}
			else if (keys[(DIK_BACKSPACE)] && !preKeys[(DIK_BACKSPACE)])
			{
				scene = Scene::Select;
			}

			break;
		case Scene::Clear:
			audio->SetVolume(BGM, 0.5f);
			result->Update(keys, preKeys);
			if (Input::GetInstance()->GetJoystickState(0, joyState)) {
				if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) && (beforeJoyState.Gamepad.wButtons ^ XINPUT_GAMEPAD_A)) {
					if (result->GetChaangeNext() == 1) {
						if (mapLoad->GetNowStage() != Stage::Stage5) {
							mapLoad->Update(static_cast<int>(mapLoad->GetNowStage()) + 2);
							startMap = map;
							scene = Scene::Game;
							playTimer = kPlayTime;
							breakCount = 0;
							playerClass->Initialize();
							MaxBreakCountSearch(maxBreakCount, startMap);
						}
					}
					else {
						scene = Scene::Title;
						next = false;
					}
					beforeJoyState = joyState;
					audio->PlayWave(SE_click);
				}
			}
			else if (keys[(DIK_SPACE)] && !preKeys[(DIK_SPACE)])
			{
				if (result->GetChaangeNext() == 1) {
					if (mapLoad->GetNowStage() != Stage::Stage12) {
						mapLoad->Update(static_cast<int>(mapLoad->GetNowStage()) + 2);
						startMap = map;
						scene = Scene::Game;
						playTimer = kPlayTime;
						breakCount = 0;
						playerClass->Initialize();
						MaxBreakCountSearch(maxBreakCount, startMap);
						blockParticle->Erase();
					}
				}
				else {
					stageSelect->SetHighScore();
					scene = Scene::Select;
					scene = Scene::Title;
					blockParticle->Erase();
				}
				audio->PlayWave(SE_click);
			}

			if (Input::GetInstance()->GetJoystickState(0, joyState)) {
				if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) && (beforeJoyState.Gamepad.wButtons ^ XINPUT_GAMEPAD_B)) {
					stageSelect->SetHighScore();
					scene = Scene::Select;
					beforeJoyState = joyState;
					audio->PlayWave(SE_click);
				}
			}
			else if (keys[(DIK_BACKSPACE)] && !preKeys[(DIK_BACKSPACE)])
			{
				stageSelect->SetHighScore();
				scene = Scene::Select;
				audio->PlayWave(SE_click);
			}
			break;
		}


		beforeJoyState = joyState;

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
			Novice::DrawSprite(0, 0, titleTex, 1.0f, 1.0f, 0.0f, WHITE);
			if (fadeTimer < kTitleSelectFadeTimer) {
				if(next)
				{
					int alpha = int(255.0f * (float(fadeTimer) / float(kTitleSelectFadeTimer)));
					Novice::DrawBox(0, 0, 1280, 720, 0.0f, 0x00000000 + alpha, kFillModeSolid);
					if (fadeTimer == 0) {
						Novice::DrawBox(0, 0, 1280, 720, 0.0f, BLACK, kFillModeSolid);
					}
				}
			}
			else {
				if (next)
				{
					Novice::DrawBox(0, 0, 1280, 720, 0.0f, BLACK, kFillModeSolid);
				}
			}
			if (isController) {
				Novice::DrawBox(0, 0, 1280, 720, 0.0f, 0x111122FF, kFillModeSolid);
				Novice::DrawSprite(440, 150, controllerTex, 1.0f, 1.0f, 0.0f, WHITE);
				Novice::DrawSprite(840, 485, aTex, 1.0f, 1.0f, 0.0f, WHITE);
			}
			break;
		case Scene::Select:
			stageSelect->Draw();
			if (fadeTimer < kTitleSelectFadeTimer) {
				int alpha = int(255.0f * (float(fadeTimer) / float(kTitleSelectFadeTimer)));
				Novice::DrawBox(0, 0, 1280, 720, 0.0f, BLACK - alpha, kFillModeSolid);
			}
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
			Novice::DrawSprite(0, 0, timeFrame, 0.7f, 0.7f, 0.0f, WHITE);
			if (playTimer > 599) {
				for (int i = 0; i < 2; i++)
				{
					Novice::DrawSpriteRect(13 + 43 * i, 18, 64 * timeDisplay.seconds[i], 0, 64, 64, blueNumberTex, 1.0f / 12.0f, 1.0f / 1.2f, 0.0f, 0xFFFFFFFF);
				}
			}
			else if (playTimer > 359)
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
				if (score.num[i] == beforeScore.num[i])
				{
					Novice::DrawSpriteRect(548 + 43 * i, 632, 64 * score.num[i], 0, 64, 64, numberTex, 1.0f / 12.0f, 1.0f / 1.2f, 0.0f, 0xFFFFFFFF);
				}
				else {
					Novice::DrawSpriteRect(548 + 43 * i, 622, 64 * score.num[i], 0, 64, 64, numberTex, 1.0f / 12.0f, 1.0f / 1.2f, 0.0f, 0xFFFFFFFF);
				}
			}
#endif // !_DEBUG
			if (endTimer > 1) {
				if (endTimer < kEndAnimationTime) {
					Novice::DrawBox(1280 - int(1280.0f * float(endTimer) / float(kEndAnimationTime)), 170, 1280, 380, 0.0f, 0x443399FF, kFillModeSolid);
				}
				else {
					Novice::DrawBox(0, 170, 1280, 380, 0.0f, 0x443399FF, kFillModeSolid);
				}
			}
			beforeScore = score;
#pragma endregion
			break;
		case Scene::Clear:
			result->Draw();
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
