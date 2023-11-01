#pragma once
#include "../BaseCharacter/BaseCharacter.h"
#include "../../Engine/Collider/Sphere/Sphere.h"
class Goal : public BaseCharacter
{

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

public: // アクセッサ

	/// <summary>
	/// 半径ゲッター
	/// </summary>
	/// <returns></returns>
	float GetRadius() { return kRadius; }

	Sphere& GetCollider() { return collider_; }

private: // メンバ変数

	// 位置
	const Vector3 kPosition = { 60.0f, 1.0f, 60.0f };

	// 半径
	const float kRadius = 2.0f;

	// コライダー
	Sphere collider_;

};

