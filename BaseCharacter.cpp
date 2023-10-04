#include "BaseCharacter.h"
#include "ViewProjection.h"

void BaseCharacter::Initialize(const std::vector<Model*>& models,
	const std::vector<Material*>& materials,
	const ViewProjection* viewProjection)
{

	//モデルデータ配列
	models_ = models;
	//マテリアルデータ配列
	materials_ = materials;
	//ワールド変換データの初期化
	worldTransform_.Initialize(viewProjection);

}

void BaseCharacter::Update()
{

	// ワールド変換データの行列更新
	worldTransform_.UpdateMatrix();

}

void BaseCharacter::Draw()
{

	//モデル描画
	for (Model* model : models_) {
		model->Draw(worldTransform_);
	}

}
