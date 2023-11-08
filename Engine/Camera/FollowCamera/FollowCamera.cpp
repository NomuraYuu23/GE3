#include "FollowCamera.h"
#include "../../Math/Vector3.h"
#include "../../Math/Matrix4x4.h"
#include "../../Input/input.h"
#include "../../Math/Ease.h"

void FollowCamera::Initialize() {

	viewProjection_.Initialize();
	//y固定
	viewProjection_.transform_.translate.y = 10.0f;
	viewProjection_.transform_.rotate.x = 0.1f;

	//ビュー更新
	viewProjection_.UpdateMatrix();

}

void FollowCamera::Update() {

	//インスタンス
	Input* input = Input::GetInstance();
	Vector3Calc* v3Calc = Vector3Calc::GetInstance();
	Matrix4x4Calc* m4Calc = Matrix4x4Calc::GetInstance();

	if (input->GetJoystickConnected()) {

		const float RotateSpeed = 0.000001f;

		viewProjection_.transform_.rotate.y += input->GetRightAnalogstick().x * RotateSpeed;
		viewProjection_.transform_.rotate.x += input->GetRightAnalogstick().y * RotateSpeed;
	}

	//追従対象がいれば
	if (target_) {
		// 追従座標の補間
		Vector3 targetPos = { target_->worldMatrix_.m[3][0], target_->worldMatrix_.m[3][1], target_->worldMatrix_.m[3][2] };
		interTarget_ = Ease::Easing(Ease::EaseName::Lerp, interTarget_, targetPos, rate_);

		// オフセット
		Vector3 offset = OffsetCalc();

		viewProjection_.transform_.translate = v3Calc->Add(interTarget_, offset);

	}

	//y固定
	//viewProjection_.transform_.translate.y = 10.0f;

	//ビュー更新
	viewProjection_.UpdateMatrix();

}

void FollowCamera::Reset()
{

	Vector3Calc* v3Calc = Vector3Calc::GetInstance();

	// 追従対象がいれば
	if (target_) {
		// 追従対象・角度の初期化
		//座標をコピーしてオフセット分ずらす
		interTarget_ = { target_->worldMatrix_.m[3][0], target_->worldMatrix_.m[3][1], target_->worldMatrix_.m[3][2] };
		viewProjection_.transform_.rotate.y = target_->transform_.rotate.y;
	}
	destinationAngleY_ = viewProjection_.transform_.rotate.y;

	// 追従対象からのオフセット
	Vector3 offset = OffsetCalc();
	viewProjection_.transform_.translate = v3Calc->Add(interTarget_,offset);

}

void FollowCamera::SetTarget(const WorldTransform* target)
{
	target_ = target;
	Reset();
}

Vector3 FollowCamera::OffsetCalc() const
{

	Matrix4x4Calc* m4Calc = Matrix4x4Calc::GetInstance();

	//追従対象からカメラまでのオフセット
	Vector3 offset = { 0.0f, 5.0f, -50.0f };

	//カメラの角度から回転行列を計算する
	Matrix4x4 rotateMatrixX = m4Calc->MakeRotateXMatrix(viewProjection_.transform_.rotate.x);
	Matrix4x4 rotateMatrixY = m4Calc->MakeRotateYMatrix(viewProjection_.transform_.rotate.y);
	Matrix4x4 rotateMatrixZ = m4Calc->MakeRotateZMatrix(viewProjection_.transform_.rotate.z);

	Matrix4x4 rotateMatrix = m4Calc->Multiply(
		rotateMatrixX, m4Calc->Multiply(rotateMatrixY, rotateMatrixZ));

	//オフセットをカメラの回転に合わせて回転させる
	offset = m4Calc->TransformNormal(offset, rotateMatrix);

	return offset;

}
