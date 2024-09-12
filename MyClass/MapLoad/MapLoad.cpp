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
	LoadMap(*map_, "./Resources/testMap.csv");
}

void MapLoad::Update(int stageNum)
{
	StageNumber(stageNum);
	switch (stage_) {
	case Stage::Stage1:
		LoadMap(*map_, "./Resources/testMap.csv");
		break;
	case Stage::Stage2:
		LoadMap(*map_, "./Resources/testMapLarge.csv");
		break;
	case Stage::Stage3:
		LoadMap(*map_, "./Resources/testMap.csv");
		break;
	case Stage::Stage4:
		LoadMap(*map_, "./Resources/testMapLarge.csv");
		break;
	case Stage::Stage5:
		LoadMap(*map_, "./Resources/testMap.csv");
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
}
