#include "BreakBox.h"
#include <cmath>
#include"../../externals/imgui/imgui.h"

void BreakBox::Initialize(Model* model, Material* material, TransformStructure transform_, bool isMoving, bool isVertical){
	// nullポインタチェック
	assert(model);

	model_ = model;

	material_ = material;

	// ワールド変換データの初期化
	worldTransform_.Initialize();
	worldTransform_.transform_.translate = transform_.translate;
	worldTransform_.transform_.rotate = transform_.rotate;
	worldTransform_.UpdateMatrix();

	drawWorldTransform_.Initialize();
	drawWorldTransform_.transform_.translate = transform_.translate;
	drawWorldTransform_.transform_.rotate = transform_.rotate;
	drawWorldTransform_.transform_.scale = transform_.scale;
	drawWorldTransform_.UpdateMatrix();

	position_ = transform_.translate;

	size_ = { drawWorldTransform_.transform_.scale.x + 0.1f,drawWorldTransform_.transform_.scale.y + 0.1f,drawWorldTransform_.transform_.scale.z + 0.1f, };

	isMoving_ = isMoving;

	isVertical_ = isVertical;

	moveTimer_ = 0.0f;

	isBreak_ = false;

	Vector3 colliderMax_ = { position_.x + size_.x, position_.y + size_.y, position_.z + size_.z };
	Vector3 colliderMin_ = { position_.x - size_.x, position_.y - size_.y, position_.z - size_.z };

	collider_.Initialize(colliderMin_, colliderMax_);
}

void BreakBox::Update(){
	if (isMoving_) {
		if (isVertical_) {
			verticalMove();
		}
		else {
			Move();
		}

	}
	Vector3 WorldPosition = { drawWorldTransform_.worldMatrix_.m[3][0] , drawWorldTransform_.worldMatrix_.m[3][1] , drawWorldTransform_.worldMatrix_.m[3][2] };
	size_ = { drawWorldTransform_.transform_.scale.x + 0.1f,drawWorldTransform_.transform_.scale.y + 0.1f,drawWorldTransform_.transform_.scale.z + 0.1f, };
	collider_.max_ = { WorldPosition.x + size_.x, WorldPosition.y + size_.y, WorldPosition.z + size_.z };
	collider_.min_ = { WorldPosition.x - size_.x, WorldPosition.y - size_.y, WorldPosition.z - size_.z };

	worldTransform_.UpdateMatrix();
	drawWorldTransform_.UpdateMatrix();
	collider_.worldTransformUpdate();
}

void BreakBox::Draw(const ViewProjection& viewProjection){
	model_->Draw(drawWorldTransform_, viewProjection);
}

void BreakBox::Move(){
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

	worldTransform_.transform_.translate = v3Calc->Add(position_, v3Calc->Multiply(sinf(moveTimer_), position));
	drawWorldTransform_.transform_.translate = v3Calc->Add(position_, v3Calc->Multiply(sinf(moveTimer_), position));
}

void BreakBox::verticalMove(){
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
	drawWorldTransform_.transform_.translate = v3Calc->Add(position_, v3Calc->Multiply(cosf(moveTimer_), position));
}

void BreakBox::DrawImgui(){
	ImGui::DragFloat3("箱の座標", &drawWorldTransform_.transform_.translate.x, 0.1f);
	ImGui::DragFloat3("箱の回転", &drawWorldTransform_.transform_.rotate.x, 0.1f);
	ImGui::DragFloat3("箱の大きさ", &drawWorldTransform_.transform_.scale.x, 0.1f, 0.0f, 300.0f);
	ImGui::Checkbox("動くかどうか", &isMoving_);
	ImGui::Checkbox("縦移動させるか", &isVertical_);
	if (ImGui::Button("このオブジェを削除")) {
		isBreak_ = true;
	}
	worldTransform_.transform_.translate = drawWorldTransform_.transform_.translate;
	position_ = drawWorldTransform_.transform_.translate;

}
