#pragma once
#include "../../Engine/3D/Model.h"
#include "../../Engine/3D/WorldTransform.h"
#include <memory>
#include <vector>
#include <array>
#include <optional>

#include "../BaseCharacter/BaseCharacter.h"
#include "../../Engine/Collider/Sphere/Sphere.h"

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
		kDash, // ダッシュ中
	};

	// コンボフェーズ
	enum ComboPhase {
		kAnticipation,
		kCharge,
		kSwing,
		kRecovery,
		kCountOfComboPhase,
	};

	// 通常用ワーク
	struct WorkRoot {
		// 初期位置
		const Vector3 kInitialPosition = { 0.0f, 0.0f, 0.0f };
		// 初期角度
		const Vector3 kInitialDirection = { 0.0f, 0.0f, 1.0f };
		// ジャンプ初期速さ
		const float kJumpSpeed = 1.0f;
		// 落下加速
		const float kFallAcceleration = 0.05f;
		// 歩行速度
		const float kWalkSpeed = 0.6f;
		// コライダーサイズ(半径)
		const float kColliderSize = 2.0f;

		// 目標角度
		Vector3 targetDirection_;

		// 補間レート
		float targetAngleT_ = 0.1f;
	
	};

	// 攻撃用ワーク
	struct WorkAttack {
		// コライダー
		std::unique_ptr<Sphere> attackCollider_;
		// プレイヤーと攻撃コライダーの中心までの距離
		Vector3 attackCenterAdd_;
		// 攻撃半径
		float attackRadius_;
		// あたり判定を取るか
		bool isAttackJudgment_;

		//攻撃行動用の媒介変数
		float behaviorAttackParameter_ = 0.0f;
		// 攻撃行動用のサイクル<frame>
		uint32_t behaviorAttackPeriod_ = 60;

		uint32_t comboIndex_ = 0;
		uint32_t inComboPhase_ = 0;
		bool comboNext_ = false;

	};

	// ダッシュ用ワーク
	struct WorkDash{
		// ダッシュ用の媒介変数
		uint32_t deshPrameter_ = 0;
		// ダッシュ速度
		float spped_ = 2.0f;
		// ダッシュフレーム
		uint32_t period_ = 10;
	};

	//浮遊ギミック用ワーク
	struct WorkFloating {
		//浮遊ギミックの媒介変数
		float floatingParameter_ = 0.0f;
		// 浮遊移動のサイクル<frame>
		int32_t floatingPeriod_ = 60;
		// 浮遊の振幅<m>
		float floatingAmplitude_ = 0.5f;
	};

	// ぶらぶらギミック用ワーク
	struct WorkSwing {
		//ぶらぶらギミックの媒介変数
		float swingParameter_ = 0.0f;
		//ぶらぶらギミックのサイクル<frame>
		int32_t swingPeriod_ = 60;
	};

	// 攻撃用定数　時間<frame>
	struct ConstAttack {
		uint32_t time_[ComboPhase::kCountOfComboPhase];
		float speed_[ComboPhase::kCountOfComboPhase];
	};

public: // 静的メンバ変数

	// コンボの数
	static const int kComboNum = 2;
	// コンボ定数表
	static const std::array<Player::ConstAttack, Player::kComboNum> kConstAttaks;

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

	/// <summary>
	/// ダッシュ行動初期化
	/// </summary>
	void BehaviorDashInitialize();

	/// <summary>
	/// ダッシュ行動更新
	/// </summary>
	void BehaviorDashUpdate();

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
	/// ダッシュ開始
	/// </summary>
	void DashStart();

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

public: // 攻撃関数

	/// <summary>
	/// コンボ継続判定
	/// </summary>
	void AttackComboContinuationJudgment();

	/// <summary>
	/// コンボフェーズ終了判定
	/// </summary>
	void AttackComboPhaseFinished();

	/// <summary>
	/// コンボ終了判定
	/// </summary>
	void AttackComboFinished();

	/// <summary>
	/// コンボ1段目
	/// </summary>
	void AttackCombo1st();

	/// <summary>
	/// コンボ2段目
	/// </summary>
	void AttackCombo2nd();

private:

	/// <summary>
	/// 調整項目の適用
	/// </summary>
	void ApplyGlobalVariables();

public: // アクセッサ

	WorldTransform* GetWorldTransformAddress() { return &worldTransform_; }

	void SetViewProjection(ViewProjection* viewProjection) { viewProjection_ = viewProjection;}

	float GetColliderRadius() { return workRoot_.kColliderSize; }

	Sphere* GetCollider() { return collider_.get(); }

	Sphere* GetAttackCollider() { return workAttack_.attackCollider_.get(); }

	bool GetIsAttackJudgment() { return workAttack_.isAttackJudgment_; }

	bool GetIsDead() { return isDead_; }

private:
	//全てのオブジェのUpdateMatrixをまとめたもの
	void allUpdateMatrix();

private: // メンバ変数

	// 速度
	Vector3 velocity_;

	// 着地しているか
	bool isLanding;

	// ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;

	// コライダー
	std::unique_ptr<Sphere> collider_;

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

	// 通常
	WorkRoot workRoot_;

	// 攻撃
	WorkAttack workAttack_;

	// ダッシュ
	WorkDash workDash_;

	// 浮遊
	WorkFloating workFloating_;

	// ぶらぶら
	WorkSwing workSwing_;

	bool isDead_ = false;

};

