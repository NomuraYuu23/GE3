#include "Floor.h"
#include <cmath>

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

	position_ = position;

	isMoving_ = isMoving;

	moveTimer_ = 0.0f;

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

	Matrix4x4Calc* m4Calc = Matrix4x4Calc::GetInstance();
	Vector3Calc* v3Calc = Vector3Calc::GetInstance();

	Vector3 position = { 20.0f, 0.0f,0.0f };
	moveTimer_ += 0.02f;
	if (moveTimer_ >= 6.28f) {
		moveTimer_ = 0.0f;
	}

	// カメラの角度から回転行列を計算する
	Matrix4x4 rotateMatrixX = m4Calc->MakeRotateXMatrix(worldTransform_.transform_.rotate.x);
	Matrix4x4 rotateMatrixY = m4Calc->MakeRotateYMatrix(worldTransform_.transform_.rotate.y);
	Matrix4x4 rotateMatrixZ = m4Calc->MakeRotateZMatrix(worldTransform_.transform_.rotate.z);

	Matrix4x4 rotateMatrix = m4Calc->Multiply(
		rotateMatrixX, m4Calc->Multiply(rotateMatrixY, rotateMatrixZ));

	// 移動ベクトルをカメラの角度だけ回転する
	position = m4Calc->TransformNormal(position, rotateMatrix);

	worldTransform_.transform_.translate = v3Calc->Add(position_, v3Calc->Multiply(sinf(moveTimer_),position));

}
