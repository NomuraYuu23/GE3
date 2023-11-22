#pragma once
// コライダー

#include "../Player/Player.h"
#include "../FloorManager/FloorManager.h"
#include "../Goal/Goal.h"
#include "../EnemyManager/EnemyManager.h"

class CollisionManager
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Player* player, FloorManager* floorManager, Goal* goal, EnemyManager* enemyManager);

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
	Goal* goal_ = nullptr;
	EnemyManager* enemyManager_ = nullptr;

};
