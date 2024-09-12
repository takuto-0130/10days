#pragma once
#include "Struct.h"
#include "Function.h"
#include <memory>
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

class PlayerClass
{
private:
	Player player_{};	// プレイヤー本体
	MapChipNum* map_ = nullptr;	//マップチップのポインタ

	Vector2 kResetPos = { 19 * blockSize + blockSize / 2, 2 * blockSize + blockSize / 2 };	//プレイヤーの初期位置
	Vector2 startPosition = kResetPos;	//プレイヤーのスタート位置
	Vector2 startPosReturn = kResetPos;	//プレイヤーのスクロールスタート位置
	Vector2 stopPosition = kResetPos;	//プレイヤーの射出後にとまった位置

	float playerShotAngle = 0;	// プレイヤーの角度
	const float kPlayerChangeAngle = float(M_PI) / 90;	// プレイヤーの射出される角度調整（キーボード）
	Vector2 playerShotDir = { 0,2 };	// プレイヤーの射出される初期角度
	Vector2 viewDir = playerShotDir;
	float kResistPower = 0.15f; // ブロックの抵抗力

	const float kMoveChangeAngle = float(M_PI) / 150;	// 体操作中の角度

	bool isShot = false;	// 体が射出されているか否か
	bool isMove = false;	// 体操作中か否か
	bool isReturn = false;	// スクロール中か否か

	float t = 0;	// 体が戻ってく時の"t"の値
	float tIncrease = 0.01f;	// 体が戻ってく時の"t"の増加量

	float tReturnNow = 0;	// 下半身に合わせたスクロール時の"t"の値
	float tReturn = 0.02f;	// 下半身に合わせたスクロール時の"t"の増加量

	float* scroll_ = nullptr;	// スクロール値のポインタ

	int playerTex = 0;	// プレイヤーのテクスチャハンドル
	int playerTex2 = 0;	// プレイヤーのテクスチャハンドル

	Vector2 topPosition = kResetPos;	//プレイヤーの射出後にとまった位置

	float kPlayerLimitDistance = 500.0f;


	/////////////////////////////////////////////////////////////////////

	Vector2 start1 = {};
	Vector2 stop1 = {};
	float t1 = 0;
	float tIncrease1 = 0.04f;
	bool kabe = false;

	const float kHitMisalignment = 1.0f; // 力技の 1 ずらし
	float kMoveDistance = 100.0f; // ブロックに当たったときに進む距離

	Vector2 direction{};
	Vector2 velocity = {};
	float speed = {};

	////////////////////////////////////////////////////////////////////

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="map">マップチップデータ</param>
	/// <param name="scroll">スクロール値</param>
	PlayerClass(MapChipNum* map, float* scroll);
	~PlayerClass();


	void ApplyGlobalVariables();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="keys">キー入力</param>
	/// <param name="preKeys">前フレームのキー入力</param>
	void Update(const char* keys, const char* preKeys);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// マップチップデータの登録
	/// </summary>
	/// <param name="map">マップチップデータ</param>
	void SetMap(MapChipNum* map) { map_ = map; }

	/// <summary>
	/// スクロール値を登録
	/// </summary>
	/// <param name="scroll">スクロール値</param>
	void SetScroll(float* scroll) { scroll_ = scroll; }

	/// <summary>
	/// プレイヤー構造体を取得
	/// </summary>
	Player& GetPlayer() { return player_; }

	/// <summary>
	/// プレイヤーの射出する向きを決定・射出
	/// </summary>
	/// <param name="keys">キー入力</param>
	/// <param name="preKeys">前フレームのキー入力</param>
	void AngleSet(const char* keys, const char* preKeys);

	void RefrectShooting();

	void RefrectMoving();

	/// <summary>
	/// プレイヤー射出中
	/// </summary>
	void Shooting();

	/// <summary>
	/// プレイヤー操作中
	/// </summary>
	/// <param name="keys">キー入力</param>
	/// <param name="preKeys">前フレームのキー入力</param>
	void Operation(const char* keys);

	/// <summary>
	/// 画面スクロール
	/// </summary>
	void ScreenScroll();
};

