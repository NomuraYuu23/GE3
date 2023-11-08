#pragma once
#include "../../../Engine/3D/ViewProjection.h"
#include "../../../Engine/3D/WorldTransform.h"

class FollowCamera {

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// 追従対象セッター
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(const WorldTransform* target);

	/// <summary>
	/// ビュープロジェクションゲッター
	/// </summary>
	/// <returns></returns>
	ViewProjection GetViewProjection() { return viewProjection_; }

	/// <summary>
	/// ビュープロジェクションアドレスゲッター
	/// </summary>
	/// <returns></returns>
	ViewProjection* GetViewProjectionAddress() { return &viewProjection_; }

private: // メンバ関数

	// 追従対象からのオフセットを計算する
	Vector3 OffsetCalc() const;

private:

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//追従対象
	const WorldTransform* target_ = nullptr;

	// 追従対象の残像座標
	Vector3 interTarget_ = {};

	float destinationAngleY_ = 0.0f;

	float rate_ = 0.1f;

};

