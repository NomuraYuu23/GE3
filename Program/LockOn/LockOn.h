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

private: // メンバ変数

	// ロックオンマーク用スプライト
	std::unique_ptr<Sprite> lockOnMark_;

	// ロックオン対策
	const Enemy* target_ = nullptr;

	// 最小距離
	float minDistance_ = 10.0f;

	// 最大距離
	float maxDistance_ = 30.0f;

	// 角度範囲
	float angleRange_ = 20.0f * 3.14f / 180.0f;

};

