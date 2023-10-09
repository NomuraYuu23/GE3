#pragma once
#include "../../ViewProjection.h"
#include "../../WorldTransform.h"

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
	/// 追従対象セッター
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(const WorldTransform* target) { target_ = target; }

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

private:

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//追従対象
	const WorldTransform* target_ = nullptr;

};

