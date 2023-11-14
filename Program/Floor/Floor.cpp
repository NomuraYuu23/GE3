#include "Floor.h"
#include <cmath>
#include"../../Engine/2D/ImguiManager.h"

void Floor::Initialize(Model* model, Material* material, Vector3 position, Vector3 rotate, bool isMoving, bool isVertical)
{

	// nullポインタチェック
	assert(model);

	model_ = model;

	material_ = material;

	// ワールド変換データの初期化
	worldTransform_.Initialize();
	worldTransform_.transform_.translate = position;
	worldTransform_.transform_.rotate = rotate;
	worldTransform_.UpdateMatrix();

	position_ = position;

	isMoving_ = isMoving;

	isVertical_ = isVertical;

	moveTimer_ = 0.0f;

	Vector3 colliderMax_ = { position_.x + size_.x, position_.y + size_.y, position_.z + size_.z };
	Vector3 colliderMin_ = { position_.x - size_.x, position_.y - size_.y, position_.z - size_.z };

	collider_.Initialize(colliderMin_, colliderMax_);

}

void Floor::Update()
{

	if (isMoving_) {
		if (isVertical_){
			verticalMove();
		}
		else {
			Move();
		}
		Vector3 WorldPosition = { worldTransform_.worldMatrix_.m[3][0] , worldTransform_.worldMatrix_.m[3][1] , worldTransform_.worldMatrix_.m[3][2] };
		collider_.max_ = { WorldPosition.x + size_.x, WorldPosition.y + size_.y, WorldPosition.z + size_.z};
		collider_.min_ = { WorldPosition.x - size_.x, WorldPosition.y - size_.y, WorldPosition.z - size_.z };
	}

	worldTransform_.UpdateMatrix();
	collider_.worldTransformUpdate();

}

void Floor::Draw(const ViewProjection& viewProjection)
{

	model_->Draw(worldTransform_, viewProjection);

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

void Floor::verticalMove(){
	Matrix4x4Calc* m4Calc = Matrix4x4Calc::GetInstance();
	Vector3Calc* v3Calc = Vector3Calc::GetInstance();

	Vector3 position = { 0.0f, 10.0f,0.0f };
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

	worldTransform_.transform_.translate = v3Calc->Add(position_, v3Calc->Multiply(cosf(moveTimer_), position));
}

void Floor::DrawImgui(){
	ImGui::DragFloat3("床の座標", &worldTransform_.transform_.translate.x, 0.1f);
	ImGui::DragFloat3("床の回転", &worldTransform_.transform_.rotate.x, 0.1f);
	ImGui::DragFloat3("床の大きさ", &worldTransform_.transform_.scale.x, 0.1f);
}
