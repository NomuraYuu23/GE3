#include"RecoveryItem.h"
#include <cmath>
#include"../../externals/imgui/imgui.h"

void RecoveryItem::Initialize(Model* model, Material* material, TransformStructure transform_, int recoveryValue/*, bool isMoving, bool isVertical*/){
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

	makeWorldTransform_.Initialize();
	makeWorldTransform_.transform_.translate = transform_.translate;
	makeWorldTransform_.transform_.rotate = transform_.rotate;
	makeWorldTransform_.transform_.scale = transform_.scale;
	makeWorldTransform_.UpdateMatrix();

	position_ = transform_.translate;

	size_ = { drawWorldTransform_.transform_.scale.x + 0.1f,drawWorldTransform_.transform_.scale.y + 0.1f,drawWorldTransform_.transform_.scale.z + 0.1f, };

	moveTimer_ = 0.0f;

	recoveryValue_ = recoveryValue;

	Vector3 colliderMax_ = { position_.x + size_.x, position_.y + size_.y, position_.z + size_.z };
	Vector3 colliderMin_ = { position_.x - size_.x, position_.y - size_.y, position_.z - size_.z };

	collider_.Initialize(colliderMin_, colliderMax_);

	isDelete_ = false;
}

void RecoveryItem::Update(){

	if (worldTransform_.worldMatrix_.m[3][1] <= -50.0f) {
		isDelete_ = true;
	}


	Vector3 WorldPosition = { drawWorldTransform_.worldMatrix_.m[3][0] , drawWorldTransform_.worldMatrix_.m[3][1] , drawWorldTransform_.worldMatrix_.m[3][2] };
	size_ = { drawWorldTransform_.transform_.scale.x + 0.1f,drawWorldTransform_.transform_.scale.y + 0.1f,drawWorldTransform_.transform_.scale.z + 0.1f, };
	collider_.max_ = { WorldPosition.x + size_.x, WorldPosition.y + size_.y, WorldPosition.z + size_.z };
	collider_.min_ = { WorldPosition.x - size_.x, WorldPosition.y - size_.y, WorldPosition.z - size_.z };

	collider_.worldTransformUpdate();
	worldTransform_.UpdateMatrix();
	drawWorldTransform_.UpdateMatrix();
	
}

void RecoveryItem::Draw(const ViewProjection& viewProjection){
	model_->Draw(drawWorldTransform_, viewProjection);
}

void RecoveryItem::DrawImgui(){
	ImGui::DragFloat3("アイテムの座標", &makeWorldTransform_.transform_.translate.x, 0.1f);
	ImGui::DragFloat3("アイテムの回転", &makeWorldTransform_.transform_.rotate.x, 0.1f);
	ImGui::DragFloat3("アイテムの大きさ", &makeWorldTransform_.transform_.scale.x, 0.1f, 0.0f, 300.0f);
	ImGui::DragInt("アイテムの回復値", &recoveryValue_, 1.0f, 1, 99);
	if (ImGui::Button("このオブジェを削除")) {
		isDelete_ = true;
	}
	drawWorldTransform_.transform_.translate = makeWorldTransform_.transform_.translate;
	worldTransform_.transform_.translate = drawWorldTransform_.transform_.translate;
}

void RecoveryItem::Fall(){
	if (!isLanding) {
		velocity_.y -= kFallAcceleration;
	}
}

void RecoveryItem::Landing(){
	if (!isLanding) {
		if (worldTransform_.parent_) {
			LostParent();
		}
	}
	else {
		velocity_.y = 0.0f;
	}
	isLanding = false;
}

void RecoveryItem::GotParent(WorldTransform* parent){

		Vector3Calc* v3Calc = Vector3Calc::GetInstance();
		Matrix4x4Calc* m4Calc = Matrix4x4Calc::GetInstance();

		if (worldTransform_.parent_) {
			LostParent();
		}

		//位置
		Vector3 position = { worldTransform_.worldMatrix_.m[3][0] - parent->worldMatrix_.m[3][0],
							worldTransform_.worldMatrix_.m[3][1] - parent->worldMatrix_.m[3][1],
							worldTransform_.worldMatrix_.m[3][2] - parent->worldMatrix_.m[3][2] };

		// 親の角度から回転行列を計算する
		Matrix4x4 rotateMatrixX = m4Calc->MakeRotateXMatrix(-parent->transform_.rotate.x);
		Matrix4x4 rotateMatrixY = m4Calc->MakeRotateYMatrix(-parent->transform_.rotate.y);
		Matrix4x4 rotateMatrixZ = m4Calc->MakeRotateZMatrix(-parent->transform_.rotate.z);

		Matrix4x4 rotateMatrix = m4Calc->Multiply(
			rotateMatrixX, m4Calc->Multiply(rotateMatrixY, rotateMatrixZ));

		// 位置を親の角度だけ回転する
		position = m4Calc->TransformNormal(position, rotateMatrix);

		worldTransform_.transform_.translate = position;
		drawWorldTransform_.transform_.translate = position;
		worldTransform_.parent_ = parent;
		drawWorldTransform_.parent_ = parent;
		worldTransform_.UpdateMatrix();
		drawWorldTransform_.UpdateMatrix();
}

void RecoveryItem::LostParent(){
	Vector3 position = { worldTransform_.worldMatrix_.m[3][0] ,worldTransform_.worldMatrix_.m[3][1] ,worldTransform_.worldMatrix_.m[3][2] };

	worldTransform_.transform_.translate = position;
	drawWorldTransform_.transform_.translate = position;
	worldTransform_.parent_ = nullptr;
	drawWorldTransform_.parent_ = nullptr;
	worldTransform_.UpdateMatrix();
	drawWorldTransform_.UpdateMatrix();
}

void RecoveryItem::OnCollision(WorldTransform* worldTransform){
	if (velocity_.y <= 0.0f) {
		if (!worldTransform_.parent_ ||
			(worldTransform_.parent_ != worldTransform)) {
			GotParent(worldTransform);
		}
		worldTransform_.transform_.translate.y = size_.y;
		drawWorldTransform_.transform_.translate.y = size_.y;
		worldTransform_.UpdateMatrix();
		drawWorldTransform_.UpdateMatrix();

		isLanding = true;
	}
}

void RecoveryItem::OnCollisionBox(WorldTransform* worldTransform, float boxSize){
	if (velocity_.y <= 0.0f) {
		if (!worldTransform_.parent_ ||
			(worldTransform_.parent_ != worldTransform)) {
			GotParent(worldTransform);
		}
		worldTransform_.transform_.translate.y = boxSize+ size_.y;
		drawWorldTransform_.transform_.translate.y = boxSize + size_.y;
		worldTransform_.UpdateMatrix();
		drawWorldTransform_.UpdateMatrix();

		isLanding = true;
	}
}

void RecoveryItem::OnCollisionPlayer(){
	isDelete_ = true;
}
