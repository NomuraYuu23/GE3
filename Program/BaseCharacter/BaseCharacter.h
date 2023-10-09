#pragma once
#include "../../Model.h"
#include "../../Material.h"
#include "../../WorldTransform.h"
#include <memory>
#include <vector>

class BaseCharacter {

protected:
	//モデルデータ配列
	std::vector<Model*> models_;
	//マテリアルデータ配列
	std::vector<Material*> materials_;
	//ワールド変換データ
	WorldTransform worldTransform_;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models">モデルデータ配列</param>
	virtual void Initialize(const std::vector<Model*>& models,
		const std::vector<Material*>& materials,
		const ViewProjection* viewProjection);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	virtual void Draw();

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

};
