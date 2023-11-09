#pragma once
// コライダー

#include "../Player/Player.h"
#include "../FloorManager/FloorManager.h"
#include"../BoxManager/BoxManager.h"
#include "../Goal/Goal.h"
#include "../Enemy/Enemy.h"

class CollisionManager
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Player* player, FloorManager* floorManager, BoxManager* boxManager/*, Goal* goal, Enemy* enemy*/);

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
	/*Goal* goal_ = nullptr;
	Enemy* enemy_ = nullptr;*/

};
