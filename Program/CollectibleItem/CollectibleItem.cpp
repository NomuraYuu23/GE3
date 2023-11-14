#include"CollectibleItem.h"
#include <cmath>
#include"../../externals/imgui/imgui.h"

void CollectibleItem::Initialize(Model* model, Material* material, TransformStructure transform_, bool isFalling/*, bool isMoving, bool isVertical*/){
	// nullポインタチェック
	assert(model);

	model_ = model;

	material_ = material;

	isFalling_ = isFalling;

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

	moveTimer_ = 0.0f;

	Vector3 colliderMax_ = { position_.x + size_.x, position_.y + size_.y, position_.z + size_.z };
	Vector3 colliderMin_ = { position_.x - size_.x, position_.y - size_.y, position_.z - size_.z };

	collider_.Initialize(colliderMin_, colliderMax_);

	isDelete_ = false;
}

void CollectibleItem::Update(){

	if (worldTransform_.worldMatrix_.m[3][1] <= -50.0f) {
		isDelete_ = true;
	}
	if (isFalling_){
		Fall();
		Landing();
	}
	
	worldTransform_.transform_.translate.y += velocity_.y;
	drawWorldTransform_.transform_.translate.y += velocity_.y;

	Vector3 WorldPosition = { drawWorldTransform_.worldMatrix_.m[3][0] , drawWorldTransform_.worldMatrix_.m[3][1] , drawWorldTransform_.worldMatrix_.m[3][2] };
	size_ = { drawWorldTransform_.transform_.scale.x + 0.1f,drawWorldTransform_.transform_.scale.y + 0.1f,drawWorldTransform_.transform_.scale.z + 0.1f, };
	collider_.max_ = { WorldPosition.x + size_.x, WorldPosition.y + size_.y, WorldPosition.z + size_.z };
	collider_.min_ = { WorldPosition.x - size_.x, WorldPosition.y - size_.y, WorldPosition.z - size_.z };

	collider_.worldTransformUpdate();
	worldTransform_.UpdateMatrix();
	drawWorldTransform_.UpdateMatrix();
	
}

void CollectibleItem::Draw(const ViewProjection& viewProjection){
	model_->Draw(drawWorldTransform_, viewProjection);
}

void CollectibleItem::DrawImgui(){
	ImGui::DragFloat3("アイテムの座標", &drawWorldTransform_.transform_.translate.x, 0.1f);
	ImGui::DragFloat3("アイテムの回転", &drawWorldTransform_.transform_.rotate.x, 0.1f);
	ImGui::DragFloat3("アイテムの大きさ", &drawWorldTransform_.transform_.scale.x, 0.1f, 0.0f, 300.0f);
	worldTransform_.transform_.translate = drawWorldTransform_.transform_.translate;
}

void CollectibleItem::Fall(){
	if (!isLanding) {
		velocity_.y -= kFallAcceleration;
	}
}

void CollectibleItem::Landing(){
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

void CollectibleItem::GotParent(WorldTransform* parent){

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

void CollectibleItem::LostParent(){
	Vector3 position = { worldTransform_.worldMatrix_.m[3][0] ,worldTransform_.worldMatrix_.m[3][1] ,worldTransform_.worldMatrix_.m[3][2] };

	worldTransform_.transform_.translate = position;
	drawWorldTransform_.transform_.translate = position;
	worldTransform_.parent_ = nullptr;
	drawWorldTransform_.parent_ = nullptr;
	worldTransform_.UpdateMatrix();
	drawWorldTransform_.UpdateMatrix();
}

void CollectibleItem::OnCollision(WorldTransform* worldTransform){
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

void CollectibleItem::OnCollisionBox(WorldTransform* worldTransform, float boxSize){
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

void CollectibleItem::OnCollisionPlayer(){
	isDelete_ = true;
}
