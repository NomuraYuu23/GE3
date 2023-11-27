#pragma once
#include <memory>
#include <list>
#include "../../Engine/2D/Sprite.h"
#include "../Enemy/Enemy.h"
#include "../../Engine/3D/ViewProjection.h"

/// <summary>
/// ロックオン
/// </summary>
class LockOn
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const std::list<Enemy*>& enemies, const ViewProjection& viewProjection);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ロックオン対象の座標取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetTargetPosition() const;

	/// <summary>
	/// ロックオン中か
	/// </summary>
	/// <returns></returns>
	bool ExistTarget() const { return target_ ? true : false; }

private: // メンバ関数

	/// <summary>
	/// 範囲外判定
	/// </summary>
	bool OutOfRangeJudgment(const ViewProjection& viewProjection);

private: // メンバ変数

	// ロックオンマーク用スプライト
	std::unique_ptr<Sprite> lockOnMark_;

	// ロックオン対策
	Enemy* target_ = nullptr;

	// 最小距離
	float minDistance_ = 50.0f;

	// 最大距離
	float maxDistance_ = 80.0f;

	// 角度範囲
	float angleRange_ = 20.0f * 3.14f / 180.0f;

};

