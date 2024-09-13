#include "Select.h"
#include "MyClass/GlobalVariables/GlobalVariables.h"
#include <Audio.h>
#include "Input.h"
#include "Function.h"

Select::~Select(){}

void Select::Initialize()
{
	bgTexture_ = Novice::LoadTexture("./Resources/white1x1.png"); // 0x191970ff
	starTexture_ = Novice::LoadTexture("./Resources/StageSelect/flowerYellow.png");

	stageTexture_[0] = Novice::LoadTexture("./Resources/white1x1.png"); // 0x4169e1ff
	stageTexture_[1] = Novice::LoadTexture("./Resources/white1x1.png"); // 0xff4500ff
	stageTexture_[2] = Novice::LoadTexture("./Resources/white1x1.png"); // 0x4169e1ff
	stageTexture_[3] = Novice::LoadTexture("./Resources/white1x1.png"); // 0xff4500ff
	stageTexture_[4] = Novice::LoadTexture("./Resources/white1x1.png"); // 0x4169e1ff

	arrowTexture_[0] = Novice::LoadTexture("./Resources/StageSelect/arrow.png");
	arrowTexture_[1] = Novice::LoadTexture("./Resources/StageSelect/arrow.png");
	buttonTexture_ = Novice::LoadTexture("./Resources/StageSelect/A.png"); 
	uiTexture_= Novice::LoadTexture("./Resources/StageSelect/titleText.png");
	humanTexture_ = Novice::LoadTexture("./Resources/StageSelect/player.png");
	numberTexture_ = Novice::LoadTexture("./Resources/number/namber_yellow.png");

	bg_.center = { 640.0f,360.0f };
	bg_.rad = { 1280.0f,720.0f };
	QuadVer(bg_.center, bg_.rad.x, bg_.rad.y, bg_.LT, bg_.RT, bg_.LB, bg_.RB);
	

	star_.center = { 50.0f,50.0f };
	star_.rad = { 64.0f,64.0f };
	QuadVer(star_.center, star_.rad.x, star_.rad.y, star_.LT, star_.RT, star_.LB, star_.RB);
	

	stage_[0].center = { 640.0f,360.0f };
	stage_[0].rad = { 980.0f,420.0f };
	QuadVer(stage_[0].center, stage_[0].rad.x, stage_[0].rad.y, stage_[0].LT, stage_[0].RT, stage_[0].LB, stage_[0].RB);
	
	stage_[1].center = { 640.0f,750.0f };
	stage_[1].rad = { 588.0f,216.0f };
	QuadVer(stage_[1].center, stage_[1].rad.x, stage_[1].rad.y, stage_[1].LT, stage_[1].RT, stage_[1].LB, stage_[1].RB);


	arrow_[0].center = {640.0f,620.0f};
	arrow_[0].rad = {64.0f,64.0f};
	QuadVer(arrow_[0].center, arrow_[0].rad.x, arrow_[0].rad.y, arrow_[0].LT, arrow_[0].RT, arrow_[0].LB, arrow_[0].RB);

	
	arrow_[1].center = { 640.0f,100.0f };
	arrow_[1].rad = { 64.0f,64.0f };
	QuadVer(arrow_[1].center, arrow_[1].rad.x, arrow_[1].rad.y, arrow_[1].LT, arrow_[1].RT, arrow_[1].LB, arrow_[1].RB);


	button_.center = { 700.0f,500.0f };
	button_.rad = { 66.0f,66.0f };
	QuadVer(button_.center, button_.rad.x, button_.rad.y, button_.LT, button_.RT, button_.LB, button_.RB);

	ui_.center = { 160.0f,650.0f };
	ui_.rad = { 350.0f,70.0f };
	QuadVer(ui_.center, ui_.rad.x, ui_.rad.y, ui_.LT, ui_.RT, ui_.LB, ui_.RB);

	
	buttonTime_ = kButtonTime;
	starTime_ = kStarTime;
	stageChangeInterval_ = kStageChangeTime;
	stageChangeTime_ = 0;
	arrowTime_ = 0;
	humanPopTime_ = kHumanPopTime_;

	isStarDraw_ = true;

	stageNum_ = 1;

	topPos_ = { 640.0f,-30.0f };
	nowPos_ = { 640,360 };
	underPos_ = { 640,750 };
	mainRad_= { 980.0f,420.0f };
	subRad_ = { 588.0f,216.0f };

	arrowStartPos_[0] = {640.0f,620.0f};
	arrowStartPos_[1] = { 640.0f,100.0f };
	arrowStopPos_[0] = { 640.0f,650.0f };
	arrowStopPos_[1]= { 640.0f,70.0f };

	buttonStartRad_= { 66.0f,66.0f };
	buttonStopRad_ = { 76.0f,76.0f };

	isPoyon_ = false;
	isPoyonChange_ = false;
	isPopHuman_ = false;

	scorePos_ = { 350,100 };
	Audio* audio = Audio::GetInstance();
	SE_scroll = audio->LoadWave("./Resources/Sound/SE_scroll.mp3");
}

void Select::Update(const char* keys, const char* preKeys)
{
	XINPUT_STATE joyState;
	Vector2 move{ 0, 0 };
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		const float threshold = 0.7f;
		bool isMoving = false;
		move = { float(joyState.Gamepad.sThumbLX) / SHRT_MAX, float(joyState.Gamepad.sThumbLY) / SHRT_MAX };
		if (Length(move) > threshold) {
			isMoving = true;
		}
		if (isMoving) {
			move.y += float(joyState.Gamepad.sThumbLY) / SHRT_MAX;
		}
		if (move.y < 0 && beforeY == 0) {
			PlusStageNum();
		}
		if (move.y > 0 && beforeY == 0) {
			MinasStageNum();
		}
		beforeY = move.y;
	}
	else {
		if (keys[DIK_W] && !preKeys[DIK_W])
		{
			MinasStageNum();
		}
		if (keys[DIK_S] && !preKeys[DIK_S])
		{
			PlusStageNum();
		}
	}
	if (stageChangeInterval_ > 0)
	{
		stageChangeInterval_ -= kTimeCount;
	}

	if (changeStage == 1 || changeStage == -1)
	{
		StageSelectUpdate();

		if (stageChangeTime_ < 1)
		{
			stageChangeTime_ += kStageChangeCo;
		}
		else
		{
			stageChangeTime_ = 0;
			changeStage = 0;
		}
	}

	StarUpdate();

	arrowTime_ += arrowTimeCo;
	if (arrowTime_ > 1)
	{
		arrowTimeCo *= -1;
	}
	else if(arrowTime_ < 0)
	{
		arrowTimeCo *= -1;
	}
	

	ArrowUpdate();

	if (buttonTime_ > 0)
	{
		buttonTime_ -= kTimeCount;
	}
	else
	{
		isPoyon_ = true;
	}

	if (isPoyon_)
	{
		poyonTime_ += kButtonCo_;


		ButtonUpdate();
	}

	for (int i = 0; i < kStageNum; i++)
	{
		QuadVer(stage_[i].center, stage_[i].rad.x, stage_[i].rad.y, stage_[i].LT, stage_[i].RT, stage_[i].LB, stage_[i].RB);
	}


	arrow_[0].rad = { 64.0f,64.0f };

	for (int i = 0; i < 2; i++)
	{
		QuadVer(arrow_[i].center, arrow_[i].rad.x, arrow_[i].rad.y, arrow_[i].LT, arrow_[i].RT, arrow_[i].LB, arrow_[i].RB);
	}

	
	if (!isPopHuman_)
	{
		humanPopTime_ -= kTimeCount;
		if (humanPopTime_ <= 0)
		{
			PopHuman();
			isPopHuman_ = true;
			humanPopTime_ = kHumanPopTime_;
		}
	}

	if (isPopHuman_)
	{
		HumanUpdate();
	}

	HighScoreUpdate();
}

void Select::Draw()
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
			0, 0, 64,64,
			starTexture_, 0xffd700ff);
	}

	if (isPopHuman_)
	{
		/*Novice::DrawQuad(int(rotateLeftTop.x), int(rotateLeftTop.y),
			int(rotateRightTop.x), int(rotateRightTop.y),
			int(rotateLeftBottom.x), int(rotateLeftBottom.y),
			int(rotateRightBottom.x), int(rotateRightBottom.y),
			0, 0, int(human_.rad.x), int(human_.rad.y),
			humanTexture_, WHITE);*/

		Novice::DrawQuad(int(human_.LT.x), int(human_.LT.y),
			int(human_.RT.x), int(human_.RT.y),
			int(human_.LB.x), int(human_.LB.y),
			int(human_.RB.x), int(human_.RB.y),
			0, 0, int(human_.rad.x), int(human_.rad.y),
			humanTexture_, WHITE);
	}

	Novice::DrawQuad((int)stage_[0].LT.x, (int)stage_[0].LT.y,
		(int)stage_[0].RT.x, (int)stage_[0].RT.y,
		(int)stage_[0].LB.x, (int)stage_[0].LB.y,
		(int)stage_[0].RB.x, (int)stage_[0].RB.y,
		0, 0, (int)stage_[0].rad.x, (int)stage_[0].rad.y,
		stageTexture_[0], 0x000000ff);

	Novice::DrawQuad((int)stage_[1].LT.x, (int)stage_[1].LT.y,
		(int)stage_[1].RT.x, (int)stage_[1].RT.y,
		(int)stage_[1].LB.x, (int)stage_[1].LB.y,
		(int)stage_[1].RB.x, (int)stage_[1].RB.y,
		0, 0, (int)stage_[1].rad.x, (int)stage_[1].rad.y,
		stageTexture_[1], 0xa9a9a9ff);

	Novice::DrawQuad((int)stage_[2].LT.x, (int)stage_[2].LT.y,
		(int)stage_[2].RT.x, (int)stage_[2].RT.y,
		(int)stage_[2].LB.x, (int)stage_[2].LB.y,
		(int)stage_[2].RB.x, (int)stage_[2].RB.y,
		0, 0, (int)stage_[2].rad.x, (int)stage_[2].rad.y,
		stageTexture_[2], 0xd3d3d3ff);

	Novice::DrawQuad((int)stage_[3].LT.x, (int)stage_[3].LT.y,
		(int)stage_[3].RT.x, (int)stage_[3].RT.y,
		(int)stage_[3].LB.x, (int)stage_[3].LB.y,
		(int)stage_[3].RB.x, (int)stage_[3].RB.y,
		0, 0, (int)stage_[3].rad.x, (int)stage_[3].rad.y,
		stageTexture_[3], 0xf5f5f5ff);

	Novice::DrawQuad((int)stage_[4].LT.x, (int)stage_[4].LT.y,
		(int)stage_[4].RT.x, (int)stage_[4].RT.y,
		(int)stage_[4].LB.x, (int)stage_[4].LB.y,
		(int)stage_[4].RB.x, (int)stage_[4].RB.y,
		0, 0, (int)stage_[4].rad.x, (int)stage_[4].rad.y,
		stageTexture_[4], 0xfffafaff);



	if (stageNum_ != 5)
	{
		Novice::DrawQuad((int)arrow_[0].LB.x, (int)arrow_[0].LB.y,
			(int)arrow_[0].RB.x, (int)arrow_[0].RB.y,
			(int)arrow_[0].LT.x, (int)arrow_[0].LT.y,
			(int)arrow_[0].RT.x, (int)arrow_[0].RT.y,
			0, 0, (int)arrow_[0].rad.x, (int)arrow_[0].rad.y,
			arrowTexture_[0], WHITE);
	}
	if (stageNum_ != 1)
	{
		Novice::DrawQuad((int)arrow_[1].LT.x, (int)arrow_[1].LT.y,
			(int)arrow_[1].RT.x, (int)arrow_[1].RT.y,
			(int)arrow_[1].LB.x, (int)arrow_[1].LB.y,
			(int)arrow_[1].RB.x, (int)arrow_[1].RB.y,
			0, 0, (int)arrow_[1].rad.x, (int)arrow_[1].rad.y,
			arrowTexture_[1], WHITE);
	}

	for (int j = 0; j < 5; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			if (j == stageNum_ or j==stageNum_ - 1)
			{
				Novice::DrawQuad((int)number_[i][j].LT.x, (int)number_[i][j].LT.y,
					(int)number_[i][j].RT.x, (int)number_[i][j].RT.y,
					(int)number_[i][j].LB.x, (int)number_[i][j].LB.y,
					(int)number_[i][j].RB.x, (int)number_[i][j].RB.y,
					numberSize_ * num_[i][j], 0, numberSize_, numberSize_,
					numberTexture_, WHITE);
			}
		}
	}

	Novice::DrawQuad((int)button_.LT.x, (int)button_.LT.y,
		(int)button_.RT.x, (int)button_.RT.y,
		(int)button_.LB.x, (int)button_.LB.y,
		(int)button_.RB.x, (int)button_.RB.y,
		0, 0, 66, 66,
		buttonTexture_, WHITE);

	Novice::DrawQuad((int)ui_.LT.x, (int)ui_.LT.y,
		(int)ui_.RT.x, (int)ui_.RT.y,
		(int)ui_.LB.x, (int)ui_.LB.y,
		(int)ui_.RB.x, (int)ui_.RB.y,
		0, 0, (int)ui_.rad.x, (int)ui_.rad.y,
		uiTexture_, WHITE);

}

void Select::SetHighScore()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "HistoryScore";
	highScore_[0] = globalVariables->GetIntValue(groupName, "Stage1");
	highScore_[1] = globalVariables->GetIntValue(groupName, "Stage2");
	highScore_[2] = globalVariables->GetIntValue(groupName, "Stage3");
	highScore_[3] = globalVariables->GetIntValue(groupName, "Stage4");
	highScore_[4] = globalVariables->GetIntValue(groupName, "Stage5");
}

void Select::PlusStageNum()
{
	if (stageChangeInterval_ == 0 && stageNum_ != kStageNum)
	{
		stageNum_ += 1;
		stageChangeInterval_ = kStageChangeTime;
		changeStage = 1;
		Audio::GetInstance()->PlayWave(SE_scroll);
	}
}

void Select::MinasStageNum()
{
	if (stageChangeInterval_ == 0 && stageNum_ != 1)
	{
		stageNum_ -= 1;
		stageChangeInterval_ = kStageChangeTime;
		changeStage = -1;
		Audio::GetInstance()->PlayWave(SE_scroll);
	}
}

void Select::StageSelectUpdate()
{
	if (changeStage == 1)
	{
		stage_[stageNum_ - 2].center = Lerp(nowPos_, topPos_, stageChangeTime_); // now
		stage_[stageNum_ - 2].rad = Lerp(mainRad_, subRad_, stageChangeTime_);
		
		stage_[stageNum_ - 1].center = Lerp(underPos_, nowPos_, stageChangeTime_); // under
		stage_[stageNum_ - 1].rad = Lerp(subRad_, mainRad_, stageChangeTime_);

		stage_[stageNum_].center = underPos_;
		stage_[stageNum_].rad = subRad_;
	}

	if (changeStage == -1)
	{
		stage_[stageNum_ - 2].center = topPos_;
		stage_[stageNum_ - 2].rad = subRad_;

		stage_[stageNum_ - 1].center = Lerp(topPos_, nowPos_, stageChangeTime_); // top
		stage_[stageNum_ - 1].rad = Lerp(subRad_, mainRad_, stageChangeTime_);

		stage_[stageNum_].center = Lerp(nowPos_, underPos_, stageChangeTime_); // now
		stage_[stageNum_].rad = Lerp(mainRad_, subRad_, stageChangeTime_);
	}
}

void Select::StarUpdate()
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

void Select::ArrowUpdate()
{
	arrow_[0].center = Lerp(arrowStartPos_[0], arrowStopPos_[0], arrowTime_);
	arrow_[1].center = Lerp(arrowStartPos_[1], arrowStopPos_[1], arrowTime_);
}

void Select::ButtonUpdate()
{
	if (!isPoyonChange_)
	{
		button_.rad = Lerp(buttonStartRad_, buttonStopRad_, EaseOutElastic(poyonTime_));
	}
	else if (isPoyonChange_)
	{
		button_.rad = Lerp(buttonStopRad_, buttonStartRad_, EaseOutElastic(poyonTime_));
	}

	if (poyonTime_ >= 1.0f)
	{
		if (!isPoyonChange_)
		{
			poyonTime_ = 0;
			isPoyonChange_ = true;
		}
		else
		{
			isPoyon_ = false;
			isPoyonChange_ = false;
			poyonTime_ = 0;
			buttonTime_ = kButtonTime;
		}
	}

	QuadVer(button_.center, button_.rad.x, button_.rad.y, button_.LT, button_.RT, button_.LB, button_.RB);
}

void Select::HighScoreUpdate()
{
	for (int i = 0; i < 5; i++)
	{
		num_[0][i] = highScore_[i] % 10;
		highScore2_[i] = (highScore_[i] - num_[0][i]) / 10;
		num_[1][i] = highScore2_[i] % 10;
		highScore3_[i] = (highScore2_[i] - num_[1][i]) / 10;
		num_[2][i] = highScore3_[i] % 10;
		highScore4_[i] = (highScore3_[i] - num_[2][i]) / 10;
		num_[3][i] = highScore4_[i] % 10;


		number_[0][i].center = {stage_[i].center.x + scorePos_.x,stage_[i].center.y + scorePos_.y};
		number_[0][i].rad = {64,64};

		number_[1][i].center = {number_[0][i].center.x - 70 ,number_[0][i].center.y};
		number_[1][i].rad = number_[0][i].rad;

		number_[2][i].center = {number_[1][i].center.x - 70 ,number_[0][i].center.y};
		number_[2][i].rad = number_[0][i].rad;

		number_[3][i].center = {number_[2][i].center.x - 70 ,number_[0][i].center.y};
		number_[3][i].rad = number_[0][i].rad;
	}

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			QuadVer(number_[j][i].center, number_[j][i].rad.x, number_[j][i].rad.y, number_[j][i].LT, number_[j][i].RT, number_[j][i].LB, number_[j][i].RB);
		}
	}
	
}

void Select::PopHuman()
{
	human_.center.x = float(rand() % 360 + 1550);
	human_.center.y = float(rand() % 500 + 200);

	//human_.center = { 500,500 };

	human_.rad = { 360,210 };

	humanRotate_ = human_;

	QuadVer(human_.center, human_.rad.x, human_.rad.y, human_.LT, human_.RT, human_.LB, human_.RB);

	//QuadVer(human_.center, human_.rad.x, human_.rad.y, humanRotate_.LT, humanRotate_.RT, humanRotate_.LB, humanRotate_.RB);
}

void Select::HumanUpdate()
{

	/*theta_ += 0.03f;

	Matrix2x2 rotataematrix = MakeRotateMatrix(theta_);

	rotateLeftTop = Multiply(humanRotate_.LT, rotataematrix);
	rotateLeftTop.x += human_.center.x;
	rotateLeftTop.y += human_.center.y;

	rotateRightTop = Multiply(humanRotate_.RT, rotataematrix);
	rotateRightTop.x += human_.center.x;
	rotateRightTop.y += human_.center.y;

	rotateLeftBottom = Multiply(humanRotate_.LB, rotataematrix);
	rotateLeftBottom.x += human_.center.x;
	rotateLeftBottom.y += human_.center.y;

	rotateRightBottom = Multiply(humanRotate_.RB, rotataematrix);
	rotateRightBottom.x += human_.center.x;
	rotateRightBottom.y += human_.center.y;*/

	human_.center.x -= 2.0f;

	QuadVer(human_.center, human_.rad.x, human_.rad.y, human_.LT, human_.RT, human_.LB, human_.RB);

	/*if (rotateLeftTop.x <= -400)
	{
		isPopHuman_ = false;
		theta_ = 0;
	}*/

	if (human_.center.x <= -400)
	{
		isPopHuman_ = false;
		theta_ = 0;
	}
}

Vector2 Select::Lerp(const Vector2& v1, const Vector2& v2, float t)
{
	Vector2 v = { v2.x - v1.x,v2.y - v1.y };

	v = { v.x * t,v.y * t };

	Vector2 result = { v1.x + v.x,v1.y + v.y };

	return result;
}

void Select::QuadVer(Vector2 pos, float width, float height, Vector2& lt, Vector2& rt, Vector2& lb, Vector2& rb)
{
	float halfwidth = width / 2;
	float halfheight = height / 2;

	lt = { pos.x - halfwidth, pos.y - halfheight };
	rt = { pos.x + halfwidth, pos.y - halfheight };
	lb = { pos.x - halfwidth, pos.y + halfheight };
	rb = { pos.x + halfwidth, pos.y + halfheight };
};
