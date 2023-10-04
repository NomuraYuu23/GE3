#include "Player.h"
#include "Input.h"

void Player::Initialize(const std::vector<Model*>& models,
	const std::vector<Material*>& materials,
	const ViewProjection* viewProjection)
{

	//nullポインタチェック
	assert(models.front());
	//基底クラスの初期化
	BaseCharacter::Initialize(models, materials, viewProjection);

}

void Player::Update()
{

	//移動
	Move();

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

	DIJOYSTATE2 joyState;

	//インスタンス
	Input* input = Input::GetInstance();
	Vector3Calc* v3Calc = Vector3Calc::GetInstance();
	Matrix4x4Calc* m4Calc = Matrix4x4Calc::GetInstance();

	//移動
	if (input->GetJoystickConnected()) {

		joyState = input->GetJoystickState();

		// 速さ
		const float speed = 0.3f;

		// 移動量
		Vector3 move = { float(joyState.lX), 0.0f, float(joyState.lY) };
		// 移動量に速さを反映
		move = v3Calc->Multiply(speed, v3Calc->Normalize(move));

		// カメラの角度から回転行列を計算する
		Matrix4x4 rotateMatrixX = m4Calc->MakeRotateXMatrix(worldTransform_.viewProjection_->transform_.rotate.x);
		Matrix4x4 rotateMatrixY = m4Calc->MakeRotateYMatrix(worldTransform_.viewProjection_->transform_.rotate.y);
		Matrix4x4 rotateMatrixZ = m4Calc->MakeRotateZMatrix(worldTransform_.viewProjection_->transform_.rotate.z);

		Matrix4x4 rotateMatrix = m4Calc->Multiply(
			rotateMatrixX, m4Calc->Multiply(rotateMatrixY, rotateMatrixZ));

		// 移動ベクトルをカメラの角度だけ回転する
		move = m4Calc->TransformNormal(move, rotateMatrix);

		// 移動
		worldTransform_.transform_.translate = v3Calc->Add(worldTransform_.transform_.translate, move);

		// 移動方向に見た目を合わせる(Y軸)
		if (std::fabsf(move.x) > 0.1 || std::fabsf(move.z) > 0.1) {
			worldTransform_.transform_.rotate.y = std::atan2f(move.x, move.z);
		}
	}

}
