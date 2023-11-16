#pragma once
#include "../../Engine/3D/Model.h"
#include "../../Engine/3D/Material.h"
#include "../../Engine/3D/WorldTransform.h"
#include "../../Engine/Collider/AABB/AABB.h"

class CollectibleItem {
public: //メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(Model* model, Material* material, TransformStructure transform_,bool isFalling/*, bool isMoving, bool isVertical*/);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	void DrawImgui();

	/// <summary>
/// 落下
/// </summary>
	void Fall();

	/// <summary>
	/// 着地
	/// </summary>
	void Landing();

	/// <summary>
	/// 親を得た
	/// </summary>
	void GotParent(WorldTransform* parent);

	/// <summary>
	/// 親を失った
	/// </summary>
	void LostParent();

	/// <summary>
	/// 衝突
	/// </summary>
	void OnCollision(WorldTransform* worldTransform);

	/// <summary>
	/// 壁との衝突
	/// </summary>
	void OnCollisionBox(WorldTransform* worldTransform, float boxSize);

	/// <summary>
	/// プレイヤーとの衝突
	/// </summary>
	void OnCollisionPlayer();

public: // アクセッサ

	WorldTransform GetWorldTransform() { return worldTransform_; }

	WorldTransform GetDrawWorldTransform()const { return drawWorldTransform_; }

	WorldTransform GetMakeWorldTransform()const { return makeWorldTransform_; }

	WorldTransform* GetWorldTransformAdress() { return &worldTransform_; }

	WorldTransform* GetDrawWorldTransformAdress() { return &drawWorldTransform_; }

	Vector3 GetSize() { return size_; }

	AABB& GetCollider() { return collider_; }

	bool GetIsDelete() { return isDelete_; }

	bool GetisFalling() { return isFalling_; }

private:

	// 速度
	Vector3 velocity_;

	// 着地しているか
	bool isLanding;

	// 落下加速
	const float kFallAcceleration = 0.05f;

	// ワールド変換データ
	WorldTransform worldTransform_;
	//描画用のワールドトランスフォーム
	WorldTransform drawWorldTransform_;
	//生成場所保存用のワールドトランスフォーム
	WorldTransform makeWorldTransform_;

	// モデル
	Model* model_ = nullptr;
	//マテリアル
	Material* material_ = nullptr;

	// 動くか
	bool isMoving_;

	// 落ちるか
	bool isFalling_;

	//アイテムを削除するか
	bool isDelete_;

	// 移動用タイマー
	float moveTimer_;

	// サイズ
	Vector3 size_ = { 10.0f,0.1f, 10.0f };

	// position
	Vector3 position_;

	AABB collider_;
};