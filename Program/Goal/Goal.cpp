#include "Goal.h"

void Goal::Initialize(const std::vector<Model*>& models, const std::vector<Material*>& materials)
{

	//モデルデータ配列
	models_ = models;
	//マテリアルデータ配列
	materials_ = materials;
	//ワールド変換データの初期化
	worldTransform_.Initialize();
	worldTransform_.transform_.translate = kPosition;
	worldTransform_.UpdateMatrix();

	collider_.Initialize(kPosition, kRadius);

}

void Goal::Update()
{

	worldTransform_.UpdateMatrix();
	collider_.worldTransformUpdate();

}

void Goal::Draw(const ViewProjection& viewProjection)
{

	for (Model* model : models_) {
		model->Draw(worldTransform_, viewProjection);
	}

}
