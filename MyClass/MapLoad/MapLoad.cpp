#include "MapLoad.h"

MapLoad::MapLoad(MapChipNum* map)
{
	SetMap(map);
}

MapLoad::~MapLoad()
{
}

void MapLoad::Initialize()
{
	LoadMap(*map_, "./Resources/csvMap/stage_1.csv");
}

void MapLoad::Update(int stageNum)
{
	StageNumber(stageNum);
	switch (stage_) {
	case Stage::Stage1:
		LoadMap(*map_, "./Resources/csvMap/stage_1.csv");
		break;
	case Stage::Stage2:
		LoadMap(*map_, "./Resources/csvMap/stage_2.csv");
		break;
	case Stage::Stage3:
		LoadMap(*map_, "./Resources/csvMap/stage_3.csv");
		break;
	case Stage::Stage4:
		LoadMap(*map_, "./Resources/csvMap/stage_4.csv");
		break;
	case Stage::Stage5:
		LoadMap(*map_, "./Resources/csvMap/stage_5.csv");
		break;
	case Stage::Stage6:
		LoadMap(*map_, "./Resources/csvMap/stage_6.csv");
		break;
	case Stage::Stage7:
		LoadMap(*map_, "./Resources/csvMap/stage_7.csv");
		break;
	case Stage::Stage8:
		LoadMap(*map_, "./Resources/csvMap/stage_8.csv");
		break;
	case Stage::Stage9:
		LoadMap(*map_, "./Resources/csvMap/stage_9.csv");
		break;
	case Stage::Stage10:
		LoadMap(*map_, "./Resources/csvMap/stage_10.csv");
		break;
	case Stage::Stage11:
		LoadMap(*map_, "./Resources/csvMap/stage_11.csv");
		break;
	case Stage::Stage12:
		LoadMap(*map_, "./Resources/csvMap/stage_12.csv");
		break;
	}
}

void MapLoad::StageNumber(int stageNum)
{
	if (stageNum == 1) {
		stage_ = Stage::Stage1;
	}
	if (stageNum == 2) {
		stage_ = Stage::Stage2;
	}
	if (stageNum == 3) {
		stage_ = Stage::Stage3;
	}
	if (stageNum == 4) {
		stage_ = Stage::Stage4;
	}
	if (stageNum == 5) {
		stage_ = Stage::Stage5;
	}
	if (stageNum == 6) {
		stage_ = Stage::Stage6;
	}
	if (stageNum == 7) {
		stage_ = Stage::Stage7;
	}
	if (stageNum == 8) {
		stage_ = Stage::Stage8;
	}
	if (stageNum == 9) {
		stage_ = Stage::Stage9;
	}
	if (stageNum == 10) {
		stage_ = Stage::Stage10;
	}
	if (stageNum == 11) {
		stage_ = Stage::Stage11;
	}
	if (stageNum == 12) {
		stage_ = Stage::Stage12;
	}
}
