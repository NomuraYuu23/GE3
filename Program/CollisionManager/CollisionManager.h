#pragma once
// コライダー

#include "../Player/Player.h"
#include "../FloorManager/FloorManager.h"
#include"../BoxManager/BoxManager.h"
#include"../BreakBoxManager/BreakBoxManager.h"
#include "../Goal/Goal.h"
#include "../EnemyManager/EnemyManager.h"
#include"../RecoveryItemManager/RecoveryItemManager.h"
#include"../CollectibleItemManager/CollectibleItemManager.h"
#include"../CheckPointManager/CheckPointManager.h"
#include"../BurningBoxManager/BurningBoxManager.h"

class CollisionManager
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Player* player, FloorManager* floorManager,
		BoxManager* boxManager, BreakBoxManager* breakBoxManager,
		RecoveryItemManager* recoveryItemManager, EnemyManager* enemyManager,
		CollectibleItemManager* collectibleItemManager, CheckPointManager* checkPointManager//,
		/*BurningBoxManager* burningBoxManager*//*, Goal* goal, Enemy* enemy*/);

	/// <summary>
	/// 衝突
	/// </summary>
	void AllCollision();

private: // メンバ関数

private: // メンバ変数

	// 計算
	Vector3Calc* v3Calc;
	Matrix4x4Calc* m4Calc;

	// ゲームオブジェクト
	Player* player_ = nullptr;
	FloorManager* floorManager_ = nullptr;
	BoxManager* boxManager_ = nullptr;
	BreakBoxManager* breakBoxManager_ = nullptr;
	BurningBoxManager* burningBoxManager_ = nullptr;
	RecoveryItemManager* recoveryItemManager_ = nullptr;
	EnemyManager* enemyManager_ = nullptr;
	CollectibleItemManager* collectibleItemManager_ = nullptr;
	CheckPointManager* checkPointManager_ = nullptr;
	/*Goal* goal_ = nullptr;
	Enemy* enemy_ = nullptr;*/

};
