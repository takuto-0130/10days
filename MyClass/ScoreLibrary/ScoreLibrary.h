#pragma once
#include "Struct.h"
class ScoreLibrary
{
public:
	ScoreLibrary();
	~ScoreLibrary();

	void ApplyGlobalVariables();

	void Update(const int& score);

	void UpdateLibrary(const int& score);

	void SetStage(Stage* stage) { stage_ = stage; }

private:
	int stageScore_[12] = {};
	Stage* stage_ = nullptr;
};

