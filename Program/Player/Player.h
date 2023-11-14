#pragma once
#include "../../Engine/3D/Model.h"
#include "../../Engine/3D/WorldTransform.h"
#include <memory>
#include <vector>
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
		kModelIndexWeapon = 4,
		kModelIndexExprode = 5
	};

	// 振るまい
	enum class Behavior {
		kRoot, // 通常状態
		kAttack, //攻撃中
		kDash, // ダッシュ中
	};

	// 通常用ワーク
	struct WorkRoot {
		// 初期位置
		Vector3 kInitialPosition = { 0.0f, 0.0f, 0.0f };
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

		// 目標角度
		float targetAngle_;

		// 補間レート
		float targetAngleT_ = 0.1f;
	
	};

	// 攻撃用ワーク
	struct WorkAttack {
		// コライダー
		Sphere attackCollider_;
		// プレイヤーと攻撃コライダーの中心までの距離
		Vector3 attackCenterAdd_;
		// 攻撃半径
		float attackRadius_;
		// あたり判定を取るか
		bool isAttackJudgment_;

		//攻撃行動用の媒介変数
		float behaviorAttackParameter_ = 0.0f;
		// 攻撃行動用のサイクル<frame>
		int32_t behaviorAttackPeriod_ = 60;

	};

	// ダッシュ用ワーク
	struct WorkDash{
		// ダッシュ用の媒介変数
		uint32_t deshPrameter_ = 0;
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
	/// 爆破
	/// </summary>
	void Explosion();

	/// <summary>
	/// 爆破範囲の拡大
	/// </summary>
	void ExplosionMove();
	
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
	/// 壁との衝突
	/// </summary>
	void OnCollisionBox(WorldTransform* worldTransform, float boxSize);

	/// <summary>
	/// 回復アイテムとの衝突
	/// </summary>
	void OnCollisionRecoveryItem(int recoveryValue);

	/// <summary>
	/// 収集アイテムとの衝突
	/// </summary>
	void OnCollisionCollectibleItem();

	/// <summary>
	/// 復活ポジション再設定
	/// </summary>
	void SetRestartPosition(const Vector3& position);

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

	void SetViewProjection(ViewProjection* viewProjection) { viewProjection_ = viewProjection;}

	float GetColliderRadius() { return workRoot_.kColliderSize; }

	Sphere& GetCollider() { return collider_; }

	Sphere& GetExplosionCollider() { return explosionCollider_; }

	Sphere& GetAttackCollider() { return workAttack_.attackCollider_; }

	bool GetIsAttackJudgment() { return workAttack_.isAttackJudgment_; }

private:
	//全てのオブジェのUpdateMatrixをまとめたもの
	void allUpdateMatrix();

	//imguiの表示まとめ
	void DrawImgui();

private: // メンバ変数

	// 速度
	Vector3 velocity_;

	// 着地しているか
	bool isLanding;

	// ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;

	// コライダー
	Sphere collider_;
	Sphere explosionCollider_;
	/*爆発関連*/
	//爆破の半径
	float explosionSpeed_;

	//爆破を広げるためのスイッチ
	bool isExplosion_;
	//爆破時間
	const int baseExplosionTimer_ = 30;

	int explosionTimer_;

	//初期爆破回数
	int startExprosionNum_ = 5;

	//残り爆発回数
	int exprosionNum_ = 0;
	//爆発の最大値
	const int exprosionMax_ = 99;
	//爆発の最低値
	const int exprosionMin_ = 0;
	//爆破回数を減らすためのフラグ
	bool isSubtraction_ = false;

	//ワールド変換データ
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	WorldTransform worldTransformWeapon_;
	WorldTransform worldTransformExprode_;

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

	//アイテム関連の変数
	int numCollectItem;
};

