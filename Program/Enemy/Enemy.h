#pragma once
#include "../../Model.h"
#include "../../WorldTransform.h"
#include <memory>
#include <vector>
#include "../BaseCharacter/BaseCharacter.h"

class Enemy : public BaseCharacter {

public:
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
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

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

	float GetColliderRadius() { return kColliderSize; }

private:

	// ワールド変換データ
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;

	// ポジション
	Vector3 position_;

	// 移動用
	// 速度
	Vector3 velocity_ = { 0.0f, 0.0f, 0.0f };
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

};
