#include "Goal.h"

void Goal::Initialize(const std::vector<Model*>& models, const std::vector<Material*>& materials, const ViewProjection* viewProjection)
{

	//モデルデータ配列
	models_ = models;
	//マテリアルデータ配列
	materials_ = materials;
	//ワールド変換データの初期化
	worldTransform_.Initialize(viewProjection);
	worldTransform_.transform_.translate = kPosition;
	worldTransform_.UpdateMatrix();

}

void Goal::Update()
{

	worldTransform_.UpdateMatrix();

}

void Goal::Draw()
{

	for (Model* model : models_) {
		model->Draw(worldTransform_);
	}

}
