#pragma once
#include "../../Model.h"
#include "../../WorldTransform.h"
#include <memory>
#include <vector>
#include <optional>

#include "../BaseCharacter/BaseCharacter.h"
#include "../Collider/Sphere/Sphere.h"

class Player : public BaseCharacter
{

public: // サブクラス

	// モデル
	enum class ModelIndex {
		kModelIndexBody = 0,
		kModelIndexHead = 1,
		kModelIndexL_arm = 2,
		kModelIndexR_arm = 3,
		kModelIndexWeapon = 4
	};

	// 振るまい
	enum class Behavior {
		kRoot, // 通常状態
		kAttack, //攻撃中
	};


public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(const std::vector<Model*>& models,
		const std::vector<Material*>& materials);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection) override;


public: // メンバ関数(行動)

	/// <summary>
	/// 通常行動初期化
	/// </summary>
	void BehaviorRootInitialize();

	/// <summary>
	/// 通常行動更新
	/// </summary>
	void BehaviorRootUpdate();

	/// <summary>
	/// 攻撃行動初期化
	/// </summary>
	void BehaviorAttackInitialize();

	/// <summary>
	/// 攻撃行動更新
	/// </summary>
	void BehaviorAttackUpdate();

private: // メンバ関数(ギミック)

	/// <summary>
	/// 浮遊ギミック初期化
	/// </summary>
	void InitializeFloatinggimmick();

	/// <summary>
	/// 浮遊ギミック更新
	/// </summary>
	void UpdateFloatinggimmick();

	/// <summary>
	/// ぶらぶらギミック初期化
	/// </summary>
	void InitializeSwinggimmick();

	/// <summary>
	/// ぶらぶらギミック更新
	/// </summary>
	void UpdateSwinggimmick();

public: // メンバ関数

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
	/// 攻撃開始
	/// </summary>
	void AttackStart();

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

	void SetViewProjection(ViewProjection* viewProjection) {	viewProjection_ = viewProjection;}

	float GetColliderRadius() { return kColliderSize; }

	Sphere& GetCollider() { return collider_; }

private: // メンバ変数

	// 速度
	Vector3 velocity_;

	// 着地しているか
	bool isLanding;

	// ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;

	// コライダー
	Sphere collider_;

	//ワールド変換データ
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	WorldTransform worldTransformWeapon_;

	//振るまい
	Behavior behavior_ = Behavior::kRoot;
	//次の振るまいリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

private: // メンバ変数(ギミック)

	//浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
	// 浮遊移動のサイクル<frame>
	int32_t floatingPeriod_ = 60;
	// 浮遊の振幅<m>
	float floatingAmplitude_ = 0.5f;

	//ぶらぶらギミックの媒介変数
	float swingParameter_ = 0.0f;
	//ぶらぶらギミックのサイクル<frame>
	int32_t swingPeriod_ = 60;

	//攻撃行動用の媒介変数
	float behaviorAttackParameter_ = 0.0f;
	// 攻撃行動用のサイクル<frame>
	int32_t behaviorAttackPeriod_ = 60;

private: // メンバ定数

	// 初期位置
	const Vector3 kInitialPosition = {1.0f, 10.5f, 1.0f};

	// 初期角度
	const Vector3 kInitialRotate = {};

	// ジャンプ初期速さ
	const float kJumpSpeed = 1.0f;

	// 落下加速
	const float kFallAcceleration = 0.05f;

	// 歩行速度
	const float kWalkSpeed = 0.6f;

	// コライダーサイズ(半径)
	const float kColliderSize = 2.0f;


};

