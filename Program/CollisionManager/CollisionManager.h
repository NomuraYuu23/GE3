#pragma once
// コライダー
#include "../Sphere/Sphere.h"
#include "../Plane/Plane.h"

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
	void AllCollision();

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

	/// <summary>
	/// 球と球
	/// </summary>
	/// <param name="s1"></param>
	/// <param name="s2"></param>
	/// <returns></returns>
	bool IsCollision(Sphere& s1, Sphere& s2);

	/// <summary>
	/// 球と平面
	/// </summary>
	/// <param name="sphere"></param>
	/// <param name="plane"></param>
	/// <returns></returns>
	bool IsCollision(Sphere& sphere, Plane& plane);

private: // メンバ変数

	// 計算
	Vector3Calc* v3Calc;
	Matrix4x4Calc* m4Calc;

	// ゲームオブジェクト
	Player* player_ = nullptr;
	FloorManager* floorManager_ = nullptr;
	Goal* goal_ = nullptr;
	Enemy* enemy_ = nullptr;

};
