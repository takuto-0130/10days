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
	globalVariables->AddItem(groupName, "Stage6", stageScore_[5]);
	globalVariables->AddItem(groupName, "Stage7", stageScore_[6]);
	globalVariables->AddItem(groupName, "Stage8", stageScore_[7]);
	globalVariables->AddItem(groupName, "Stage9", stageScore_[8]);
	globalVariables->AddItem(groupName, "Stage10", stageScore_[9]);
	globalVariables->AddItem(groupName, "Stage11", stageScore_[10]);
	globalVariables->AddItem(groupName, "Stage12", stageScore_[11]);
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
	stageScore_[5] = globalVariables->GetIntValue(groupName, "Stage6");
	stageScore_[6] = globalVariables->GetIntValue(groupName, "Stage7");
	stageScore_[7] = globalVariables->GetIntValue(groupName, "Stage8");
	stageScore_[8] = globalVariables->GetIntValue(groupName, "Stage9");
	stageScore_[9] = globalVariables->GetIntValue(groupName, "Stage10");
	stageScore_[10] = globalVariables->GetIntValue(groupName, "Stage11");
	stageScore_[11] = globalVariables->GetIntValue(groupName, "Stage12");
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
	case Stage::Stage6:
		if (stageScore_[5] < score) {
			globalVariables->SetValue(groupName, "Stage6", score);
		}
		break;
	case Stage::Stage7:
		if (stageScore_[6] < score) {
			globalVariables->SetValue(groupName, "Stage7", score);
		}
		break;
	case Stage::Stage8:
		if (stageScore_[7] < score) {
			globalVariables->SetValue(groupName, "Stage8", score);
		}
		break;
	case Stage::Stage9:
		if (stageScore_[8] < score) {
			globalVariables->SetValue(groupName, "Stage9", score);
		}
		break;
	case Stage::Stage10:
		if (stageScore_[9] < score) {
			globalVariables->SetValue(groupName, "Stage10", score);
		}
		break;
	case Stage::Stage11:
		if (stageScore_[10] < score) {
			globalVariables->SetValue(groupName, "Stage11", score);
		}
		break;
	case Stage::Stage12:
		if (stageScore_[11] < score) {
			globalVariables->SetValue(groupName, "Stage12", score);
		}
		break;
	}
	globalVariables->SaveFile(groupName);
}
