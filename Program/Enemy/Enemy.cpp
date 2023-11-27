#include <cassert>
#include <cmath>
#include <numbers>

#include "Enemy.h"
/// <summary>
/// 初期化
/// </summary>
/// <param name="model">モデル</param>
/// <param name="textureHandle">テクスチャハンドル</param>
void Enemy::Initialize(const std::vector<Model*>& models,
	const std::vector<Material*>& materials,
	const Vector3& position) {

	// nullポインタチェック
	assert(models.front());

	// 基底クラスの初期化
	BaseCharacter::Initialize(models, materials);

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
	position_ = position;
	worldTransform_.transform_.translate = position_;

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

	collider_ = std::make_unique<Sphere>();
	collider_->Initialize(worldTransform_.transform_.translate, kColliderSize);

	isDead_ = false;

	disappear_ = false;

	color_ = { 1.0f,1.0f,1.0f,1.0f };

	// タイム
	disappearTime_ = 0.0f;
	// タイム
	disappearPeriod_ = 60;

	singleMaterial_.reset(Material::Create());

	blowOffDirection_ = { 0.0f,0.0f,0.0f };

}

/// <summary>
/// 更新
/// </summary>
void Enemy::Update() {

	if (!disappear_) {
		// 回転
		Rotation();

		// 移動
		Move();

		// 腕回転ギミック
		UpdateArmRotationgimmick();

	}
	else {
		DisappearUpdate();
	}

	//ワールド変換データ更新
	worldTransform_.UpdateMatrix();

	worldTransformBody_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();

	collider_->center_ = { worldTransform_.worldMatrix_.m[3][0],worldTransform_.worldMatrix_.m[3][1], worldTransform_.worldMatrix_.m[3][2] };
	collider_->worldTransformUpdate();

}

/// <summary>
/// 描画
/// </summary>
/// <param name="viewProjection">ビュープロジェクション</param>
void Enemy::Draw(const ViewProjection& viewProjection) {

	if (!isDead_) {
		models_[0]->Draw(worldTransformBody_, viewProjection, singleMaterial_.get());
		models_[1]->Draw(worldTransformL_arm_, viewProjection, singleMaterial_.get());
		models_[2]->Draw(worldTransformR_arm_, viewProjection, singleMaterial_.get());
	}

}

/// <summary>
/// 移動
/// </summary>
void Enemy::Move() {


	Matrix4x4Calc* m4Calc = Matrix4x4Calc::GetInstance();
	Vector3Calc* v3Calc = Vector3Calc::GetInstance();
	
	//移動速度
	Vector3 velocity(0.0f, 0.0f, kMoveSpeed);

	//速度ベクトルを向きに合わせて回転させる
	velocity_ = m4Calc->TransformNormal(velocity, worldTransform_.worldMatrix_);

	//移動
	worldTransform_.transform_.translate = v3Calc->Add(worldTransform_.transform_.translate, velocity_);

}

/// <summary>
/// 回転
/// </summary>
void Enemy::Rotation() {

	
	worldTransform_.transform_.rotate.y += kRotateSpeed;
	if (worldTransform_.transform_.rotate.y >= 2.0f * float(std::numbers::pi)) {
		worldTransform_.transform_.rotate.y -= 2.0f * float(std::numbers::pi);
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

void Enemy::DisappearUpdate()
{

	Vector3Calc* v3Calc = Vector3Calc::GetInstance();
	float speed = 1.0f;

	// 吹っ飛ぶ
	worldTransform_.transform_.translate = v3Calc->Add(worldTransform_.transform_.translate,
		v3Calc->Multiply(speed, blowOffDirection_));

	// 消える
	disappearTime_ += 1.0f / disappearPeriod_;
	if (disappearTime_ >= 1.0f) {
		isDead_ = true;
	}
	else {
		color_.w = 1.0f - disappearTime_;
		TransformStructure uvTransform = {
				{1.0f,1.0f,1.0f},
				{0.0f,0.0f,0.0f},
				{0.0f,0.0f,0.0f},
		};
		singleMaterial_->Update(uvTransform, color_, HalfLambert);
	}

}

void Enemy::SetBlowOffDirection(const Vector3& vector)
{

	Vector3Calc* v3Calc = Vector3Calc::GetInstance();

	Vector3 position = { worldTransform_.worldMatrix_.m[3][0],worldTransform_.worldMatrix_.m[3][1], worldTransform_.worldMatrix_.m[3][2] };

	blowOffDirection_ = v3Calc->Normalize(v3Calc->Subtract(position, vector));

}
