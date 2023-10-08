#include "floor.h"

void floor::Initialize(Model* model, Material* material, ViewProjection* viewProjection)
{

	// nullポインタチェック
	assert(model);

	model_ = model;

	material_ = material;

	// ワールド変換データの初期化
	worldTransform_.Initialize(viewProjection);

}

void floor::Update()
{

	worldTransform_.UpdateMatrix();

}

void floor::Draw()
{

	model_->Draw(worldTransform_);

}
