#include "Floor.h"

void Floor::Initialize(Model* model, Material* material, ViewProjection* viewProjection, Vector3 position, Vector3 rotate, bool isMoving)
{

	// nullポインタチェック
	assert(model);

	model_ = model;

	material_ = material;

	// ワールド変換データの初期化
	worldTransform_.Initialize(viewProjection);
	worldTransform_.transform_.translate = position;
	worldTransform_.transform_.rotate = rotate;
	worldTransform_.UpdateMatrix();

	isMoving_ = isMoving;

}

void Floor::Update()
{

	if (isMoving_) {
		Move();
	}

	worldTransform_.UpdateMatrix();

}

void Floor::Draw()
{

	model_->Draw(worldTransform_);

}

void Floor::Move()
{
}
