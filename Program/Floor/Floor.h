#pragma once
#include "../../Model.h"
#include "../../Material.h"
#include "../../WorldTransform.h"

class Floor
{
public: //メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(Model* model, Material* material, ViewProjection* viewProjection, Vector3 position, Vector3 rotate, bool isMovig);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw();

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

public: // アクセッサ

	WorldTransform GetWorldTransform() { return worldTransform_; }

	WorldTransform* GetWorldTransformAdress() { return &worldTransform_; }

	Vector3 GetColliderSize() { return colliderSize_; }

private:

	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	//マテリアル
	Material* material_ = nullptr;

	// 動くか
	bool isMoving_;

	// サイズ
	Vector3 colliderSize_ = {10.0f,1.0f, 10.0f};

};

