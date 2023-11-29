#include "Fence.h"
#include"../../externals/imgui/imgui.h"

void Fence::Initialize(Model* model, Material* material, TransformStructure transform_) {
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

	Vector3 colliderMax_ = { position_.x + size_.x, position_.y + (size_.y * 300.0f), position_.z + size_.z };
	Vector3 colliderMin_ = { position_.x - size_.x, position_.y - size_.y, position_.z - size_.z };

	collider_.Initialize(colliderMin_, colliderMax_);
}

void Fence::Update() {
	Vector3 WorldPosition = { drawWorldTransform_.worldMatrix_.m[3][0] , drawWorldTransform_.worldMatrix_.m[3][1] , drawWorldTransform_.worldMatrix_.m[3][2] };
	size_ = { drawWorldTransform_.transform_.scale.x + 0.1f,drawWorldTransform_.transform_.scale.y + 0.1f,drawWorldTransform_.transform_.scale.z + 0.1f, };
	collider_.max_ = { WorldPosition.x + size_.x, WorldPosition.y + (size_.y * 300.0f), WorldPosition.z + size_.z };
	collider_.min_ = { WorldPosition.x - size_.x, WorldPosition.y - size_.y, WorldPosition.z - size_.z };

	worldTransform_.UpdateMatrix();
	drawWorldTransform_.UpdateMatrix();
	collider_.worldTransformUpdate();
}

void Fence::Draw(const ViewProjection& viewProjection) {

	model_->Draw(drawWorldTransform_, viewProjection);
}

void Fence::DrawImgui() {
	ImGui::DragFloat3("箱の座標", &drawWorldTransform_.transform_.translate.x, 0.1f);
	ImGui::DragFloat3("箱の回転", &drawWorldTransform_.transform_.rotate.x, 0.1f);
	ImGui::DragFloat3("箱の大きさ", &drawWorldTransform_.transform_.scale.x, 0.1f, 0.0f, 300.0f);
	if (ImGui::Button("このオブジェを削除")) {
		isDelete_ = true;
	}
	worldTransform_.transform_.translate = drawWorldTransform_.transform_.translate;
	//position_ = drawWorldTransform_.transform_.translate;
}
