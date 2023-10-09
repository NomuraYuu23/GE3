#pragma once
#include "../../Model.h"
#include "../../WorldTransform.h"
#include <memory>
#include <vector>
#include <optional>

#include "../BaseCharacter/BaseCharacter.h"

class Player : public BaseCharacter
{

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(const std::vector<Model*>& models,
		const std::vector<Material*>& materials,
		const ViewProjection* viewProjection);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw() override;

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 歩く
	/// </summary>
	void Walk();

	/// <summary>
	/// ジャンプ
	/// </summary>
	void Jump();
	
	/// <summary>
	/// 落下
	/// </summary>
	void Fall();

	/// <summary>
	/// 着地
	/// </summary>
	void Landing();

	/// <summary>
	/// リスタート
	/// </summary>
	void Restart();

	/// <summary>
	/// 衝突
	/// </summary>
	void OnCollision(WorldTransform* worldTransform);

	/// <summary>
	/// 親を得た
	/// </summary>
	void GotParent(WorldTransform* parent);

	/// <summary>
	/// 親を失った
	/// </summary>
	void LostParent();

public: // アクセッサ

	WorldTransform* GetWorldTransformAddress() { return &worldTransform_; }

	void SetViewProjection(const ViewProjection* viewProjection) {	worldTransform_.viewProjection_ = viewProjection;}

	float GetColliderRadius() { return kColliderSize; }

private: // メンバ変数

	// 速度
	Vector3 velocity_;

	// 着地しているか
	bool isLanding;


private: // メンバ定数

	// 初期位置
	const Vector3 kInitialPosition = {};

	// 初期角度
	const Vector3 kInitialRotate = {};

	// ジャンプ初期速さ
	const float kJumpSpeed = 1.0f;

	// 落下加速
	const float kFallAcceleration = 0.05f;

	// 歩行速度
	const float kWalkSpeed = 0.6f;

	// コライダーサイズ(半径)
	const float kColliderSize = 5.0f;


};

