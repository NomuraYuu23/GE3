#pragma once

#include "../Player/Player.h"
#include "../FloorManager/FloorManager.h"
#include "../Goal/Goal.h"
#include "../Enemy/Enemy.h"

class CollisionManager
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Player* player, FloorManager* floorManager, Goal* goal, Enemy* enemy);

	/// <summary>
	/// 衝突
	/// </summary>
	void Collision();

private: // メンバ関数

	/// <summary>
	/// プレイヤーと床
	/// </summary>
	bool PlayerAndFloor(Floor* floor);

	/// <summary>
	/// プレイヤーとゴール
	/// </summary>
	/// <returns></returns>
	bool PlayerAndGoal();

	/// <summary>
	/// プレイヤーとエネミー
	/// </summary>
	/// <returns></returns>
	bool PlayerAndEnemy();

private: // メンバ変数

	// ゲームオブジェクト
	Player* player_ = nullptr;

	FloorManager* floorManager_ = nullptr;

	Goal* goal_ = nullptr;

	Enemy* enemy_ = nullptr;

};
