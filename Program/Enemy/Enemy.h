#pragma once
#include "../../Engine/3D/Model.h"
#include "../../Engine/3D/WorldTransform.h"
#include <memory>
#include <vector>
#include "../BaseCharacter/BaseCharacter.h"
#include "../../Engine/Collider/Sphere/Sphere.h"

class Player;

class Enemy : public BaseCharacter {

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(TransformStructure transform, const std::vector<Model*>& models,
		const std::vector<Material*>& materials);
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 落下
	/// </summary>
	void Fall();

	/// <summary>
	/// 着地
	/// </summary>
	void Landing();

	/// <summary>
	/// 衝突
	/// </summary>
	void OnCollision(WorldTransform* worldTransform);

	/// <summary>
	/// 壁との衝突
	/// </summary>
	void OnCollisionBox(WorldTransform* worldTransform, Vector3 boxSize, bool isMove);

	/// <summary>
	/// 敵同士の衝突
	/// </summary>
	void OnCollisionEnemy(WorldTransform* worldTransform, float size);

	/// <summary>
	/// 回転
	/// </summary>
	void Rotation();



	/// <summary>
	/// 腕回転ギミック初期化
	/// </summary>
	void InitializeArmRotationgimmick();

	/// <summary>
	/// 腕回転ギミック更新
	/// </summary>
	void UpdateArmRotationgimmick();

	/// <summary>
	/// 親を得た
	/// </summary>
	void GotParent(WorldTransform* parent);


	/// <summary>
	/// 親を失った
	/// </summary>
	void LostParent();

	//imguiの表示まとめ
	void DrawImgui();

public:
	WorldTransform* GetWorldTransformAddress() { return &worldTransform_; }

	WorldTransform GetWorldTransform()const { return worldTransform_; }

	float GetColliderRadius() { return kColliderSize; }

	Sphere& GetCollider() { return collider_; }

	Sphere& GetSearchCollider() { return searchCollider_; }

	bool GetIsDead() { return isDead_; }

	void SetIsDead(bool isDead) { isDead_ = isDead; }

	void SetPlayer(const Player* player) { player_ = player; }

private:
	//全てのオブジェのUpdateMatrixをまとめたもの
	void allUpdateMatrix();

	bool isFall_ = false;

	bool isMove_ = false;

private:

	// ワールド変換データ
	WorldTransform worldTransformBody_;/*
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;*/

	// ポジション
	Vector3 position_;

	// 移動用
	// 速度
	Vector3 velocity_ = { 0.0f, 0.0f, 0.0f };

	float fallSpeed_ = 0.0f;
	// 速さ
	float kMoveSpeed;

	// 回転用
	// 回転速度
	float kRotateSpeed;

	// 腕回転ギミック用
	// 腕回転ギミックの媒介変数
	float armRotationParameter_ = 0.0f;

	// 腕回転ギミックのサイクル<frame>
	uint16_t armRotationPeriod_ = 1;

	// コライダーサイズ
	const float kColliderSize = 1.0f;

	const float kSearchColliderSize = 40.0f;

	// コライダー
	Sphere collider_;

	Sphere searchCollider_;

	// 死亡フラグ
	bool isDead_;

	// 着地しているか
	bool isLanding;

	const Player* player_ = nullptr;
};
