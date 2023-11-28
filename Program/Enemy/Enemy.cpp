#include <cassert>
#include <cmath>
#include <numbers>

#include "Enemy.h"
#include"../../Engine/2D/ImguiManager.h"
#include"../Player/Player.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="model">モデル</param>
/// <param name="textureHandle">テクスチャハンドル</param>
void Enemy::Initialize(TransformStructure transform,const std::vector<Model*>& models,
	const std::vector<Material*>& materials) {

	// nullポインタチェック
	assert(models.front());

	// 基底クラスの初期化
	BaseCharacter::Initialize(models, materials);
	worldTransform_.transform_.translate = transform.translate;
	worldTransform_.transform_.rotate = transform.rotate;

	worldTransformBody_.Initialize();
	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformL_arm_.Initialize();
	worldTransformL_arm_.transform_.translate.x -= 2.0f;
	worldTransformL_arm_.transform_.translate.y += 1.0f;
	worldTransformL_arm_.transform_.rotate.x += float(std::numbers::pi) / 2.0f;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.Initialize();
	worldTransformR_arm_.transform_.translate.x += 2.0f;
	worldTransformR_arm_.transform_.translate.y += 1.0f;
	worldTransformR_arm_.transform_.rotate.x += float(std::numbers::pi) / 2.0f;
	worldTransformR_arm_.parent_ = &worldTransformBody_;

	// ポジション
	/*position_ = { -0.2f, 0.0f, 60.0f};
	worldTransform_.transform_.translate = position_;*/

	// 移動用
	// 速度
	velocity_ = { 0.0f, 0.0f, 0.0f };
	// 速さ
	kMoveSpeed = -0.2f;

	// 回転用
	// 回転速度
	kRotateSpeed = 0.05f;

	// 腕回転ギミック初期化
	InitializeArmRotationgimmick();

	collider_.Initialize(worldTransform_.transform_.translate, kColliderSize);

	searchCollider_.Initialize(worldTransform_.transform_.translate, kSearchColliderSize);

	isDead_ = false;

}

/// <summary>
/// 更新
/// </summary>
void Enemy::Update() {	
	// 回転
	//Rotation();
	if (isMove_){
		// 移動
		Move();
	}
	//落下
	Fall();

	// 腕回転ギミック
	UpdateArmRotationgimmick();

	//ワールド変換データ更新
	worldTransform_.UpdateMatrix();

	Landing();

	//ワールド変換データ更新
	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();

	collider_.center_ = { worldTransform_.worldMatrix_.m[3][0],worldTransform_.worldMatrix_.m[3][1], worldTransform_.worldMatrix_.m[3][2] };
	collider_.worldTransformUpdate();
	searchCollider_.center_ = { worldTransform_.worldMatrix_.m[3][0],worldTransform_.worldMatrix_.m[3][1], worldTransform_.worldMatrix_.m[3][2] };
	searchCollider_.worldTransformUpdate();
	
}

/// <summary>
/// 描画
/// </summary>
/// <param name="viewProjection">ビュープロジェクション</param>
void Enemy::Draw(const ViewProjection& viewProjection) {

	if (!isDead_) {
		models_[0]->Draw(worldTransformBody_, viewProjection);
		models_[1]->Draw(worldTransformL_arm_, viewProjection);
		models_[2]->Draw(worldTransformR_arm_, viewProjection);
	}

}

/// <summary>
/// 移動
/// </summary>
void Enemy::Move() {
	Fall();

	Matrix4x4Calc* m4Calc = Matrix4x4Calc::GetInstance();
	Vector3Calc* v3Calc = Vector3Calc::GetInstance();
	if (!player_) {
		Rotation();
		
		//移動速度
		Vector3 velocity(0.0f, 0.0f, kMoveSpeed);

		//速度ベクトルを向きに合わせて回転させる
		velocity_ = m4Calc->TransformNormal(velocity, worldTransform_.worldMatrix_);
		if (isFall_) {
			velocity_.y += fallSpeed_;
		}

		//移動
		worldTransform_.transform_.translate = v3Calc->Add(worldTransform_.transform_.translate, velocity_);
	}
	else {
		//移動速度
		Vector3 velocity = v3Calc->Subtract(worldTransform_.transform_.translate,player_->GetTransform());
		velocity.y = 0.0f;
		velocity_ =v3Calc->Multiply(kMoveSpeed, (v3Calc->Normalize(velocity)));

		worldTransform_.transform_.rotate.y = std::atan2(velocity_.x, velocity_.z) + (1.57f * 2.0f);

		worldTransform_.transform_.translate = v3Calc->Add(worldTransform_.transform_.translate, velocity_);
	}
}

void Enemy::Fall(){
	if (!isLanding) {
		fallSpeed_ -= 0.05f;
	}
}

void Enemy::Landing(){
	if (!isLanding) {
		if (worldTransform_.parent_) {
			LostParent();
		}
	}
	else {
		fallSpeed_ = 0.0f;
	}
	
}

void Enemy::OnCollision(WorldTransform* worldTransform){
	if (velocity_.y <= 0.0f) {
		if (!worldTransform_.parent_ ||
			(worldTransform_.parent_ != worldTransform)) {
			GotParent(worldTransform);
		}
		worldTransform_.transform_.translate.y = 0;
		allUpdateMatrix();

		isLanding = true;
	}
}

void Enemy::OnCollisionBox(WorldTransform* worldTransform, Vector3 boxSize, bool isMove){
	if (isMove) {
		if (worldTransform_.transform_.translate.y >= worldTransform->transform_.translate.y) {

			if (!worldTransform_.parent_ ||
				(worldTransform_.parent_ != worldTransform)) {
				GotParent(worldTransform);
			}
			worldTransform_.transform_.translate.y = boxSize.y;
			allUpdateMatrix();

			isLanding = true;
		}
	}
	else {
		if (worldTransform_.transform_.translate.y >= worldTransform->transform_.translate.y + (boxSize.y) - (collider_.radius_ * 1.5f)) {
			worldTransform_.transform_.translate.y = worldTransform->transform_.translate.y + boxSize.y;
			allUpdateMatrix();

			isLanding = true;
		}
		else {
			bool isSideHit_ = false;
			bool isVerticalHit_ = false;
			Vector3Calc* v3Calc = Vector3Calc::GetInstance();
			Vector3 length = (v3Calc->Subtract(worldTransform->transform_.translate, worldTransform_.transform_.translate));

			if (sqrtf(length.x * length.x) > boxSize.x) {
				isSideHit_ = true;
				worldTransform_.transform_.translate.x -= velocity_.x;
				allUpdateMatrix();
			}
			if (sqrtf(length.z * length.z) > boxSize.z) {
				isVerticalHit_ = true;
				worldTransform_.transform_.translate.z -= velocity_.z;
				allUpdateMatrix();
			}
			/*if (isSideHit_ && isVerticalHit_) {
				worldTransform_.transform_.translate.z += (velocity_.z / 2.0f);
				allUpdateMatrix();
			}*/

		}
	}
}

/// <summary>
/// 回転
/// </summary>
void Enemy::Rotation() {	
	worldTransform_.transform_.rotate.y += kRotateSpeed;
	if (worldTransform_.transform_.rotate.y >= 2.0f * float(std::numbers::pi)) {
		//worldTransform_.transform_.rotate.y -= 2.0f * float(std::numbers::pi);
	}
	

}

/// <summary>
/// 腕回転ギミック初期化
/// </summary>
void Enemy::InitializeArmRotationgimmick() {

	// 腕回転ギミックの媒介変数
	armRotationParameter_ = 0.0f;
	// 腕回転ギミックのサイクル<frame>
	armRotationPeriod_ = 60;

}

/// <summary>
/// 腕回転ギミック
/// </summary>
void Enemy::UpdateArmRotationgimmick() {

	// 1フレームでのパラメータ加算値
	const float step = 2.0f * float(std::numbers::pi) / armRotationPeriod_;

	armRotationParameter_ += step;
	armRotationParameter_ = std::fmod(armRotationParameter_, 2.0f * float(std::numbers::pi));

	worldTransformL_arm_.transform_.rotate.x = armRotationParameter_;
	worldTransformR_arm_.transform_.rotate.x = armRotationParameter_;

}

void Enemy::GotParent(WorldTransform* parent){
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
	worldTransform_.parent_ = parent;
	worldTransform_.UpdateMatrix();
}

void Enemy::LostParent(){
	Vector3 position = { worldTransform_.worldMatrix_.m[3][0] ,worldTransform_.worldMatrix_.m[3][1] ,worldTransform_.worldMatrix_.m[3][2] };

	worldTransform_.transform_.translate = position;
	worldTransform_.parent_ = nullptr;
	worldTransform_.UpdateMatrix();
}

void Enemy::allUpdateMatrix(){
	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
}

void Enemy::DrawImgui(){
	ImGui::DragFloat3("アイテムの座標", &worldTransform_.transform_.translate.x, 0.1f);
	ImGui::DragFloat3("アイテムの回転", &worldTransform_.transform_.rotate.x, 0.1f);
	ImGui::DragFloat3("アイテムの大きさ", &worldTransform_.transform_.scale.x, 0.1f, 0.0f, 300.0f);
	ImGui::Checkbox("落ちるようにする", &isFall_);
	ImGui::Checkbox("動くようにする", &isMove_);
	if (ImGui::Button("このオブジェを削除")) {
		isDead_ = true;
	}
}
