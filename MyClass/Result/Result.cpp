#include "Result.h"
#include <time.h>
#include <stdio.h>

Result::Result()
{
	unsigned int currenttime = (unsigned int)time(nullptr);
	srand(currenttime);


	bgTexture_ = Novice::LoadTexture("./Resources/backGround.png");
	starTexture_ = Novice::LoadTexture("./Resources/StageSelect/flowerYellow.png");
	uiTexture_[0] = Novice::LoadTexture("./Resources/result/next.png");
	uiTexture_[1] = Novice::LoadTexture("./Resources/result/title.png");
	forUITexture_[0] = Novice::LoadTexture("./Resources/result/for_next.png");
	forUITexture_[1] = Novice::LoadTexture("./Resources/result/for_title.png");
	buttonTexture_ = Novice::LoadTexture("./Resources/StageSelect/A.png");
	underBarTexture_ = Novice::LoadTexture("./Resources/white1x1.png");
	numberTextureYellow_ = Novice::LoadTexture("./Resources/number/namber_yellow.png");
	numberTextureRed_= Novice::LoadTexture("./Resources/number/namber_red.png");
	stageTexture_ = Novice::LoadTexture("./Resources/result/stage.png");
	humanTexture_ = Novice::LoadTexture("./Resources/result/backGround_player.png");
	evaluationTexture_[0] = Novice::LoadTexture("./Resources/result/nice.png");
	evaluationTexture_[1] = Novice::LoadTexture("./Resources/result/omigoto.png");
	evaluationTexture_[2] = Novice::LoadTexture("./Resources/result/saikou.png");

	Initialize();

}

Result::~Result(){}

void Result::Initialize()
{


	bg_.center = { 640.0f,360.0f };
	bg_.rad = { 1280.0f,720.0f };
	QuadVer(bg_.center, bg_.rad.x, bg_.rad.y, bg_.LT, bg_.RT, bg_.LB, bg_.RB);

	star_.center = { 50.0f,50.0f };
	star_.rad = { 64.0f,64.0f };
	QuadVer(star_.center, star_.rad.x, star_.rad.y, star_.LT, star_.RT, star_.LB, star_.RB);

	ui_[0].center = { 1000.0f,500.0f };
	ui_[0].rad = { 400.0f,120.0f };
	QuadVer(ui_[0].center, ui_[0].rad.x, ui_[0].rad.y, ui_[0].LT, ui_[0].RT, ui_[0].LB, ui_[0].RB);

	ui_[1].center = { 1000.0f,600.0f };
	ui_[1].rad = { 400.0f,120.0f };
	QuadVer(ui_[1].center, ui_[1].rad.x, ui_[1].rad.y, ui_[1].LT, ui_[1].RT, ui_[1].LB, ui_[1].RB);

	button_.center = { ui_[0].RT};
	button_.rad = { 66.0f,66.0f };
	QuadVer(button_.center, button_.rad.x, button_.rad.y, button_.LT, button_.RT, button_.LB, button_.RB);

	underBar_.center = { 640.0f,250.0f };
	underBar_.rad = { 500.0f,10.0f };
	QuadVer(underBar_.center, underBar_.rad.x, underBar_.rad.y, underBar_.LT, underBar_.RT, underBar_.LB, underBar_.RB);

	stageNum_.center = { 55,40 };
	stageNum_.rad = { 64,64 };
	QuadVer(stageNum_.center, stageNum_.rad.x, stageNum_.rad.y, stageNum_.LT, stageNum_.RT, stageNum_.LB, stageNum_.RB);

	stage_.center = { 110.0f,80.0f };
	stage_.rad = { 220.0f,160.0f };
	QuadVer(stage_.center, stage_.rad.x, stage_.rad.y, stage_.LT, stage_.RT, stage_.LB, stage_.RB);

	human_.center = { 320.0f, 1080.0f };
	human_.rad = { 640.0f,360.0f };
	QuadVer(human_.center, human_.rad.x, human_.rad.y, human_.LT, human_.RT, human_.LB, human_.RB);

	evaluation_.center = { 640.0f,300.0f };
	evaluation_.rad = { 500.0f,150.0f };
	QuadVer(evaluation_.center, evaluation_.rad.x, evaluation_.rad.y, evaluation_.LT, evaluation_.RT, evaluation_.LB, evaluation_.RB);


	isStarDraw_ = true;
	isEvaluationDraw_ = false;
	isHumanDraw_ = false;
	isPopHuman_ = false;
	isScoreDrawComp0_ = false;
	isScoreDrawComp1_= false;
	isScoreDrawComp2_ = false;
	isScoreDrawComp3_ = false;
	isScoreDrawComp4_ = false;

	changeNext_ = 1;

	starTime_ = kStarTime;
	buttonTime_ = 0;
	buttonCount_ = 0.05f;
	humanCount = 0.02f;
	humanPopTime_ = 0;
	humanFloatTime_ = 0;
	evaluationTime_ = kEvaluationTime;
	evaluationEasTime_ = 0;
	scoreCT_ = kScoreCT;

	buttonStartRad_ = { 66.0f,66.0f };
	buttonStopRad_ = { 76.0f,76.0f };

	HumanPopStart_ = { 320.0f,1080.0f };
	HumanPopStop_ = { 320.0f,640.0f };
	HumanFloatStart_ = { 320.0f,640.0f };
	HumanFloatStop_ = { 320.0f,590.0f };

	evaluationStartTPos_ = evaluation_.LT;
	evaluationStopTPos_ = { 800.0f,evaluation_.LT.y };
	evaluationStartBPos_ = evaluation_.LB;
	evaluationStopBPos_ = { 800.0f,evaluation_.LB.y };

	evaluationPoint_ = 0;

	stageNumber_ = 0;
}

void Result::Update(const char* keys, const char* preKeys)
{
	if (keys[(DIK_W)] && !preKeys[(DIK_W)])
	{
		ChangeNext();
	}
	if (keys[(DIK_S)] && !preKeys[(DIK_S)])
	{
		ChangeNext();
	}

	if (scoreCT_ >= 0)
	{
		scoreCT_ -= kTimeCount;
	}

	StarUpdate();
	ScoreUpdate();
	UIUpdate();
	HumanUpdate();
}

void Result::Draw()
{
	Novice::DrawQuad((int)bg_.LT.x, (int)bg_.LT.y,
		(int)bg_.RT.x, (int)bg_.RT.y,
		(int)bg_.LB.x, (int)bg_.LB.y,
		(int)bg_.RB.x, (int)bg_.RB.y,
		0, 0, (int)bg_.rad.x, (int)bg_.rad.y,
		bgTexture_, 0x191970ff);

	if (isStarDraw_)
	{
		Novice::DrawQuad((int)star_.LT.x, (int)star_.LT.y,
			(int)star_.RT.x, (int)star_.RT.y,
			(int)star_.LB.x, (int)star_.LB.y,
			(int)star_.RB.x, (int)star_.RB.y,
			0, 0, 64, 64,
			starTexture_, WHITE);
	}

	for (int i = 0; i < 2; i++)
	{
		Novice::DrawQuad((int)ui_[i].LT.x, (int)ui_[i].LT.y,
			(int)ui_[i].RT.x, (int)ui_[i].RT.y,
			(int)ui_[i].LB.x, (int)ui_[i].LB.y,
			(int)ui_[i].RB.x, (int)ui_[i].RB.y,
			0, 0, (int)uiTextureRad_[i].x, (int)uiTextureRad_[i].y,
			nextUITex_[i], WHITE);
	}

	Novice::DrawQuad((int)button_.LT.x, (int)button_.LT.y,
		(int)button_.RT.x, (int)button_.RT.y,
		(int)button_.LB.x, (int)button_.LB.y,
		(int)button_.RB.x, (int)button_.RB.y,
		0, 0, 66, 66,
		buttonTexture_, 0xffd700ff);

	Novice::DrawQuad((int)underBar_.LT.x, (int)underBar_.LT.y,
		(int)underBar_.RT.x, (int)underBar_.RT.y,
		(int)underBar_.LB.x, (int)underBar_.LB.y,
		(int)underBar_.RB.x, (int)underBar_.RB.y,
		0, 0, 66, 66,
		underBarTexture_, WHITE);


	Novice::DrawQuad((int)number_[0].LT.x, (int)number_[0].LT.y,
		(int)number_[0].RT.x, (int)number_[0].RT.y,
		(int)number_[0].LB.x, (int)number_[0].LB.y,
		(int)number_[0].RB.x, (int)number_[0].RB.y,
		numberSize_ * num_[0], 0, numberSize_, numberSize_,
		numberTextureYellow_, WHITE);

	Novice::DrawQuad((int)number_[1].LT.x, (int)number_[1].LT.y,
		(int)number_[1].RT.x, (int)number_[1].RT.y,
		(int)number_[1].LB.x, (int)number_[1].LB.y,
		(int)number_[1].RB.x, (int)number_[1].RB.y,
		numberSize_ * num_[1], 0, numberSize_, numberSize_,
		numberTextureYellow_, WHITE);

	Novice::DrawQuad((int)number_[2].LT.x, (int)number_[2].LT.y,
		(int)number_[2].RT.x, (int)number_[2].RT.y,
		(int)number_[2].LB.x, (int)number_[2].LB.y,
		(int)number_[2].RB.x, (int)number_[2].RB.y,
		numberSize_ * num_[2], 0, numberSize_, numberSize_,
		numberTextureYellow_, WHITE);

	Novice::DrawQuad((int)number_[3].LT.x, (int)number_[3].LT.y,
		(int)number_[3].RT.x, (int)number_[3].RT.y,
		(int)number_[3].LB.x, (int)number_[3].LB.y,
		(int)number_[3].RB.x, (int)number_[3].RB.y,
		numberSize_ * num_[3], 0, numberSize_, numberSize_,
		numberTextureYellow_, WHITE);

	Novice::DrawQuad((int)stageNum_.LT.x, (int)stageNum_.LT.y,
		(int)stageNum_.RT.x, (int)stageNum_.RT.y,
		(int)stageNum_.LB.x, (int)stageNum_.LB.y,
		(int)stageNum_.RB.x, (int)stageNum_.RB.y,
		numberSize_ * stageNumber_, 0, numberSize_, numberSize_,
		numberTextureRed_, WHITE);

	Novice::DrawQuad((int)stage_.LT.x, (int)stage_.LT.y,
		(int)stage_.RT.x, (int)stage_.RT.y,
		(int)stage_.LB.x, (int)stage_.LB.y,
		(int)stage_.RB.x, (int)stage_.RB.y,
		0, 0, (int)stage_.rad.x, (int)stage_.rad.y,
		stageTexture_, WHITE);

	if (isPopHuman_)
	{
		Novice::DrawQuad((int)human_.LT.x, (int)human_.LT.y,
			(int)human_.RT.x, (int)human_.RT.y,
			(int)human_.LB.x, (int)human_.LB.y,
			(int)human_.RB.x, (int)human_.RB.y,
			0, 0, (int)human_.rad.x, (int)human_.rad.y,
			humanTexture_, WHITE);
	}

	if (evaluationTime_==0)
	{
		Novice::DrawQuad((int)evaluation_.LT.x, (int)evaluation_.LT.y,
			(int)evaluation_.RT.x, (int)evaluation_.RT.y,
			(int)evaluation_.LB.x, (int)evaluation_.LB.y,
			(int)evaluation_.RB.x, (int)evaluation_.RB.y,
			0, 0, (int)evaluation_.rad.x, (int)evaluation_.rad.y,
			evaluationTex_, WHITE);
	}
}

void Result::StarUpdate()
{
	if (starTime_ >= 0)
	{
		starTime_ -= kTimeCount;
	}
	else if (isStarDraw_)
	{
		isStarDraw_ = false;
		starTime_ = kStarEraseTime;
	}
	else if (!isStarDraw_)
	{
		isStarDraw_ = true;
		starTime_ = kStarTime;
	}
}

void Result::ScoreUpdate()
{
	if (scoreCT_ <= 0)
	{
		if (!isScoreDrawComp0_)
		{
			isScoreDrawComp0_ = true;
			scoreCT_ = kScoreCT;
		}
		else if (!isScoreDrawComp1_)
		{
			isScoreDrawComp1_ = true;
			scoreCT_ = kScoreCT;
		}
		else if (!isScoreDrawComp2_)
		{
			isScoreDrawComp2_ = true;
			scoreCT_ = kScoreCT;
		}
		else if (!isScoreDrawComp3_)
		{
			isScoreDrawComp3_ = true;
			scoreCT_ = kScoreCT;
		}

	}

	if (!isScoreDrawComp0_)
	{
		num_[0] = rand() % 10;
	}
	else
	{
		num_[0] = score_ % 10;
		score2_ = (score_ - num_[0]) / 10;
	}

	if (!isScoreDrawComp1_)
	{
		num_[1] = rand() % 10;
	}
	else
	{
		num_[1] = score2_ % 10;
		score3_ = (score2_ - num_[1]) / 10;
	}

	if (!isScoreDrawComp2_)
	{
		num_[2] = rand() % 10;
	}
	else
	{
		num_[2] = score3_ % 10;
		score4_ = (score3_ - num_[2]) / 10;
	}

	if (!isScoreDrawComp3_)
	{
		num_[3] = rand() % 10;
	}
	else
	{
		num_[3] = score4_ % 10;
		isScoreDrawComp4_ = true;
	}

	number_[0].center = { 750,180 };
	number_[0].rad = { 64,64 };
	
	number_[1].center = { number_[0].center.x - 70 ,number_[0].center.y };
	number_[1].rad = number_[0].rad;

	number_[2].center = { number_[1].center.x - 70 ,number_[0].center.y };
	number_[2].rad = number_[0].rad;

	number_[3].center = { number_[2].center.x - 70 ,number_[0].center.y };
	number_[3].rad = number_[0].rad;

	for (int i = 0; i < 4; i++)
	{
		QuadVer(number_[i].center, number_[i].rad.x, number_[i].rad.y, number_[i].LT, number_[i].RT, number_[i].LB, number_[i].RB);
	}

	/////////////////////////////////////////////////////////// 
	if (isScoreDrawComp4_ && evaluationTime_ != 0)
	{
		evaluationTime_ -= kTimeCount;
	}

	if (evaluationTime_ == 0)
	{
		evaluationEasTime_ += kEvaluationCount;

		evaluation_.RT = Lerp(evaluationStartTPos_, evaluationStopTPos_, evaluationEasTime_);
		evaluation_.RB = Lerp(evaluationStartBPos_, evaluationStopBPos_, evaluationEasTime_);
	}
	if (evaluationEasTime_ >= 1.0f)
	{
		//evaluationEasTime_ = 0;
		isPopHuman_ = true;
	}

	if (evaluationPoint_ == 0) 
	{
		evaluationTex_ = evaluationTexture_[0];
	}
	else if (evaluationPoint_ == 1)
	{
		evaluationTex_ = evaluationTexture_[1];
	}
	else if (evaluationPoint_ == 2)
	{
		evaluationTex_ = evaluationTexture_[2];
	}

	QuadVer(evaluation_.center, evaluation_.rad.x, evaluation_.rad.y, evaluation_.LT, evaluation_.RT, evaluation_.LB, evaluation_.RB);
}

void Result::UIUpdate()
{
	if (changeNext_ == 1)
	{
		nextUITex_[0] = forUITexture_[0];
		ui_[0].rad = { 400.0f,120.0f };

		nextUITex_[1] = uiTexture_[1];
		ui_[1].rad = { 280.0f,92.0f };

		uiTextureRad_[0] = { 500.0f,150.0f };
		uiTextureRad_[1] = { 345.0f,112.0f };

		button_.center = { ui_[0].RT };
	}

	if (changeNext_ == 2)
	{
		nextUITex_[0] = uiTexture_[0];
		ui_[0].rad = { 280.0f,92.0f };

		nextUITex_[1] = forUITexture_[1];
		ui_[1].rad = { 400.0f,120.0f };

		uiTextureRad_[0] = { 345.0f,112.0f };
		uiTextureRad_[1] = { 500.0f, 150.0f };

		button_.center = { ui_[1].RT };
	}

	QuadVer(ui_[0].center, ui_[0].rad.x, ui_[0].rad.y, ui_[0].LT, ui_[0].RT, ui_[0].LB, ui_[0].RB);
	QuadVer(ui_[1].center, ui_[1].rad.x, ui_[1].rad.y, ui_[1].LT, ui_[1].RT, ui_[1].LB, ui_[1].RB);
	QuadVer(button_.center, button_.rad.x, button_.rad.y, button_.LT, button_.RT, button_.LB, button_.RB);
	///////////////////////////////////////////////
	buttonTime_ += buttonCount_;
	if (buttonTime_ > 1)
	{
		buttonCount_ *= -1;
	}
	else if (buttonTime_ < 0)
	{
		buttonCount_ *= -1;
	}

	button_.rad = Lerp(buttonStartRad_, buttonStopRad_, buttonTime_);

	QuadVer(button_.center, button_.rad.x, button_.rad.y, button_.LT, button_.RT, button_.LB, button_.RB);

}

void Result::HumanUpdate()
{
	if (humanFloatTime_ > 1)
	{
		humanCount *= -1;
	}
	else if (humanFloatTime_ < 0)
	{
		humanCount *= -1;
	}

	if (isPopHuman_)
	{
		if (humanPopTime_ <= 1.0f)
		{
			humanPopTime_ += kHumanCount;
			human_.center = Lerp(HumanPopStart_, HumanPopStop_, humanPopTime_);
		}
		else
		{
			humanFloatTime_ += humanCount;
			human_.center = Lerp(HumanFloatStart_, HumanFloatStop_, humanFloatTime_);
		}
	}


	QuadVer(human_.center, human_.rad.x, human_.rad.y, human_.LT, human_.RT, human_.LB, human_.RB);
}

void Result::ChangeNext()
{
	if (changeNext_ == 1)
	{
		changeNext_ = 2;
	}
	else if (changeNext_ == 2)
	{
		changeNext_ = 1;
	}
}

Vector2 Result::Lerp(const Vector2& v1, const Vector2& v2, float t)
{
	Vector2 v = { v2.x - v1.x,v2.y - v1.y };

	v = { v.x * t,v.y * t };

	Vector2 result = { v1.x + v.x,v1.y + v.y };

	return result;
}

void Result::QuadVer(Vector2 pos, float width, float height, Vector2& lt, Vector2& rt, Vector2& lb, Vector2& rb)
{
	float halfwidth = width / 2;
	float halfheight = height / 2;

	lt = { pos.x - halfwidth, pos.y - halfheight };
	rt = { pos.x + halfwidth, pos.y - halfheight };
	lb = { pos.x - halfwidth, pos.y + halfheight };
	rb = { pos.x + halfwidth, pos.y + halfheight };
};
