#pragma once
#include <Novice.h>
#include "Vector2.h"
#include <functional>
#define _USE_MATH_DEFINES
#include <math.h>
#include "Easing.h"

const int kStageNum = 12;

struct Matrix2x2
{
	float m[2][2];
};


class Select
{
public:
	~Select();
	void Initialize();

	void Update(const char* keys, const char* preKeys);
	void Draw();

	int GetStageNum() { return stageNum_; }
	void SetHighScore();

	void PlusStageNum();
	void MinasStageNum();

	void StageSelectUpdate();
	void StarUpdate();
	void ArrowUpdate();
	void ButtonUpdate();
	void HighScoreUpdate();

	void PopHuman();
	void HumanUpdate();

	Vector2 Lerp(const Vector2& v1, const Vector2& v2, float t);

	void QuadVer(Vector2 pos, float width, float height, Vector2& lt, Vector2& rt, Vector2& lb, Vector2& rb);


	Matrix2x2 MakeRotateMatrix(float theta)
	{
		Matrix2x2 matrix1;
		matrix1.m[0][0] = cosf(theta);
		matrix1.m[0][1] = sinf(theta);
		matrix1.m[1][0] = -sinf(theta);
		matrix1.m[1][1] = cosf(theta);
		return matrix1;
	}

	Vector2 Multiply(Vector2 vector, Matrix2x2 matrix)
	{
		Vector2 vector3;
		vector3.x = (vector.x * matrix.m[0][0]) + (vector.y * matrix.m[1][0]);
		vector3.y = (vector.x * matrix.m[0][1]) + (vector.y * matrix.m[1][1]);
		return vector3;
	}

private:

	int bgTexture_ = 0;
	int starTexture_ = 0;
	int stageTexture_[kStageNum];
	int arrowTexture_[2];
	int buttonTexture_ = 0;
	int uiTexture_ = 0;
	int humanTexture_ = 0;
	int numberTexture_ = 0;
	int stageNumTexter_[kStageNum];
	int scoreFontTexter_ = 0;

	Quad bg_{};
	Quad star_{};
	Quad stage_[kStageNum];
	Quad number_[4][5];
	Quad arrow_[2];
	Quad button_{};
	Quad ui_{};
	Quad human_{};
	Quad humanRotate_{};
	Quad stageNumber_{};
	Quad scoreFont_{};


	const int kTimeCount = 1;
	const int kButtonTime = 300;
	const int kStarTime = 120;
	const int kStarEraseTime = 10;
	const int kStageChangeTime = 20;
	const float kStageChangeCo = 0.1f;
	const float kButtonCo_ = 0.05f;
	const int kHumanPopTime_ = 300;

	int buttonTime_ = 0;
	float poyonTime_ = 0;
	int starTime_ = 0;
	int stageChangeInterval_ = 0;
	float stageChangeTime_ = 0;
	float arrowTimeCo = 0.02f;
	float arrowTime_ = 0;
	int humanPopTime_ = 0;
	float theta_ = 0;

	bool isStarDraw_ = true;

	int num_[4][5];
	int numberSize_ = 64;

	int stageNum_ = 1;
	int changeStage = 0;
	Vector2 topPos_{};
	Vector2 nowPos_{};
	Vector2 underPos_{};
	Vector2 mainRad_{};
	Vector2 subRad_{};

	Vector2 arrowStartPos_[2];
	Vector2 arrowStopPos_[2];

	Vector2 buttonStartRad_{};
	Vector2 buttonStopRad_{};

	bool isPoyon_ = false;
	bool isPoyonChange_ = false;
	bool isPopHuman_ = false;

	int highScore_[5]{};
	int highScore2_[5]{};
	int highScore3_[5]{};
	int highScore4_[5]{};
	int stageNumDraw_ = 0;

	Vector2 rotateLeftTop{};
	Vector2 rotateRightTop{};
	Vector2 rotateLeftBottom{};
	Vector2 rotateRightBottom{};

	Vector2 scorePos_{};
};
