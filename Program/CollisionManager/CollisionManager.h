#pragma once

#include "../Player/Player.h"
#include "../FloorManager/FloorManager.h"
#include "../Goal/Goal.h"

class CollisionManager
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Player* player, FloorManager* floorManager, Goal* goal);

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

private: // メンバ変数

	// ゲームオブジェクト
	Player* player_ = nullptr;

	FloorManager* floorManager_ = nullptr;

	Goal* goal_ = nullptr;

};
