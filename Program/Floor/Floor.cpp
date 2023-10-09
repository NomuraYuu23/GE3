#include "Floor.h"

void Floor::Initialize(Model* model, Material* material, ViewProjection* viewProjection)
{

	// nullポインタチェック
	assert(model);

	model_ = model;

	material_ = material;

	// ワールド変換データの初期化
	worldTransform_.Initialize(viewProjection);

}

void Floor::Update()
{

	worldTransform_.UpdateMatrix();

}

void Floor::Draw()
{

	model_->Draw(worldTransform_);

}

void Floor::Move()
{
}
