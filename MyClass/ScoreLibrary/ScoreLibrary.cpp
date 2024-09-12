#include "ScoreLibrary.h"
#include "MyClass/GlobalVariables/GlobalVariables.h"

ScoreLibrary::ScoreLibrary()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "HistoryScore";
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	GlobalVariables::GetInstance()->LoadFiles();
	globalVariables->AddItem(groupName, "Stage1", stageScore_[0]);
	globalVariables->AddItem(groupName, "Stage2", stageScore_[1]);
	globalVariables->AddItem(groupName, "Stage3", stageScore_[2]);
	globalVariables->AddItem(groupName, "Stage4", stageScore_[3]);
	globalVariables->AddItem(groupName, "Stage5", stageScore_[4]);
	globalVariables->SaveFile(groupName);
}

ScoreLibrary::~ScoreLibrary()
{
}

void ScoreLibrary::ApplyGlobalVariables()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "HistoryScore";
	stageScore_[0] = globalVariables->GetIntValue(groupName, "Stage1");
	stageScore_[1] = globalVariables->GetIntValue(groupName, "Stage2");
	stageScore_[2] = globalVariables->GetIntValue(groupName, "Stage3");
	stageScore_[3] = globalVariables->GetIntValue(groupName, "Stage4");
	stageScore_[4] = globalVariables->GetIntValue(groupName, "Stage5");
}

void ScoreLibrary::Update(const int& score)
{
	UpdateLibrary(score);
	ApplyGlobalVariables();
}

void ScoreLibrary::UpdateLibrary(const int& score)
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "HistoryScore";
	switch (*stage_) {
	case Stage::Stage1:
		if (stageScore_[0] < score) {
			globalVariables->SetValue(groupName, "Stage1", score);
		}
		break;
	case Stage::Stage2:
		if (stageScore_[1] < score) {
			globalVariables->SetValue(groupName, "Stage2", score);
		}
		break;
	case Stage::Stage3:
		if (stageScore_[2] < score) {
			globalVariables->SetValue(groupName, "Stage3", score);
		}
		break;
	case Stage::Stage4:
		if (stageScore_[3] < score) {
			globalVariables->SetValue(groupName, "Stage4", score);
		}
		break;
	case Stage::Stage5:
		if (stageScore_[4] < score) {
			globalVariables->SetValue(groupName, "Stage5", score);
		}
		break;
	}
	globalVariables->SaveFile(groupName);
}
