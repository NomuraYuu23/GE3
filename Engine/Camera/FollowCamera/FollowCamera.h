#pragma once
#include "../../../Engine/3D/ViewProjection.h"
#include "../../../Engine/3D/WorldTransform.h"
#include "../BaseCamera.h"

class LockOn;

class FollowCamera : public BaseCamera{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

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
	/// リスタート
	/// </summary>
	void Restart();

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

	void SetLockOn(LockOn* lockOn) { lockOn_ = lockOn; }

private: // メンバ関数

	// 追従対象からのオフセットを計算する
	Vector3 OffsetCalc() const;

	/// <summary>
	/// 調整項目の適用
	/// </summary>
	void ApplyGlobalVariables();

private:

	//追従対象
	const WorldTransform* target_ = nullptr;

	// 追従対象の残像座標
	Vector3 interTarget_ = {};

	Vector3 destinationAngle_ = {0.0f,0.0f,0.0f};

	float moveRate_ = 0.1f;
	float rotateRate_ = 0.1f;

	LockOn* lockOn_ = nullptr;

};

