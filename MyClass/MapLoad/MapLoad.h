#pragma once
#include "Struct.h"
#include "Function.h"
#include <memory>

class MapLoad
{
private:
	MapChipNum* map_ = nullptr;	//マップチップのポインタ

	Stage stage_ = Stage::Stage1;



public:
	MapLoad(MapChipNum* map);
	~MapLoad();
	void Initialize();

	void Update(int stageNum);

	/// <summary>
	/// マップチップデータの登録
	/// </summary>
	/// <param name="map">マップチップデータ</param>
	void SetMap(MapChipNum* map) { map_ = map; }

	void StageNumber(int stageNum);

	Stage& GetNowStage() { return stage_; }
};

