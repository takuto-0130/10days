#pragma once
#include <Novice.h>
#include "Vector2.h"
#include <functional>
#define _USE_MATH_DEFINES
#include <math.h>
#include "Easing.h"


class Result
{
public:

	Result();
	~Result();
	void Initialize();

	void Update(const char* keys, const char* preKeys);
	void Draw();

	void SetEvaluation(int evaluation) { evaluationPoint_ = evaluation; }
	void SetScore(int32_t score) { score_ = score; }
	void SetStage(int stageNum) { stageNumber_ = stageNum; }

	void StarUpdate();
	void ScoreUpdate();
	void UIUpdate();
	void HumanUpdate();

	void ChangeNext();

	int GetChaangeNext() { return changeNext_; }

	Vector2 Lerp(const Vector2& v1, const Vector2& v2, float t);

	void QuadVer(Vector2 pos, float width, float height, Vector2& lt, Vector2& rt, Vector2& lb, Vector2& rb);

private:

	int bgTexture_ = 0;
	int starTexture_ = 0;
	int uiTexture_[2];
	int forUITexture_[2];
	int buttonTexture_ = 0;
	int underBarTexture_ = 0;
	int numberTextureYellow_ = 0;
	int numberTextureRed_ = 0;
	int stageTexture_ = 0;
	int humanTexture_ = 0;
	int evaluationTexture_[3];

	int nextUITex_[2];
	int evaluationTex_ = 0;

	Quad bg_{};
	Quad star_{};
	Quad ui_[2];
	Quad button_{};
	Quad underBar_{};
	Quad number_[4];
	Quad stageNum_{};
	Quad stage_{};
	Quad human_{};
	Quad evaluation_{};

	Vector2 uiTextureRad_[2];

	const int kTimeCount = 1;
	const int kStarTime = 120;
	const int kStarEraseTime = 10;
	const float kHumanCount = 0.02f;
	const int kEvaluationTime = 60;
	const float kEvaluationCount = 0.1f;
	const int kScoreCT = 30;

	int starTime_ = 0;
	float buttonTime_ = 0;
	float buttonCount_ = 0.05f;
	float humanCount = 0.05f;
	float humanPopTime_ = 0;
	float humanFloatTime_ = 0;
	int evaluationTime_ = 0;
	float evaluationEasTime_ = 0;
	int scoreCT_ = 0;

	bool isStarDraw_ = true;
	bool isEvaluationDraw_ = false;
	bool isHumanDraw_ = false;
	bool isPopHuman_ = false;
	bool isScoreDrawComp0_ = false;
	bool isScoreDrawComp1_ = false;
	bool isScoreDrawComp2_ = false;
	bool isScoreDrawComp3_ = false;
	bool isScoreDrawComp4_ = false;

	int evaluationPoint_ = 0;
	int32_t score_ = 0;
	int score2_ = 0;
	int score3_ = 0;
	int score4_ = 0;
	int stageNumber_ = 0;
	int changeNext_ = 1;

	int num_[4];
	int numberSize_ = 64;

	Vector2 buttonStartRad_{};
	Vector2 buttonStopRad_{};

	Vector2 HumanPopStart_{};
	Vector2 HumanPopStop_{};
	Vector2 HumanFloatStart_{};
	Vector2 HumanFloatStop_{};

	Vector2 evaluationStartTPos_{};
	Vector2 evaluationStopTPos_{};
	Vector2 evaluationStartBPos_{};
	Vector2 evaluationStopBPos_{};
};
