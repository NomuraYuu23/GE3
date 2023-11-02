#pragma once
#include "../../Engine/3D/ViewProjection.h"

class DebugCamera
{

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

	// 移動速度
	float moveSpeed_;
	// 回転速度
	float rotateSpeed_;

};

