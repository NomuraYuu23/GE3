#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <memory>
#include <vector>
#include <optional>

#include "BaseCharacter.h"
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

};

