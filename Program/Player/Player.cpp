#include "../Player/Player.h"
#include "../../Input.h"

void Player::Initialize(const std::vector<Model*>& models,
	const std::vector<Material*>& materials)
{

	//nullポインタチェック
	assert(models.front());
	//基底クラスの初期化
	BaseCharacter::Initialize(models, materials);
	worldTransform_.transform_.translate = kInitialPosition;
	worldTransform_.UpdateMatrix();

	velocity_ = {0.0f,0.0f,0.0f};

	isLanding = false;

	collider_.Initialize(worldTransform_.transform_.translate, kColliderSize);

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

	if (worldTransform_.worldMatrix_.m[3][1] <= -10.0f) {
		Restart();
	}

}

void Player::Draw(const ViewProjection& viewProjection)
{

	for (Model* model : models_) {
		model->Draw(worldTransform_, viewProjection);
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

		const float kThreshold = 0.7f;
		bool isMoving = false;

		// 移動量
		Vector3 move = { input->GetLeftAnalogstick().x, 0.0f, -input->GetLeftAnalogstick().y };
		if (v3Calc->Length(move) > kThreshold) {
			isMoving = true;
		}

		if (isMoving) {

			// 移動量に速さを反映
			move = v3Calc->Multiply(kWalkSpeed, v3Calc->Normalize(move));

			// カメラの角度から回転行列を計算する
			Matrix4x4 rotateMatrixX = m4Calc->MakeRotateXMatrix(viewProjection_->transform_.rotate.x);
			Matrix4x4 rotateMatrixY = m4Calc->MakeRotateYMatrix(viewProjection_->transform_.rotate.y);
			Matrix4x4 rotateMatrixZ = m4Calc->MakeRotateZMatrix(viewProjection_->transform_.rotate.z);
			if (worldTransform_.parent_) {
				rotateMatrixY = m4Calc->MakeRotateYMatrix(viewProjection_->transform_.rotate.y - worldTransform_.parent_->transform_.rotate.y);
			}

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
		else {
			// 移動
			velocity_.x = 0.0f;
			velocity_.z = 0.0f;
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
	if (!worldTransform_.parent_ || 
		(worldTransform_.parent_ != worldTransform)) {
		GotParent(worldTransform);
	}
	worldTransform_.transform_.translate.y = 0.0f;
	isLanding = true;

}

void Player::GotParent(WorldTransform* parent)
{

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

void Player::LostParent()
{

	Vector3 position = { worldTransform_.worldMatrix_.m[3][0] ,worldTransform_.worldMatrix_.m[3][1] ,worldTransform_.worldMatrix_.m[3][2] };

	worldTransform_.transform_.translate = position;
	worldTransform_.parent_ = nullptr;
	worldTransform_.UpdateMatrix();

}
