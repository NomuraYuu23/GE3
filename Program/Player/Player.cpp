#include "../Player/Player.h"
#include "../../Input.h"

void Player::Initialize(const std::vector<Model*>& models,
	const std::vector<Material*>& materials,
	const ViewProjection* viewProjection)
{

	//nullポインタチェック
	assert(models.front());
	//基底クラスの初期化
	BaseCharacter::Initialize(models, materials, viewProjection);

	velocity_ = {0.0f,0.0f,0.0f};

	isLanding = false;

}

void Player::Update()
{

	Walk();
	Jump();
	Fall();
	Move();
	//行列を定数バッファに転送
	worldTransform_.UpdateMatrix();

	Landing();
	//行列を定数バッファに転送
	worldTransform_.UpdateMatrix();

}

void Player::Draw()
{

	for (Model* model : models_) {
		model->Draw(worldTransform_);
	}

}

void Player::Move()
{

	Vector3Calc* v3Calc = Vector3Calc::GetInstance();
	worldTransform_.transform_.translate = v3Calc->Add(worldTransform_.transform_.translate, velocity_);


}

void Player::Walk()
{

	//インスタンス
	Input* input = Input::GetInstance();
	Vector3Calc* v3Calc = Vector3Calc::GetInstance();
	Matrix4x4Calc* m4Calc = Matrix4x4Calc::GetInstance();

	//移動
	if (input->GetJoystickConnected()) {

		// 移動量
		Vector3 move = { input->GetLeftAnalogstick().x, 0.0f, -input->GetLeftAnalogstick().y };
		// 移動量に速さを反映
		move = v3Calc->Multiply(kSpeed, v3Calc->Normalize(move));

		// カメラの角度から回転行列を計算する
		Matrix4x4 rotateMatrixX = m4Calc->MakeRotateXMatrix(worldTransform_.viewProjection_->transform_.rotate.x);
		Matrix4x4 rotateMatrixY = m4Calc->MakeRotateYMatrix(worldTransform_.viewProjection_->transform_.rotate.y);
		Matrix4x4 rotateMatrixZ = m4Calc->MakeRotateZMatrix(worldTransform_.viewProjection_->transform_.rotate.z);

		Matrix4x4 rotateMatrix = m4Calc->Multiply(
			rotateMatrixX, m4Calc->Multiply(rotateMatrixY, rotateMatrixZ));

		// 移動ベクトルをカメラの角度だけ回転する
		move = m4Calc->TransformNormal(move, rotateMatrix);

		// 移動
		velocity_.x = move.x;
		velocity_.z = move.z;

		// 移動方向に見た目を合わせる(Y軸)
		if (std::fabsf(move.x) > 0.1 || std::fabsf(move.z) > 0.1) {
			worldTransform_.transform_.rotate.y = std::atan2f(move.x, move.z);
		}
	}

}

void Player::Jump()
{

	//インスタンス
	Input* input = Input::GetInstance();

	//移動
	if (input->GetJoystickConnected()) {
		
		if (input->TriggerJoystick(0) && isLanding) {
			velocity_.y += kJumpSpeed;
			isLanding = false;
		}

	}

}

void Player::Fall()
{

	if (!isLanding) {
		velocity_.y -= kFallAcceleration;
	}

}

void Player::Landing()
{

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

void Player::Restart()
{

	worldTransform_.transform_.translate = kInitialPosition;
	worldTransform_.transform_.rotate = kInitialRotate;
	worldTransform_.parent_ = nullptr;
	worldTransform_.UpdateMatrix();

}

void Player::OnCollision(WorldTransform* worldTransform)
{
	GotParent(worldTransform);
	worldTransform_.transform_.translate.y = 0.0f;
	isLanding = true;
}

void Player::GotParent(WorldTransform* parent)
{

	if (worldTransform_.parent_) {
		LostParent();
	}

	Vector3 position = { worldTransform_.transform_.translate.x - parent->transform_.translate.x,
						worldTransform_.transform_.translate.y - parent->transform_.translate.y,
						worldTransform_.transform_.translate.z - parent->transform_.translate.z };

	Vector3 rotate = { worldTransform_.transform_.rotate.x - parent->transform_.rotate.x,
						worldTransform_.transform_.rotate.y - parent->transform_.rotate.y,
						worldTransform_.transform_.rotate.z - parent->transform_.rotate.z };
	
	worldTransform_.transform_.translate = position;
	worldTransform_.transform_.rotate = rotate;
	worldTransform_.parent_ = parent;
	worldTransform_.UpdateMatrix();

}

void Player::LostParent()
{

	Vector3 position = { worldTransform_.worldMatrix_.m[3][0] ,worldTransform_.worldMatrix_.m[3][1] ,worldTransform_.worldMatrix_.m[3][2] };
	Vector3 rotate = { worldTransform_.transform_.rotate.x + worldTransform_.parent_->transform_.rotate.x,
						worldTransform_.transform_.rotate.y + worldTransform_.parent_->transform_.rotate.y,
						worldTransform_.transform_.rotate.z + worldTransform_.parent_->transform_.rotate.z };

	worldTransform_.transform_.translate = position;
	worldTransform_.transform_.rotate = rotate;
	worldTransform_.parent_ = nullptr;
	worldTransform_.UpdateMatrix();

}
